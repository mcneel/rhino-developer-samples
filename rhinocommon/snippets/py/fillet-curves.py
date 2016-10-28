from Rhino import *
from Rhino.Commands import *
from Rhino.Geometry import *
from Rhino.Input import *
from Rhino.DocObjects import *
from Rhino.Input.Custom import *
from scriptcontext import doc

def RunCommand():
  gc1 = GetObject()
  gc1.DisablePreSelect()
  gc1.SetCommandPrompt("Select first curve to fillet (close to the end you want to fillet)")
  gc1.GeometryFilter = ObjectType.Curve
  gc1.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
  gc1.Get()
  if gc1.CommandResult() != Result.Success:
    return gc1.CommandResult()
  curve1_obj_ref = gc1.Object(0)
  curve1 = curve1_obj_ref.Curve()
  if curve1 == None: return Result.Failure
  curve1_point_near_end = curve1_obj_ref.SelectionPoint()
  if curve1_point_near_end == Point3d.Unset:
    return Result.Failure

  gc2 = GetObject()
  gc2.DisablePreSelect()
  gc2.SetCommandPrompt("Select second curve to fillet (close to the end you want to fillet)")
  gc2.GeometryFilter = ObjectType.Curve
  gc2.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
  gc2.Get()
  if gc2.CommandResult() != Result.Success:
    return gc2.CommandResult()
  curve2_obj_ref = gc2.Object(0)
  curve2 = curve2_obj_ref.Curve()
  if curve2 == None: return Result.Failure
  curve2_point_near_end = curve2_obj_ref.SelectionPoint()
  if curve2_point_near_end == Point3d.Unset:
    return Result.Failure

  radius = 0.0
  rc, radius = RhinoGet.GetNumber("fillet radius", False, radius)
  if rc != Result.Success: return rc

  fillet_curve = Curve.CreateFilletCurves(curve1, curve1_point_near_end, curve2, curve2_point_near_end, radius,
    True, True, True, doc.ModelAbsoluteTolerance, doc.ModelAngleToleranceDegrees)
  if fillet_curve == None or fillet_curve.Length != 1:
    return Result.Failure

  doc.Objects.AddCurve(fillet_curve[0])
  doc.Views.Redraw()
  return rc

if __name__ == "__main__":
  RunCommand()
