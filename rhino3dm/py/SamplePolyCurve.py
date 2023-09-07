# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2

import rhino3dm
import math

polyCurve = rhino3dm.PolyCurve()

print('Curve Type: ' + str(type(polyCurve)))

circle = rhino3dm.Circle( 10 )
xform = rhino3dm.Transform.Translation(20, 0, 20)
circle.Transform(xform)
arc = rhino3dm.Arc( circle, math.pi )
p1 = arc.StartPoint
print('P1 ' + str(p1))
p2 = rhino3dm.Point3d(p1.X + 10, p1.Y, p1.Z + 10)
line = rhino3dm.Line( p1, p2 )

r1 = polyCurve.Append(arc)
r2 = polyCurve.Append(line)

print('Append Arc ok? ' + str(r1))
print('Append Line ok? ' + str(r2))

res = polyCurve.Explode()
print('Res Count: ' + str(len(res)))

