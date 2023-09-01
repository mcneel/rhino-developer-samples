import rhino3dm
import math

def info(arc, name):
    print(name + ' IsValid: ' + str(arc.IsValid))
    print(name + ' IsCircle: ' + str(arc.IsCircle))
    print(name + ' Radius: ' + str(arc.Radius))
    print(name + ' Diameter: ' + str(arc.Diameter))
    print(name + ' Center: ' + str(arc.Center))
    print(name + ' Circumference: ' + str(arc.Circumference))
    print(name + ' Length: ' + str(arc.Length))
    print(name + ' StartPoint: ' + str(arc.StartPoint))
    print(name + ' MidPoint: ' + str(arc.MidPoint))
    print(name + ' EndPoint: ' + str(arc.EndPoint))
    print(name + ' AngleDomain: ' + str(arc.AngleDomain))
    print(name + ' StartAngle: ' + str(arc.StartAngle))
    print(name + ' EndAngle: ' + str(arc.EndAngle))
    print(name + ' AngleRadians: ' + str(arc.AngleRadians))
    print(name + ' StartAngleDegrees: ' + str(arc.StartAngleDegrees))
    print(name + ' EndAngleDegrees: ' + str(arc.EndAngleDegrees))
    print(name + ' AngleDegrees: ' + str(arc.AngleDegrees))

    print(name + ' BoundingBox: ' + str(arc.BoundingBox()))
    print(name + ' PointAt: ' + str(arc.PointAt(0.1)))
    print(name + ' TangentAt: ' + str(arc.TangentAt(0.1)))
    print(name + ' ClosestParameter: ' + str(arc.ClosestParameter(rhino3dm.Point3d(0,0,0))))
    print(name + ' ClosestPoint: ' + str(arc.ClosestPoint(rhino3dm.Point3d(0,0,0))))
    print(name + ' Reverse: ' + str(arc.Reverse()))

    print()

circle = rhino3dm.Circle(10)

arc1 = rhino3dm.Arc(circle, math.pi / 2)
arc2 = rhino3dm.Arc(rhino3dm.Point3d(0,0,0), 10, math.pi/2)
arc3 = rhino3dm.Arc(rhino3dm.Point3d(0,0,0), rhino3dm.Point3d(1,1,1), rhino3dm.Point3d(1,2,3))
arc4 = rhino3dm.Arc(rhino3dm.Point3d(0,0,0), rhino3dm.Vector3d(1,0,0), rhino3dm.Point3d(0,10,0))

info(arc1, 'Arc 1')
info(arc2, 'Arc 2')
info(arc3, 'Arc 3')
info(arc4, 'Arc 4')