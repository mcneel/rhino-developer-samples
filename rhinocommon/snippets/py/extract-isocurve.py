from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.Input.Custom import *
from Rhino.Geometry import *
from scriptcontext import doc

def RunCommand():
  rc, obj_ref = RhinoGet.GetOneObject("Select surface", False, ObjectType.Surface)
  if rc != Result.Success or obj_ref == None:
    return rc
  surface = obj_ref.Surface()

  gp = GetPoint()
  gp.SetCommandPrompt("Point on surface")
  gp.Constrain(surface, False)
  option_toggle = OptionToggle(False, "U", "V")
  gp.AddOptionToggle("Direction", option_toggle)
  point = Point3d.Unset

  while True:
    grc = gp.Get()
    if grc == GetResult.Option:
      continue
    elif grc == GetResult.Point:
      point = gp.Point()
      break
    else:
      return Result.Nothing

  if point == Point3d.Unset:
    return Result.Nothing

  direction = 1 if option_toggle.CurrentValue else 0
  b, u_parameter, v_parameter = surface.ClosestPoint(point)
  if not b: return Result.Failure

  iso_curve = surface.IsoCurve(direction, u_parameter if direction == 1 else v_parameter)
  if iso_curve == None:
    return Result.Failure

  doc.Objects.AddCurve(iso_curve)
  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()
