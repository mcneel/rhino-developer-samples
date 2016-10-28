import Rhino
import scriptcontext

def BooleanDifference():
    filter = Rhino.DocObjects.ObjectType.PolysrfFilter
    rc, objrefs = Rhino.Input.RhinoGet.GetMultipleObjects("Select first set of polysurfaces", False, filter)
    if rc != Rhino.Commands.Result.Success: return rc
    if not objrefs: return Rhino.Commands.Result.Failure

    in_breps0 = []
    for objref in objrefs:
        brep = objref.Brep()
        if brep: in_breps0.append(brep)

    scriptcontext.doc.Objects.UnselectAll()
    rc, objrefs = Rhino.Input.RhinoGet.GetMultipleObjects("Select second set of polysurfaces", False, filter)
    if rc != Rhino.Commands.Result.Success: return rc
    if not objrefs: return Rhino.Commands.Result.Failure

    in_breps1 = []
    for objref in objrefs:
        brep = objref.Brep()
        if brep: in_breps1.append(brep)

    tolerance = scriptcontext.doc.ModelAbsoluteTolerance
    breps = Rhino.Geometry.Brep.CreateBooleanDifference(in_breps0, in_breps1, tolerance)
    if not breps: return Rhino.Commands.Result.Nothing
    for brep in breps: scriptcontext.doc.Objects.AddBrep(brep)
    scriptcontext.doc.Views.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    BooleanDifference()
