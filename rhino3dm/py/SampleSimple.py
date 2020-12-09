# Simple example
import rhino3dm
center = rhino3dm.Point3d(1,2,3)
arc = rhino3dm.Arc(center, 10, 1)
nc = arc.ToNurbsCurve()
start = nc.PointAtStart
print(start)
