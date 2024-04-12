#requires rhino3dm.py >= 8.6.0
import rhino3dm

model = rhino3dm.File3dm()
model.ApplicationName = 'python'
model.ApplicationDetails = 'rhino-developer-samples'
model.ApplicationUrl = 'https://rhino3d.com'

# create objects to be in instance definition
circle = rhino3dm.Circle(10)

bbox = rhino3dm.BoundingBox(-10,-10,0, 10,10,20)
box = rhino3dm.Box(bbox)
extrusion = rhino3dm.Extrusion.CreateBoxExtrusion(box, False) #this extrusion is fine

objects = [circle.ToNurbsCurve(), extrusion] #objects need to derive from GeometryBase, hence circle.ToNurbsCurve()

#create the instance definition
oa = rhino3dm.ObjectAttributes()
attributes = [oa, oa]
p1 = rhino3dm.Point3d(0,0,0)
#index = model.InstanceDefinitions.AddInstanceDefinition('test', 'an idef', 'https://www.rhino3d.com', 'tag', p1, objects, attributes)

index = model.InstanceDefinitions.AddInstanceDefinition2( 'test', 'an idef', 'https://www.rhino3d.com', 'tag', p1,(circle.ToNurbsCurve(), extrusion), (oa, oa) )

#add a reference to the Instance Definition to the model
idef = model.InstanceDefinitions.FindIndex(index)
xform = rhino3dm.Transform(100)
iref = rhino3dm.InstanceReference(idef.Id, xform)
model.Objects.AddInstanceObject(iref)

model.Write("idef_py.3dm", 0)
