from System import *
from Rhino import *
from Rhino.DocObjects import *
from Rhino.Geometry import *
from Rhino.Input import *
from Rhino.Input.Custom import *
from Rhino.Commands import *
from scriptcontext import doc

def RunCommand():
  filter = ObjectType.Surface | ObjectType.PolysrfFilter | ObjectType.Mesh
  rc, obj_refs = RhinoGet.GetMultipleObjects("Select objects to contour", False, filter)
  if rc != Result.Success:
    return rc

  gp = GetPoint()
  gp.SetCommandPrompt("Contour plane base point")
  gp.Get()
  if gp.CommandResult() != Result.Success:
    return gp.CommandResult()
  base_point = gp.Point()

  gp.DrawLineFromPoint(base_point, True)
  gp.SetCommandPrompt("Direction perpendicular to contour planes")
  gp.Get()
  if gp.CommandResult() != Result.Success:
    return gp.CommandResult()
  end_point = gp.Point()

  if base_point.DistanceTo(end_point) < RhinoMath.ZeroTolerance:
    return Result.Nothing

  distance = 1.0
  rc, distance = RhinoGet.GetNumber("Distance between contours", False, distance)
  if rc != Result.Success:
    return rc

  interval = Math.Abs(distance)

  for obj_ref in obj_refs:
    geometry = obj_ref.Geometry()
    if geometry == None:
      return Result.Failure

    if type(geometry) == Brep:
      curves = Brep.CreateContourCurves(geometry, base_point, end_point, interval)
    else:
      curves = Mesh.CreateContourCurves(geometry, base_point, end_point, interval)

    for curve in curves:
      curve_object_id = doc.Objects.AddCurve(curve)
      doc.Objects.Select(curve_object_id)

  if curves != None:
    doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()
