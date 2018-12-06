from Rhino import *
from Rhino.Geometry import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.Input.Custom import *
from scriptcontext import doc

def RunCommand():

  rc, boundary_obj_refs = RhinoGet.GetMultipleObjects("Select boundary objects", False, ObjectType.AnyObject)
  if rc != Result.Success:
    return rc
  if boundary_obj_refs == None or boundary_obj_refs.Length == 0:
    return Result.Nothing

  gc = GetObject()
  gc.SetCommandPrompt("Select curve to extend")
  gc.GeometryFilter = ObjectType.Curve
  gc.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
  gc.Get()
  if gc.CommandResult() != Result.Success:
    return gc.CommandResult()
  curve_obj_ref = gc.Object(0)

  curve = curve_obj_ref.Curve()
  if curve == None: return Result.Failure
  b, t = curve.ClosestPoint(curve_obj_ref.SelectionPoint())
  if not b: return Result.Failure
  curve_end = CurveEnd.Start if t <= curve.Domain.Mid else CurveEnd.End

  geometry = [obj.Geometry() for obj in boundary_obj_refs]
  extended_curve = curve.Extend(curve_end, CurveExtensionStyle.Line, geometry)
  if extended_curve != None and extended_curve.IsValid:
    if not doc.Objects.Replace(curve_obj_ref.ObjectId, extended_curve):
      return Result.Failure
    doc.Views.Redraw()
    return Result.Success
  else:
    RhinoApp.WriteLine("No boundary object was intersected so curve not extended")
    return Result.Nothing

if __name__ == "__main__":
  RunCommand()
