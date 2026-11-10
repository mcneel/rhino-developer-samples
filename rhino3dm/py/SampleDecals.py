# requires rhino3dm 9.2.0b0
# python3 -m pip install rhino3dm==9.2.0b0

import rhino3dm
model = rhino3dm.File3dm.Read('../models/decal_r8.3dm')
 
for obj in model.Objects:
    print('Nº of decals: ' + str(len(obj.Attributes.Decals)))
    for decal in obj.Attributes.Decals:
        print('Decal Texture Instance Id: ' + str(decal.TextureInstanceId))

        rc = model.RenderTextures.Find(decal.TextureInstanceId)

        print('Decal Texture FileName: ' + str(rc.Filename))

        print('Decal Mapping: ' + str(decal.Mapping))
        print('Decal Projection: ' + str(decal.Projection))
        print('Decal MapToInside: ' + str(decal.MapToInside))
        print('Decal Transparency: ' + str(decal.Transparency))
        print('Decal Origin: ' + str(decal.Origin))
        print('Decal VectorUp: ' + str(decal.VectorUp))
        print('Decal VectorAcross: ' + str(decal.VectorAcross))
        print('Decal Height: ' + str(decal.Height))
        print('Decal Radius: ' + str(decal.Radius))
        horzSweepStart, horzSweepEnd = decal.HorzSweep()
        vertSweepStart, vertSweepEnd = decal.VertSweep()
        boundsMinU, boundsMinV, boundsMaxU, boundsMaxV = decal.GetUVBounds()
        print('Decal HorzSweepStart: ' + str(horzSweepStart))
        print('Decal HorzSweepEnd: ' + str(horzSweepEnd))
        print('Decal VertSweepStart: ' + str(vertSweepStart))
        print('Decal VertSweepEnd: ' + str(vertSweepEnd))
        print('Decal BoundsMinU: ' + str(boundsMinU))
        print('Decal BoundsMinV: ' + str(boundsMinV))
        print('Decal BoundsMaxU: ' + str(boundsMaxU))
        print('Decal BoundsMaxV: ' + str(boundsMaxV))
        print()

        decal.Mapping = rhino3dm.DecalMapping.Spherical
        decal.Projection = rhino3dm.DecalProjection.Backward
        decal.MapToInside = True
        decal.Transparency = 0.5
        decal.Origin = rhino3dm.Point3d(1,0,1)
        decal.VectorUp = rhino3dm.Vector3d(10,0,33)
        decal.VectorAcross = rhino3dm.Vector3d(11,11,11)
        decal.Height = 2.0
        decal.Radius = 11.11
        decal.SetHorzSweep(0.2, 1.55)
        decal.SetVertSweep(0.2, 2.11)
        decal.SetUVBounds(0.2, 1.2, 0.3, 1.44)
        print()

        print('Decal Mapping: ' + str(decal.Mapping))
        print('Decal Projection: ' + str(decal.Projection))
        print('Decal MapToInside: ' + str(decal.MapToInside))
        print('Decal Transparency: ' + str(decal.Transparency))
        print('Decal Origin: ' + str(decal.Origin))
        print('Decal VectorUp: ' + str(decal.VectorUp))
        print('Decal VectorAcross: ' + str(decal.VectorAcross))
        print('Decal Height: ' + str(decal.Height))
        print('Decal Radius: ' + str(decal.Radius))
        horzSweepStart, horzSweepEnd = decal.HorzSweep()
        vertSweepStart, vertSweepEnd = decal.VertSweep()
        boundsMinU, boundsMinV, boundsMaxU, boundsMaxV = decal.GetUVBounds()
        print('Decal HorzSweepStart: ' + str(horzSweepStart))
        print('Decal HorzSweepEnd: ' + str(horzSweepEnd))
        print('Decal VertSweepStart: ' + str(vertSweepStart))
        print('Decal VertSweepEnd: ' + str(vertSweepEnd))
        print('Decal BoundsMinU: ' + str(boundsMinU))
        print('Decal BoundsMinV: ' + str(boundsMinV))
        print('Decal BoundsMaxU: ' + str(boundsMaxU))
        print('Decal BoundsMaxV: ' + str(boundsMaxV))
        print()

