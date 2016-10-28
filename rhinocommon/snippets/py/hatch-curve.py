import Rhino
import scriptcontext

def HatchCurve():
    go = Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select closed planar curve")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve
    go.SubObjectSelect = False
    go.Get()
    if go.CommandResult()!=Rhino.Commands.Result.Success: return

    curve = go.Object(0).Curve()
    if (not curve or not curve.IsClosed or not curve.IsPlanar()): return

    hatch_name = scriptcontext.doc.HatchPatterns[scriptcontext.doc.HatchPatterns.CurrentHatchPatternIndex].Name
    rc, hatch_name = Rhino.Input.RhinoGet.GetString("Hatch pattern", True, hatch_name)
    if rc!=Rhino.Commands.Result.Success or not hatch_name: return

    index = scriptcontext.doc.HatchPatterns.Find(hatch_name, True)
    if index<0:
        print "Hatch pattern does not exist."
        return

    hatches = Rhino.Geometry.Hatch.Create(curve, index, 0, 1)
    for hatch in hatches:
        scriptcontext.doc.Objects.AddHatch(hatch)
    if hatches: scriptcontext.doc.Views.Redraw()

if __name__=="__main__":
    HatchCurve()
