################################################################################
# SampleTextBox.py
# Copyright (c) 20197 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import rhinoscriptsyntax as rs
import scriptcontext as sc
import clr

# GetObject-derived class for limiting object picking to text
class GetTextObject(Rhino.Input.Custom.GetObject):
    def CustomGeometryFilter(self, rh_object, geometry, component_index):
        if not rh_object: 
            return False
        if not geometry: 
            return False
        if not isinstance(geometry, Rhino.Geometry.TextEntity): 
            return False
        return True

# Draws a bounding rectangle around selected text
def SampleTextBox():
    go = GetTextObject()
    go.SetCommandPrompt("Select text")
    go.Get()
    if go.CommandResult() != Rhino.Commands.Result.Success:
        return
    
    obj = go.Object(0).Object()
    text = extrusion_obj = clr.Convert(obj, Rhino.DocObjects.TextObject )
    if not text:
        return
    
    plane = text.TextGeometry.Plane
    xform = Rhino.Geometry.Transform.ChangeBasis(Rhino.Geometry.Plane.WorldXY, plane)
    curves = text.TextGeometry.Explode()
    
    tbox = Rhino.Geometry.BoundingBox.Empty
    for crv in curves:
        bbox = crv.GetBoundingBox(xform)
        tbox.Union(bbox)
    
    rmin = Rhino.Geometry.Interval(tbox.Min.X, tbox.Max.X)
    rmax = Rhino.Geometry.Interval(tbox.Min.Y, tbox.Max.Y)
    rect = Rhino.Geometry.Rectangle3d(plane, rmin, rmax)
    pline = rect.ToPolyline()
    curve = Rhino.Geometry.PolylineCurve(pline)
    
    # Scale based on a percentage of the height of the rectangle.
    # Change this value to adjust padding
    scale = 0.1
    height = rect.Corner(0).DistanceTo(rect.Corner(3)) * scale
    
    tol = sc.doc.ModelAbsoluteTolerance
    corner_style = Rhino.Geometry.CurveOffsetCornerStyle.Sharp
    offsets = curve.Offset(plane, height, tol, corner_style)
    
    sc.doc.Objects.AddCurve(offsets[0])
    sc.doc.Views.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleTextBox()