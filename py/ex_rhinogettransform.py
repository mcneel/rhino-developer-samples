import Rhino
from Rhino.Geometry import *
from Rhino.Input.Custom import *
from Rhino.Commands import *
from scriptcontext import doc
import rhinoscriptsyntax as rs

class GetTranslation(GetTransform):
  def CalculateTransform(self, viewport, point):
    xform = Transform.Identity
    b, base_point = self.TryGetBasePoint()
    if (b):
      v = point - base_point
      if (not v.IsZero):
        xform = Transform.Translation(v)
        if (not xform.IsValid):
          xform = Transform.Identity
    return xform

def RunCommand():
  objectIds = rs.GetObjects("Select objects to move")
  if objectIds is None: return

  gp = GetPoint()
  gp.SetCommandPrompt("Point to move from")
  gp.Get()
  if gp.CommandResult() != Result.Success:
    return gp.CommandResult()

  gt = GetTranslation()
  gt.SetCommandPrompt("Point to move to")
  gt.SetBasePoint(gp.Point(), True)
  gt.DrawLineFromPoint(gp.Point(), True)
  gt.GetXform()
  if gt.CommandResult() != Result.Success:
    return gt.CommandResult()

  list = Rhino.Collections.TransformObjectList()
  for id in objectIds:
      rhobj = rs.coercerhinoobject(id, True, True)
      list.Add(rhobj)
  gt.AddTransformObjects(list)

  xform = gt.CalculateTransform(gt.View().ActiveViewport, gt.Point())
  TransformObjects(list, xform, False, False)
  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()
