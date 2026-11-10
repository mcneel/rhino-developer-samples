import rhino3dm

model = rhino3dm.File3dm()

ef = rhino3dm.File3dmEmbeddedFile.Read('./rhinologo.png')
print('FileName: ' + ef.Filename)
# an embedded file's Filename is read-only, and EmbeddedFiles.Add takes a path,
# so embed the file under the name the material uses
model.EmbeddedFiles.Add('rhinologo.png')
ef = list(model.EmbeddedFiles)[0]
print('FileName: ' + ef.Filename)

print('How many embedded files: ' + str(len(model.EmbeddedFiles)))

material = rhino3dm.Material()
material.SetBitmapTexture('rhinologo.png')
model.Materials.Add(material)
index = len(model.Materials) - 1

oa = rhino3dm.ObjectAttributes()
oa.MaterialSource = rhino3dm.ObjectMaterialSource.MaterialFromObject
oa.MaterialIndex = index

sphere = rhino3dm.Sphere(rhino3dm.Point3d(0,0,0), 10)
brep = rhino3dm.Brep.CreateFromSphere(sphere)
model.Objects.Add(brep, oa)

model.Write("material.3dm", 0)