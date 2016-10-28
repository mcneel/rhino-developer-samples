from Rhino import *
from Rhino.DocObjects import *
from Rhino.Geometry import *
from Rhino.Commands import *
from Rhino.Input.Custom import *
from System.Drawing import Color

def RunCommand():
  gp = GetDrawStringPoint()
  gp.SetCommandPrompt("Point")
  gp.Get()
  return gp.CommandResult()

class GetDrawStringPoint(GetPoint):
  def OnDynamicDraw(self, e):
    xform = e.Viewport.GetTransform(
      CoordinateSystem.World, CoordinateSystem.Screen)

    current_point = e.CurrentPoint
    current_point.Transform(xform)
    screen_point = Point2d(current_point.X, current_point.Y)

    msg = "screen {0}, {1}".format(screen_point.X, screen_point.Y)
    e.Display.Draw2dText(msg, Color.Blue, screen_point, False)

if __name__ == "__main__":
  RunCommand()
