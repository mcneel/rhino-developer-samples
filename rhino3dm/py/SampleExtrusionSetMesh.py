# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2

import rhino3dm
model = rhino3dm.File3dm.Read('../models/MeshBox_r5.3dm')

mesh = model.Objects[0].Geometry

bbox = rhino3dm.BoundingBox(-10,-10,0, 10,10,20)
box = rhino3dm.Box(bbox)

extrusion = rhino3dm.Extrusion.CreateBoxExtrusion(box, False) #this extrusion is fine
extrusion.SetMesh(mesh, rhino3dm.MeshType.Render) 

outModel = rhino3dm.File3dm()
outModel.Objects.AddExtrusion(extrusion, None)
outModel.Write('../models/ExtrusionBox_r8.3dm')