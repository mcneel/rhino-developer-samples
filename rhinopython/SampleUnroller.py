################################################################################
# SampleUnroller.py
# MIT License - Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import Rhino
import scriptcontext as sc
import System.Collections.Generic.IEnumerable as IEnumerable

# Demonstrates how to use Rhino.Geometry.Unroller
def SampleUnroller():
    
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select object to unroll")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.PolysrfFilter
    go.Get()
    if go.CommandResult() != Rhino.Commands.Result.Success: return
    
    brep = go.Object(0).Brep()
    if not brep: return
    
    gd = Rhino.Input.Custom.GetObject()
    gd.SetCommandPrompt("Select text dots that follow")
    gd.GeometryFilter = Rhino.DocObjects.ObjectType.TextDot
    gd.EnablePreSelect(False, True)
    gd.DeselectAllBeforePostSelect = False
    gd.GetMultiple(1, 0)
    if gd.CommandResult() != Rhino.Commands.Result.Success: return
    
    dots = []
    for obj_ref in gd.Objects():
        dots.append(obj_ref.TextDot())
    
    unroller = Rhino.Geometry.Unroller(brep)
    unroller.AbsoluteTolerance = sc.doc.ModelAbsoluteTolerance
    
    # IronPython doesn't really have overloads of functions, just one single
    # function with all the functionality. Normally IronPython would do this
    # automatically but the generic types complicate things. To disambiguate
    # which overload to use, get the function using the Overloads dictionary
    # passing the types in the signature as the key.
    unroller.AddFollowingGeometry.Overloads[IEnumerable[Rhino.Geometry.TextDot]](dots)
    
    out_breps, out_curves, out_points, out_dots = unroller.PerformUnroll()
    if out_breps:
        attribs = go.Object(0).Object().Attributes
        attribs.RemoveFromAllGroups();
        for b in out_breps:
          sc.doc.Objects.Add(b, attribs);
    
    if out_dots:
        for i in range(len(out_dots)):
            d = out_dots[i]
            attribs = sc.doc.CreateDefaultAttributes();
            index = unroller.FollowingGeometryIndex(d);
            if index >= 0:
                attribs = gd.Object(i).Object().Attributes
                attribs.RemoveFromAllGroups()
            sc.doc.Objects.Add(d, attribs)
    
    sc.doc.Views.Redraw()

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleUnroller()