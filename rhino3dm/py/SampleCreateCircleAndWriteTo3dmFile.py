import rhino3dm
import random

model = rhino3dm.File3dm()
for i in range(20):
	pt = rhino3dm.Point3d(random.uniform(-10,10), random.uniform(-10,10), 0)
	model.Objects.AddPoint(pt)
	circle = rhino3dm.Circle(pt, random.uniform(1,4))
	model.Objects.AddCircle(circle)
	
model.Write("circle.3dm")
