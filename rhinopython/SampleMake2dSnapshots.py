################################################################################
# SampleMake2dSnapshots.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################

import Rhino
import System
import scriptcontext as sc

################################################################################
# Creates a unique root layer
################################################################################
def __CreateRootLayer():
    for i in range(1, 99):
        layer_name = "Make2d " + str(i).zfill(2) 
        layer_index = sc.doc.Layers.FindByFullPath(layer_name, -1)
        if layer_index < 0:
            layer_index = sc.doc.Layers.Add(layer_name, System.Drawing.Color.Black)
            return sc.doc.Layers[layer_index].Id
    return System.Guid.Empty

################################################################################
# Creates some sublayer to the root layer
################################################################################
def __CreateSubLayers(layer_name, parent_id):
    
    # Create a layer named snapshot
    layer = Rhino.DocObjects.Layer();
    layer.Name = layer_name
    layer.ParentLayerId = parent_id
    layer_index = sc.doc.Layers.Add(layer)
    layer_id = sc.doc.Layers[layer_index].Id
    
    # Create a sublayer named "Silhouettes" that is hidden
    sublayer0 = Rhino.DocObjects.Layer();
    sublayer0.Name = "Silhouettes"
    sublayer0.IsVisible = False
    sublayer0.PlotWeight = 0.5
    sublayer0.ParentLayerId = layer_id
    sublayer0_index = sc.doc.Layers.Add(sublayer0)
    
    # Create a sublayer named "Curves" that is hidden
    sublayer1 = Rhino.DocObjects.Layer();
    sublayer1.Name = "Curves"
    sublayer1.IsVisible = False
    sublayer1.ParentLayerId = layer_id
    sublayer1_index = sc.doc.Layers.Add(sublayer1)    
    
    return sublayer0_index, sublayer1_index
    
    
################################################################################
# Unpack block references recursively if required
################################################################################
def __AddGeometryToHDLP_Recursive(parameters, obj, xform) : 
    
    # Just add the geometry if there's nothing to recurse into
    if obj.ObjectType != Rhino.DocObjects.ObjectType.InstanceReference:
        parameters.AddGeometry(obj.Geometry, xform, None)
        return
    
    xform = obj.InstanceXform * xform
    
    idef = obj.InstanceDefinition
    
    for o in idef.GetObjects():
        __AddGeometryToHDLP_Recursive(parameters, o, xform)
    

################################################################################
# Computes hidden line drawing curves for a snapshot
################################################################################
def __ComputeHiddenLineDrawing(snapshot, viewport):
    
    # Restore the snapshot
    script = "_-Snapshots _Restore \"" + snapshot + "\" _Enter _Enter"
    Rhino.RhinoApp.RunScript(script, False)
    Rhino.RhinoApp.Wait()
    
    # Create and define hidden line drawing parameters
    parameters = Rhino.Geometry.HiddenLineDrawingParameters()
    parameters.AbsoluteTolerance = sc.doc.ModelAbsoluteTolerance
    parameters.Flatten = True
    parameters.IncludeHiddenCurves = False
    parameters.IncludeTangentEdges = False
    #parameters.IncludeTangentSeams = False
    parameters.SetViewport(viewport)
    
    transform = Rhino.Geometry.Transform(1.0)
    
    # Process all of the document objects
    for obj in sc.doc.Objects:
        if obj.IsSelectable(True, False, False, False):
            __AddGeometryToHDLP_Recursive(parameters, obj, transform)
    
    # Create the hidden line drawing geometry
    return Rhino.Geometry.HiddenLineDrawing.Compute(parameters, True)
    
################################################################################
# Main function
################################################################################
def SampleMake2dSnapshots():
    
    # Unselect everything
    sc.doc.Objects.UnselectAll()
    
    # Verify there are objects in the document
    if 0 == sc.doc.Objects.Count:
        print "No objects to make a 2-D drawing."
        return
    
    # Verify there are snapshots in the document
    snapshots = sc.doc.Snapshots.Names
    if 0 == snapshots.Length:
        print "No snapshots to restore."
        return
    
    # Get the active view
    view = sc.doc.Views.ActiveView
    if view is None: 
        return

    # Verify the active viewport is in a perspective projection
    viewport = view.ActiveViewport
    if not viewport.IsPerspectiveProjection:
        print "Set a perspective view active before running script."
        return
    
    # Store the results of each operation on a layer, with the name
    # of the snapshot, under this layer.
    parent_id = __CreateRootLayer()
    
    # Use this to offset the results of each operation
    origin = Rhino.Geometry.Point3d.Origin
    
    # Save indices of new layers, that are initially hidden, 
    # so we can turn them on later.
    layer_indices = []
    
    # Do this for every snapshot...
    for snapshot in snapshots:
        
        # Create the hidden line drawing (Make2D) geometry
        hld = __ComputeHiddenLineDrawing(snapshot, viewport)
        if hld is None: 
            continue
        
        # Create a transformation that moves the objects to the origin
        bbox = hld.BoundingBox(True)
        dir = origin - Rhino.Geometry.Point3d(bbox.Min.X, bbox.Min.Y, 0.0)
        xform = Rhino.Geometry.Transform.Translation(dir)
        
        # Update the origin, for next use, by offsetting it the length
        # of the bounding box along the xaxis
        dir = bbox.Corner(False, True, True) - bbox.Corner(True, True, True)
        origin = origin + Rhino.Geometry.Vector3d.XAxis * (dir.Length * 1.1)
        
        # Create a sublayer with the name of the snapshot
        silhouette_layer_index, curve_layer_index = __CreateSubLayers(snapshot, parent_id)
        layer_indices.append(silhouette_layer_index)
        layer_indices.append(curve_layer_index)
        
        # Create object attributes that use the sublayer
        attributes = sc.doc.CreateDefaultAttributes()
        
        # Add the results to the document
        for hld_curve in hld.Segments:
            if not hld_curve: 
                continue
            if not hld_curve.ParentCurve: 
                continue
            if hld_curve.ParentCurve.SilhouetteType == Rhino.Geometry.SilhouetteType.None: 
                continue
            if hld_curve.SegmentVisibility == Rhino.Geometry.HiddenLineDrawingSegment.Visibility.Visible: 
                if hld_curve.IsSceneSilhouette:
                    attributes.LayerIndex = silhouette_layer_index
                else:
                    attributes.LayerIndex = curve_layer_index
                # Copy the curve
                curve = hld_curve.CurveGeometry.DuplicateCurve()
                # Flatten the curve
                curve.Transform(xform)
                # Add it to the document
                sc.doc.Objects.AddCurve(curve, attributes)
    
    # Turn on the new, hidden layers
    for layer_index in layer_indices:
        sc.doc.Layers[layer_index].IsVisible = True
    
    # Redraw the document
    sc.doc.Views.Redraw()

################################################################################
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
################################################################################
if __name__ == "__main__":
    SampleMake2dSnapshots()
