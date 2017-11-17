################################################################################
# SamplePlaneSurfaceHoleMaker.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc

# Demonstrates how to quickly punch a bunch of holes in a planar surface.
def SamplePlaneSurfaceHoleMaker():
    
    # Select planar surface
    gs = Rhino.Input.Custom.GetObject()
    gs.SetCommandPrompt("Select planar surface")
    gs.GeometryFilter = Rhino.DocObjects.ObjectType.Surface
    gs.SubObjectSelect = False
    gs.Get()
    if (gs.CommandResult() != Rhino.Commands.Result.Success):
        return
    
    brep_ref = gs.Object(0)
    brep = brep_ref.Brep()
    if not brep:
        return
    
    # Verify underlying surface is a PlaneSurface
    if not type(brep.Faces[0].UnderlyingSurface()) is Rhino.Geometry.PlaneSurface:
        return
    
    # Select trimming curves on planar surface
    gc = Rhino.Input.Custom.GetObject()
    gc.SetCommandPrompt("Select trimming curves on planar surface")
    gc.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    gc.GroupSelect = True
    gc.SubObjectSelect = False
    gc.EnablePreSelect(False, True)
    gc.DeselectAllBeforePostSelect = False
    gc.GetMultiple(1, 0)
    if (gc.CommandResult() != Rhino.Commands.Result.Success):
        return
    
     # Make a copy of the selected Brep
    new_brep = Rhino.Geometry.Brep()
    new_brep.Append(brep)
    boundary = []
    
    for i in range(gc.ObjectCount):
        curve = gc.Object(i).Curve()
        if curve:
          boundary.append(curve)
          new_brep.Loops.AddPlanarFaceLoop(0, Rhino.Geometry.BrepLoopType.Inner, [boundary.pop()])

    new_brep.Faces[0].RebuildEdges(sc.doc.ModelAbsoluteTolerance, True, True)
    new_brep.Compact()

    if new_brep.IsValid:
        sc.doc.Objects.Replace(brep_ref.ObjectId, new_brep)
        sc.doc.Views.Redraw()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SamplePlaneSurfaceHoleMaker()