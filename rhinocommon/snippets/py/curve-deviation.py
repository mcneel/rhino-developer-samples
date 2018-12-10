import rhinoscriptsyntax as rs
import scriptcontext
import Rhino

def RunCommand():
    crvA = rs.GetCurveObject("first curve")[0]
    crvA = rs.coercecurve(crvA)
    crvB = rs.GetCurveObject("second curve")[0]
    crvB = rs.coercecurve(crvB)
    if crvA == None or crvB == None:
        return Rhino.Commands.Result.Failure

    maxa, maxb, maxd, mina, minb, mind = rs.CurveDeviation(crvA, crvB)

    if mind <= Rhino.RhinoMath.ZeroTolerance:
        mind = 0.0;
    maxDistPtA = crvA.PointAt(maxa)
    maxDistPtB = crvB.PointAt(maxb)
    minDistPtA = crvA.PointAt(mina)
    minDistPtB = crvB.PointAt(minb)

    print "Minimum deviation = {0}   pointA({1}, {2}, {3}), pointB({4}, {5}, {6})".format(mind, minDistPtA.X, minDistPtA.Y, minDistPtA.Z, minDistPtB.X, minDistPtB.Y, minDistPtB.Z)
    print "Maximum deviation = {0}   pointA({1}, {2}, {3}), pointB({4}, {5}, {6})".format(maxd, maxDistPtA.X, maxDistPtA.Y, maxDistPtA.Z, maxDistPtB.X, maxDistPtB.Y, maxDistPtB.Z)

if __name__=="__main__":
    RunCommand()
