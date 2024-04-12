import rhino3dm

model = rhino3dm.File3dm()

ef = rhino3dm.EmbeddedFile.Read2('./rhinologo.png')
print('FileName: ' + ef.Filename)
ef.Filename = 'rhinologo.png'
print('FileName: ' + ef.Filename)
model.EmbeddedFiles.Add(ef)

print('How many embedded files: ' + str(len(model.EmbeddedFiles)))

material = rhino3dm.Material()
material.SetBitmapTexture('rhinologo.png')
index = model.Materials.Add(material)

oa = rhino3dm.ObjectAttributes()
oa.MaterialSource = rhino3dm.ObjectMaterialSource.MaterialFromObject
oa.MaterialIndex = index

sphere = rhino3dm.Sphere(rhino3dm.Point3d(0,0,0), 10)
brep = rhino3dm.Brep.CreateFromSphere(sphere)
model.Objects.Add(brep, oa)

model.Write("material.3dm", 0)