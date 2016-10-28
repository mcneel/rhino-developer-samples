from System import *
from  Rhino import *
from  Rhino.Commands import *
from  Rhino.DocObjects import *
from  Rhino.Geometry import *
from  Rhino.Input import *
from scriptcontext import doc

def RunCommand():

  rc, obj_ref = RhinoGet.GetOneObject("Select curve to reparameterize", False, ObjectType.Curve)
  if rc <> Result.Success:
    return rc
  curve = obj_ref.Curve()
  if curve == None:
    return Result.Failure

  domain_start = 0
  rc, domain_start = RhinoGet.GetNumber("Domain start", False, domain_start)
  if rc <> Result.Success:
    return rc

  domain_end = 100
  rc, domain_end = RhinoGet.GetNumber("Domain end", False, domain_end)
  if rc <> Result.Success:
    return rc

  if Math.Abs(curve.Domain.T0 - domain_start) < RhinoMath.ZeroTolerance and \
      Math.Abs(curve.Domain.T1 - domain_end) < RhinoMath.ZeroTolerance:
    return Result.Nothing

  curve_copy = curve.DuplicateCurve()
  curve_copy.Domain = Interval(domain_start, domain_end)
  if not doc.Objects.Replace(obj_ref, curve_copy):
    return Result.Failure
  else:
    doc.Views.Redraw()
    return Result.Success

if __name__ == "__main__":
  RunCommand()
