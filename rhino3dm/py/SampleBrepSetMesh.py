# requires rhino3dm 9.2.0b0
# python3 -m pip install rhino3dm==9.2.0b0

import rhino3dm
model = rhino3dm.File3dm.Read('../models/MeshSphere_r5.3dm')

mesh = list(model.Objects)[0].Geometry

sphere = rhino3dm.Sphere( rhino3dm.Point3d( 0,0,0 ), 10)
brep = sphere.ToBrep()
brepFace = brep.Faces[0]
brepFace.SetMesh(rhino3dm.MeshType.Render, mesh)

outModel = rhino3dm.File3dm()
outModel.Objects.AddBrep(brep, None)

outModel.Write('../models/BrepSphere.3dm',8)


