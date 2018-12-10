import rhinoscriptsyntax as rs
from scriptcontext import *
import Rhino

def circleWithRadiusOf10GeometryFilter (rhObject, geometry, componentIndex):
    isCircleWithRadiusOf10 = False
    c = rs.coercecurve(geometry)
    if c:
        b, circle = c.TryGetCircle()
    if b:
        isCircleWithRadiusOf10 = circle.Radius <= 10.0 + Rhino.RhinoMath.ZeroTolerance and circle.Radius >= 10.0 - Rhino.RhinoMath.ZeroTolerance
    return isCircleWithRadiusOf10

def RunCommand():
    # only use a custom geometry filter if no simpler filter does the job

    # for curves - only a simple filter is needed
    if rs.GetObject("select curve", rs.filter.curve): #Rhino.DocObjects.ObjectType.Curve):
        print "curve vas selected"

    # for circles with a radius of 10 - a custom geometry filter is needed
    if rs.GetObject("select circle with radius of 10", rs.filter.curve, False, False, circleWithRadiusOf10GeometryFilter):
        print "circle with radius of 10 was selected"

if __name__=="__main__":
    RunCommand()
