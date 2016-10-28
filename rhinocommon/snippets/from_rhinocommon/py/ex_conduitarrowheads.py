import Rhino
import System.Drawing
import scriptcontext
import rhinoscriptsyntax as rs

class DrawArrowHeadsConduit(Rhino.Display.DisplayConduit):
  def __init__(self, line, screenSize, worldSize):
    self.line = line
    self.screenSize = screenSize
    self.worldSize = worldSize

  def DrawForeground(self, e):
    e.Display.DrawArrow(self.line, System.Drawing.Color.Black, self.screenSize, self.worldSize)

def RunCommand():
  # get arrow head size
  go = Rhino.Input.Custom.GetOption()
  go.SetCommandPrompt("ArrowHead length in screen size (pixles) or world size (percentage of arrow lenght)?")
  go.AddOption("screen")
  go.AddOption("world")
  go.Get()
  if (go.CommandResult() != Rhino.Commands.Result.Success):
    return go.CommandResult()

  screenSize = 0
  worldSize = 0.0
  if (go.Option().EnglishName == "screen"):
    gi = Rhino.Input.Custom.GetInteger()
    gi.SetLowerLimit(0,True)
    gi.SetCommandPrompt("Length of arrow head in pixels")
    gi.Get()
    if (gi.CommandResult() != Rhino.Commands.Result.Success):
      return gi.CommandResult()
    screenSize = gi.Number()
  else:
    gi = Rhino.Input.Custom.GetInteger()
    gi.SetLowerLimit(0, True)
    gi.SetUpperLimit(100, False)
    gi.SetCommandPrompt("Lenght of arrow head in percentage of total arrow lenght")
    gi.Get()
    if (gi.CommandResult() != Rhino.Commands.Result.Success):
      return gi.CommandResult()
    worldSize = gi.Number()/100.0


  # get arrow start and end points
  gp = Rhino.Input.Custom.GetPoint()
  gp.SetCommandPrompt("Start of line")
  gp.Get()
  if (gp.CommandResult() != Rhino.Commands.Result.Success):
    return gp.CommandResult()
  ptStart = gp.Point()

  gp.SetCommandPrompt("End of line")
  gp.SetBasePoint(ptStart, False)
  gp.DrawLineFromPoint(ptStart, True)
  gp.Get()
  if (gp.CommandResult() != Rhino.Commands.Result.Success):
    return gp.CommandResult()
  ptEnd = gp.Point()


  v = ptEnd - ptStart
  if (v.IsTiny(Rhino.RhinoMath.ZeroTolerance)):
    return Rhino.Commands.Result.Nothing

  line = Rhino.Geometry.Line(ptStart, ptEnd)

  conduit = DrawArrowHeadsConduit(line, screenSize, worldSize)
  conduit.Enabled = True
  scriptcontext.doc.Views.Redraw()
  rs.GetString("Pausing for user input")
  conduit.Enabled = False
  scriptcontext.doc.Views.Redraw()

  return Rhino.Commands.Result.Success

if __name__=="__main__":
  RunCommand()
