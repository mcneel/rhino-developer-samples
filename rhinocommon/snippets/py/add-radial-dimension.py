from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Geometry import *
from Rhino.Input import *
from scriptcontext import doc

def RunCommand():
    rc, obj_ref = RhinoGet.GetOneObject("Select curve for radius dimension", True, ObjectType.Curve)
    if rc != Result.Success:
        return rc
    curve, curve_parameter = obj_ref.CurveParameter()
    if curve == None:
        return Result.Failure

    if curve.IsLinear() or curve.IsPolyline():
        print "Curve must be non-linear."
        return Result.Nothing

    # in this example just deal with planar curves
    if not curve.IsPlanar():
        print "Curve must be planar."
        return Result.Nothing

    point_on_curve = curve.PointAt(curve_parameter)
    curvature_vector = curve.CurvatureAt(curve_parameter)
    len = curvature_vector.Length
    if len < RhinoMath.SqrtEpsilon:
        print "Curve is almost linear and therefore has no curvature."
        return Result.Nothing

    center = point_on_curve + (curvature_vector/(len*len))
    _, plane = curve.TryGetPlane()
    radial_dimension = RadialDimension(center, point_on_curve, plane.XAxis, plane.Normal, 5.0)
    doc.Objects.AddRadialDimension(radial_dimension)
    doc.Views.Redraw()
    return Result.Success

if __name__=="__main__":
    RunCommand()
