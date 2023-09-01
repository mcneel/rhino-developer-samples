# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2
import rhino3dm

pc = rhino3dm.PointCloud()

pc.Add(rhino3dm.Point3d(0,0,0))
pc.Add(rhino3dm.Point3d(0,0,0), rhino3dm.Vector3d(0,0,1))
pc.Add(rhino3dm.Point3d(0,0,0), (255,0,0,0) )
pc.Add(rhino3dm.Point3d(0,0,0), rhino3dm.Vector3d(0,1,1), (255,0,0,0))
pc.Add(rhino3dm.Point3d(0,0,0), 1.234)
pc.Add(rhino3dm.Point3d(0,0,0), rhino3dm.Vector3d(0,1,1), (255,0,0,0), 1.234)

points = [rhino3dm.Point3d(1,0,0), rhino3dm.Point3d(0,1,0), rhino3dm.Point3d(0,1,0)]
normals = [rhino3dm.Vector3d(1,0,0), rhino3dm.Vector3d(0,1,0), rhino3dm.Vector3d(0,1,0)]
colors = [(255,0,0,255),(0,255,0,0),(0,0,255,0)]
values = [2.34, 3.45, 5.67]

pc.AddRange(points)
pc.AddRange(points, normals)
pc.AddRange(points, colors)
pc.AddRange(points, normals, colors)
pc.AddRange(points, values)
pc.AddRange(points, normals, colors, values)

pc.Insert(1, rhino3dm.Point3d(10,10,10))
pc.Insert(1, rhino3dm.Point3d(10,10,10), rhino3dm.Vector3d(10,10,10))
pc.Insert(1, rhino3dm.Point3d(10,10,10), (255,255,255,0))
pc.Insert(1, rhino3dm.Point3d(10,10,10), rhino3dm.Vector3d(10,10,10), (255,255,255,0))
pc.Insert(1, rhino3dm.Point3d(10,10,10), 34.567)
pc.Insert(1, rhino3dm.Point3d(10,10,10), rhino3dm.Vector3d(10,10,10), (255,255,255,0), 5.678)


print('Point Cloud Length ' + str(len(pc)))
print('Point Cloud Has Normals ' + str(pc.ContainsNormals))
print('Point Cloud Has Colors ' + str(pc.ContainsColors))
print('Point Cloud Has Point Values ' + str(pc.ContainsValues))
print()

pts = pc.GetPoints()
nrmls = pc.GetNormals()
cols = pc.GetColors()
vals = pc.GetValues()

print('Pts len: ' + str(len(pts)))

for pt in pts:
    print(pt)
print()

print('nrmls len: ' + str(len(nrmls)))

for n in nrmls:
    print(n)
print()

print('cols len: ' + str(len(cols)))

for c in cols:
    print(c)
print()

print('Vals len: ' + str(len(vals)))
for v in vals:
    print(v)