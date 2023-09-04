# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2

import rhino3dm
model = rhino3dm.File3dm.Read('../models/MeshSphere_r5.3dm')

mesh = model.Objects[0].Geometry

sphere = rhino3dm.Sphere( rhino3dm.Point3d( 0,0,0 ), 10)
brep = sphere.ToBrep()
brepFace = brep.Faces[0]
brepFace.SetMesh(mesh, rhino3dm.MeshType.Render)

outModel = rhino3dm.File3dm()
outModel.Objects.AddBrep(brep, None)

outModel.Write('../models/BrepSphere.3dm',8)


