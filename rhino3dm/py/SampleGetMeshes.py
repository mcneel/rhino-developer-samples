# Sample read render meshes from 3dm file
import rhino3dm
model = rhino3dm.File3dm.Read('mymodel.3dm')
brep = model.Objects[0].Geometry
face = brep.Faces[0]
mesh = face.GetMesh(rhino3dm.MeshType.Any)
print (len(mesh.Faces))
