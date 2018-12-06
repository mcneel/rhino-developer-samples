from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.Input.Custom import *
from Rhino.Geometry import *
from scriptcontext import doc

def RunCommand():
  rc, obj_refs = RhinoGet.GetMultipleObjects("Select points to move", False, ObjectType.Point)
  if rc != Result.Success or obj_refs == None:
    return rc

  gp = GetPoint()
  gp.SetCommandPrompt("Point to move from")
  gp.Get()
  if gp.CommandResult() != Result.Success:
    return gp.CommandResult()
  start_point = gp.Point()

  gp.SetCommandPrompt("Point to move to")
  gp.SetBasePoint(start_point, False)
  gp.DrawLineFromPoint(start_point, True)
  gp.Get()
  if gp.CommandResult() != Result.Success:
    return gp.CommandResult()
  end_point = gp.Point()

  xform = Transform.Translation(end_point - start_point)

  for obj_ref in obj_refs:
    doc.Objects.Transform(obj_ref, xform, True)

  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()
