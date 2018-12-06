from Rhino import *
from Rhino.Geometry import *
from Rhino.Commands import *
from Rhino.Input.Custom import *
from scriptcontext import doc
from System.Drawing import *

def RunCommand():
  gp = GetPoint()
  gp.SetCommandPrompt("Center point")
  gp.Get()
  if gp.CommandResult() != Result.Success:
    return gp.CommandResult()
  center_point = gp.Point()
  if center_point == Point3d.Unset:
    return Result.Failure

  gcp = GetCircleRadiusPoint(center_point)
  gcp.SetCommandPrompt("Radius")
  gcp.ConstrainToConstructionPlane(False)
  gcp.SetBasePoint(center_point, True)
  gcp.DrawLineFromPoint(center_point, True)
  gcp.Get()
  if gcp.CommandResult() != Result.Success:
    return gcp.CommandResult()

  radius = center_point.DistanceTo(gcp.Point())
  cplane = doc.Views.ActiveView.ActiveViewport.ConstructionPlane()
  doc.Objects.AddCircle(Circle(cplane, center_point, radius))
  doc.Views.Redraw()
  return Result.Success

class GetCircleRadiusPoint (GetPoint):
  def __init__(self, centerPoint):
    self.m_center_point = centerPoint

  def OnDynamicDraw(self, e):
    cplane = e.RhinoDoc.Views.ActiveView.ActiveViewport.ConstructionPlane()
    radius = self.m_center_point.DistanceTo(e.CurrentPoint)
    circle = Circle(cplane, self.m_center_point, radius)
    e.Display.DrawCircle(circle, Color.Black)

if __name__ == "__main__":
    RunCommand()
