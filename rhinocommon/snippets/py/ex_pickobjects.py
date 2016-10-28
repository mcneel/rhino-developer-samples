import System.Drawing
import Rhino.Input.Custom
from scriptcontext import doc

def RunCommand():
  conduitPoints = []
  conduit = PointsConduit(conduitPoints)
  conduit.Enabled = True

  gp = Rhino.Input.Custom.GetPoint()
  result = Rhino.Commands.Result.Success

  while True:
    gp.SetCommandPrompt("click location to create point. (<ESC> exit)")
    gp.AcceptNothing(True)
    gp.Get()
    result = gp.CommandResult()
    if result != Rhino.Commands.Result.Success:
      break
    conduitPoints.append(ConduitPoint(gp.Point()))
    doc.Views.Redraw()
    
  gcp = GetConduitPoint(conduitPoints)
  result = Rhino.Commands.Result.Success
  
  while True:
    gcp.SetCommandPrompt("select conduit point. (<ESC> to exit)")
    gcp.AcceptNothing(True)
    gcp.Get(True)
    doc.Views.Redraw()
    if gcp.CommandResult() != Rhino.Commands.Result.Success:
      break
    
  return Rhino.Commands.Result.Success

class ConduitPoint():
  def __init__(self, point):
    self.Color = System.Drawing.Color.White
    self.Point = point

class GetConduitPoint(Rhino.Input.Custom.GetPoint):
  def __init__(self, conduitPoints):
    self.conduitPoints = conduitPoints

  def OnMouseDown(self, getPointMouseEventArgs):
    picker = PickContext()
    picker.View = getPointMouseEventArgs.Viewport.ParentView

    picker.PickStyle = PickStyle.PointPick

    xform = getPointMouseEventArgs.Viewport.GetPickTransform(getPointMouseEventArgs.WindowPoint)
    picker.SetPickTransform(xform)

    for cp in conduitPoints:
      b, depth, distance = picker.PickFrustumTest(cp.Point)
      if b:
        cp.Color = System.Drawing.Color.Red
      else:
        cp.Color = System.Drawing.Color.White

class PointsConduit(Rhino.Display.DisplayConduit):
  def __init__(self, conduitPoints ):
    self.conduitPoints = conduitPoints

  def DrawForeground(self, drawEventArgs):
    for cp in conduitPoints:
      drawEventArgs.Display.DrawPoint(cp.Point, PointStyle.Simple, 3, cp.Color)

if __name__ == "__main__":
  RunCommand()
