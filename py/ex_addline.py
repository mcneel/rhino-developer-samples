import Rhino
import scriptcontext
import System.Guid

def AddLine():
    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Start of line")
    gp.Get()
    if gp.CommandResult()!=Rhino.Commands.Result.Success:
        return gp.CommandResult()
    pt_start = gp.Point()

    gp.SetCommandPrompt("End of line")
    gp.SetBasePoint(pt_start, False)
    gp.DrawLineFromPoint(pt_start, True)
    gp.Get()
    if gp.CommandResult() != Rhino.Commands.Result.Success:
        return gp.CommandResult()
    pt_end = gp.Point()
    v = pt_end - pt_start
    if v.IsTiny(Rhino.RhinoMath.ZeroTolerance):
        return Rhino.Commands.Result.Nothing
    
    id = scriptcontext.doc.Objects.AddLine(pt_start, pt_end)
    if id!=System.Guid.Empty:
        scriptcontext.doc.Views.Redraw()
        return Rhino.Commands.Result.Success
    return Rhino.Commands.Result.Failure

if __name__=="__main__":
    AddLine()
