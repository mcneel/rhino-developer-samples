import System
from Rhino.Geometry import NurbsCurve
from Rhino.Commands import Result
from scriptcontext import doc

def AddNurbsCircle():
    # The easy way to get a NURBS curve from a circle is with
    # the following two lines of code.
    #
    # Circle c = new Circle(20)
    # NurbsCurve nc = c.ToNurbsCurve()
    #
    # This sample demonstrates creating a NURBS curve from scratch.
    dimension = 3
    isRational = True
    order = 3
    cv_count = 9
    nc = NurbsCurve(dimension, isRational, order, cv_count)
    nc.Points.SetPoint(0, 1.0, 0.0, 0.0, 1.0)
    nc.Points.SetPoint(1, 1.0, 1.0, 0.0, 0.707107)
    nc.Points.SetPoint(2, 0.0, 1.0, 0.0, 1.0)
    nc.Points.SetPoint(3, -1.0, 1.0, 0.0, 0.707107)
    nc.Points.SetPoint(4, -1.0, 0.0, 0.0, 1.0)
    nc.Points.SetPoint(5, -1.0, -1.0, 0.0, 0.707107)
    nc.Points.SetPoint(6, 0.0, -1.0, 0.0, 1.0)
    nc.Points.SetPoint(7, 1.0, -1.0, 0.0, 0.707107)
    nc.Points.SetPoint(8, 1.0, 0.0, 0.0, 1.0)
    nc.Knots[0] = 0.0
    nc.Knots[1] = 0.0
    nc.Knots[2] = 0.5 * System.Math.PI
    nc.Knots[3] = 0.5 * System.Math.PI
    nc.Knots[4] = System.Math.PI
    nc.Knots[5] = System.Math.PI
    nc.Knots[6] = 1.5 * System.Math.PI
    nc.Knots[7] = 1.5 * System.Math.PI
    nc.Knots[8] = 2.0 * System.Math.PI
    nc.Knots[9] = 2.0 * System.Math.PI
    if nc.IsValid:
        doc.Objects.AddCurve(nc)
        doc.Views.Redraw()
        return Result.Success
    return Result.Failure

if __name__=="__main__":
    AddNurbsCircle()
