# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2

import rhino3dm
model = rhino3dm.File3dm.Read('../models/nurbsSurface_r5.3dm')
brep = model.Objects[0].Geometry

print('Brep Vertex Count: ' + str(len(brep.Vertices)))
print('Brep Vertex ' + str(brep.Vertices[3].Location) )
print('Brep Vertex Index: ' + str(brep.Vertices[3].VertexIndex) )
print('Brep Edge Count: ' +  str((brep.Vertices[3].EdgeCount)) )
print('Brep Edge Indices: ' +  str(brep.Vertices[3].EdgeIndices()) )

nurbssurface = brep.Surfaces[0].ToNurbsSurface()

print ('Nurbs Surface Order U: ' + str(nurbssurface.OrderU))
print ('Nurbs Surface Order V: ' + str(nurbssurface.OrderV))
print ('Nurbs Surface Control Point Count: ' + str(len(nurbssurface.Points)))
print (nurbssurface.Points[0,0])
print (nurbssurface.Points.GetControlPoint(0,0))
print (nurbssurface.KnotsU[3])

print()
knotsU = nurbssurface.KnotsU
knotUList = knotsU.ToList()
print('Knots U List')
print(knotUList)

knotsV = nurbssurface.KnotsV
knotVList = knotsV.ToList()
print('Knots V List')
print(knotVList)