# requires rhino3dm 9.2.0b0
# python3 -m pip install rhino3dm==9.2.0b0

# Computes the texture coordinates a mesh object gets from its material's
# texture mappings, caches them on the mesh, and reads them back per texture.

import sys
import rhino3dm

path = sys.argv[1] if len(sys.argv) > 1 else '../models/planarMappedMesh.3dm'
model = rhino3dm.File3dm.Read(path)

for obj in model.Objects:
    mesh = obj.Geometry
    if not isinstance(mesh, rhino3dm.Mesh):
        continue

    # The object's material comes from the object itself or from its layer
    attributes = obj.Attributes
    if attributes.MaterialSource == rhino3dm.ObjectMaterialSource.MaterialFromObject:
        material_index = attributes.MaterialIndex
    else:
        material_index = model.Layers[attributes.LayerIndex].RenderMaterialIndex
    if material_index < 0:
        continue
    material = model.Materials[material_index]

    texture = material.GetBitmapTexture()
    if texture is None:
        continue

    # Evaluate the object's texture mappings for every texture in the material
    # and cache the resulting texture coordinates on the mesh
    mesh.SetCachedTextureCoordinatesFromMaterial(model, attributes.ObjectId, material)

    # Read back the texture coordinates the bitmap texture uses
    tcs = mesh.GetCachedTextureCoordinates(model, attributes.ObjectId, texture)
    if tcs is None:
        continue

    print('Object: ' + str(attributes.ObjectId))
    print('Texture: ' + texture.FileName)
    print('Mapping Id: ' + str(tcs.MappingId))
    print('Texture coordinates: ' + str(tcs.Count) + ', dimension ' + str(tcs.Dim))
    for i in range(min(tcs.Count, 5)):
        ok, u, v, w = tcs.TryGetAt(i)
        if tcs.Dim == 2:
            print('  ' + str(i) + ': ' + str(u) + ', ' + str(v))
        else:
            print('  ' + str(i) + ': ' + str(u) + ', ' + str(v) + ', ' + str(w))
    print()
