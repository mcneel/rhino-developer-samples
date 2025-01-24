import rhino3dm

model = rhino3dm.File3dm()

#create geometry
sphere1 = rhino3dm.Sphere( rhino3dm.Point3d( 0,0,0 ), 10)
sphere2 = rhino3dm.Sphere( rhino3dm.Point3d( 10,10,10 ), 4)
geometry = (sphere1.ToBrep(), sphere2.ToBrep())

#create attributes
attr1 = rhino3dm.ObjectAttributes()
attr1.Name = "Sphere 1"
attr2 = rhino3dm.ObjectAttributes()
attr2.Name = "Sphere 2"
attributes = (attr1, attr2)
basepoint = rhino3dm.Point3d( 0,0,0 )

#create idef
index = model.InstanceDefinitions.Add("name", "description", "url", "urltag", basepoint, geometry, attributes)
print("Index of new idef: " + str(index))

#create iref
idef = model.InstanceDefinitions.FindIndex(index)
xf = rhino3dm.Transform(10.00)
iref = rhino3dm.InstanceReference(idef.Id, xf)
uuid = model.Objects.Add(iref, None)
print("id of new iref: " + str(uuid))

#save file
model.Write('../models/InstanceDefinition.3dm',8)