# requires rhino3dm 8.0.0b2
# python3 -m pip install rhino3dm==8.0.0b2

import rhino3dm
model = rhino3dm.File3dm.Read('../models/decal_r8.3dm')
 
for obj in model.Objects:
    print('Nº of decals: ' + str(len(obj.Attributes.Decals)))
    for decal in obj.Attributes.Decals:
        print('Decal Texture Instance Id: ' + str(decal.TextureInstanceId))

        rc = model.RenderContent.FindId(decal.TextureInstanceId)

        print('Decal Texture FileName: ' + str(rc.FileName))

        print('Decal Mapping: ' + str(decal.Mapping))
        print('Decal Projection: ' + str(decal.Projection))
        print('Decal MapToInside: ' + str(decal.MapToInside))
        print('Decal Transparency: ' + str(decal.Transparency))
        print('Decal Origin: ' + str(decal.Origin))
        print('Decal VectorUp: ' + str(decal.VectorUp))
        print('Decal VectorAcross: ' + str(decal.VectorAcross))
        print('Decal Height: ' + str(decal.Height))
        print('Decal Radius: ' + str(decal.Radius))
        print('Decal HorzSweepStart: ' + str(decal.HorzSweepStart))
        print('Decal HorzSweepEnd: ' + str(decal.HorzSweepEnd))
        print('Decal VertSweepStart: ' + str(decal.VertSweepStart))
        print('Decal VertSweepEnd: ' + str(decal.VertSweepEnd))
        print('Decal BoundsMinU: ' + str(decal.BoundsMinU))
        print('Decal BoundsMinV: ' + str(decal.BoundsMinV))
        print('Decal BoundsMaxU: ' + str(decal.BoundsMaxU))
        print('Decal BoundsMaxV: ' + str(decal.BoundsMaxV))
        print()

        decal.Mapping = rhino3dm.Mappings.Spherical
        decal.Projection = rhino3dm.Projections.Backward
        decal.MapToInside = True
        decal.Transparency = 0.5
        decal.Origin = rhino3dm.Point3d(1,0,1)
        decal.VectorUp = rhino3dm.Vector3d(10,0,33)
        decal.VectorAcross = rhino3dm.Vector3d(11,11,11)
        decal.Height = 2.0
        decal.Radius = 11.11
        decal.HorzSweepStart = 0.2
        decal.HorzSweepEnd = 1.55
        decal.VertSweepStart = 0.2
        decal.VertSweepEnd = 2.11
        decal.BoundsMinU = 0.2
        decal.BoundsMinV = 1.2
        decal.BoundsMaxU = 0.3
        decal.BoundsMaxV = 1.44
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
        print('Decal HorzSweepStart: ' + str(decal.HorzSweepStart))
        print('Decal HorzSweepEnd: ' + str(decal.HorzSweepEnd))
        print('Decal VertSweepStart: ' + str(decal.VertSweepStart))
        print('Decal VertSweepEnd: ' + str(decal.VertSweepEnd))
        print('Decal BoundsMinU: ' + str(decal.BoundsMinU))
        print('Decal BoundsMinV: ' + str(decal.BoundsMinV))
        print('Decal BoundsMaxU: ' + str(decal.BoundsMaxU))
        print('Decal BoundsMaxV: ' + str(decal.BoundsMaxV))
        print()

