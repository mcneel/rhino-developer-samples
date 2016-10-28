from Rhino import *
from Rhino.DocObjects import *
from Rhino.Geometry import *
from Rhino.Input import *
from Rhino.Commands import *
from scriptcontext import doc
import rhinoscriptsyntax as rs

def RunCommand():
  rs, obj_ref = RhinoGet.GetOneObject("Select Curve", False, ObjectType.Curve)
  if rs <> Result.Success:
    return rs
  curve = obj_ref.Curve()
  if curve == None:
    return Result.Nothing

  rs, point = RhinoGet.GetPoint("Select Side", False)
  if rs <> Result.Success:
    return rs
  if point == Point3d.Unset:
    return Result.Nothing

  curves = curve.Offset(point, Vector3d.ZAxis, 1.0,
    doc.ModelAbsoluteTolerance, CurveOffsetCornerStyle.None)

  for offset_curve in curves:
    doc.Objects.AddCurve(offset_curve)

  doc.Views.Redraw()
  return Result.Success

if __name__ == "__main__":
  RunCommand()
