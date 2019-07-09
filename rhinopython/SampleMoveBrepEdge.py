################################################################################
# SampleMoveBrepEdge.py
# Copyright (c) 2019 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

def SampleMoveBrepEdge():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select edge to move")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.EdgeCurve
    go.Get()
    if go.CommandResult() != Rhino.Commands.Result.Success: return
    
    obj_ref = go.Object(0)
    edge = obj_ref.Edge()
    if not edge: return
    brep = edge.Brep
    if not brep: return
    
    rc, p0 = Rhino.Input.RhinoGet.GetPoint("Point to move from", False)
    if rc != Rhino.Commands.Result.Success: return
        
    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Point to move to")
    gp.DrawLineFromPoint(p0, False)
    gp.Get()
    if gp.CommandResult() != Rhino.Commands.Result.Success: return
    
    dir = gp.Point() - p0
    xform = Rhino.Geometry.Transform.Translation(dir)
    
    new_brep = brep.DuplicateBrep()
    edge_ci = brep.Edges[edge.EdgeIndex].ComponentIndex()
    list = [edge_ci]
    
    rc = new_brep.TransformComponent(list, xform, sc.doc.ModelAbsoluteTolerance, 0, False)
    if rc:
        sc.doc.Objects.Replace(obj_ref.Object().Id, new_brep)
        sc.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleMoveBrepEdge() # Call the function defined above