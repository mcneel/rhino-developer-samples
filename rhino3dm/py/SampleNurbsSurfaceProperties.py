# requires rhino3dm 9.2.0b0
# python3 -m pip install rhino3dm==9.2.0b0

import rhino3dm
model = rhino3dm.File3dm.Read('../models/nurbsSurface_r5.3dm')
brep = list(model.Objects)[0].Geometry

print('Brep Vertex Count: ' + str(len(brep.Vertices)))
print('Brep Vertex ' + str(brep.Vertices[3].Location) )
print('Brep Vertex Index: ' + str(brep.Vertices[3].VertexIndex) )
print('Brep Edge Count: ' +  str(len(brep.Vertices[3].EdgeIndices())) )
print('Brep Edge Indices: ' +  str(brep.Vertices[3].EdgeIndices()) )

nurbssurface = brep.Surfaces[0].ToNurbsSurface()

print ('Nurbs Surface Order U: ' + str(nurbssurface.OrderU))
print ('Nurbs Surface Order V: ' + str(nurbssurface.OrderV))
print ('Nurbs Surface Control Point Count: ' + str(nurbssurface.Points.CountU * nurbssurface.Points.CountV))
print (nurbssurface.Points.GetPoint4d(0,0)[1])
print (nurbssurface.Points.GetControlPoint(0,0))
print (nurbssurface.KnotsU[3])

print()
knotsU = nurbssurface.KnotsU
knotUList = list(knotsU)
print('Knots U List')
print(knotUList)

knotsV = nurbssurface.KnotsV
knotVList = list(knotsV)
print('Knots V List')
print(knotVList)