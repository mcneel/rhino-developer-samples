# requires rhino3dm.py ^8.6.0b0
import rhino3dm
model = rhino3dm.File3dm.Read('../models/textEntities_r8.3dm')
objects = model.Objects
for obj in objects:
    geo = obj.Geometry
    print(geo.ObjectType)
    print(geo.PlainText)
    print(geo.RichText)
    print(geo.PlainTextWithFields)
