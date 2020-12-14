from rhino3dm import *
import compute_rhino3d.Curve
import compute_rhino3d.Util

compute_rhino3d.Util.url = "http://localhost:8081/"
#compute_rhino3d.Util.apiKey = ""

model = File3dm.Read("crv.3dm")
crv = model.Objects[0].Geometry
print(crv)

points = Point3dList(5)
points.Add(0, 0, 0)
points.Add(0, 2, 0)
points.Add(2, 3, 0)
points.Add(4, 2, 0)
points.Add(4, 0, 0)

nc = NurbsCurve.Create(False, 3, points)

print(nc)

result = compute_rhino3d.Curve.Fit(nc, 2, 1.0,1.0)
print(result)