# SampleCachedTextureCoordinates

Standalone openNURBS console application that computes the texture coordinates a mesh object gets from its material's texture mappings, caches them on the mesh with `ON_Mesh::SetCachedTextureCoordinatesFromMaterial`, and reads them back per texture with `ON_Mesh::GetCachedTextureCoordinates`.

## Build

Clone [openNURBS](https://github.com/mcneel/opennurbs), then:

```
cmake -S . -B build -DOPENNURBS_DIR=<path to opennurbs>
cmake --build build --config Release
```

## Run

```
SampleCachedTextureCoordinates ../../../rhino3dm/models/planarMappedMesh.3dm
```

`planarMappedMesh.3dm` has a mesh with a planar texture mapping on channel 1 and a material whose bitmap texture uses that channel.
