from Rhino import *
from Rhino.ApplicationSettings import *
from Rhino.Commands import *
from Rhino.Input.Custom import *
from scriptcontext import doc

def RunCommand():
    gp = GetPoint()
    gp.SetCommandPrompt("Start of line")
    gp.Get()
    if gp.CommandResult() != Result.Success:
        return gp.CommandResult()
    start_point = gp.Point()

    original_ortho = ModelAidSettings.Ortho
    if not original_ortho:
        ModelAidSettings.Ortho = True

    gp.SetCommandPrompt("End of line")
    gp.SetBasePoint(start_point, False)
    gp.DrawLineFromPoint(start_point, True)
    gp.Get()
    if gp.CommandResult() != Result.Success:
        return gp.CommandResult()
    end_point = gp.Point()

    if ModelAidSettings.Ortho != original_ortho:
        ModelAidSettings.Ortho = original_ortho

    doc.Objects.AddLine(start_point, end_point)
    doc.Views.Redraw()
    return Result.Success

if __name__ == "__main__":
    RunCommand()
