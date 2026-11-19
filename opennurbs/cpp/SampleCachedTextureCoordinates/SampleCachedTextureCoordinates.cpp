////////////////////////////////////////////////////////////////
//
// SampleCachedTextureCoordinates.cpp
//
// Standalone openNURBS console application. Reads a 3dm file and,
// for every mesh object, computes the texture coordinates the
// object's texture mappings give each texture of its material,
// caches them on the mesh and prints them.
//
// Usage: SampleCachedTextureCoordinates <file.3dm>
//
////////////////////////////////////////////////////////////////

#include "opennurbs.h"

static void PrintTextureCoordinates(ON_TextLog& log, const ON_TextureCoordinates& tcs)
{
  log.Print("Mapping Id: ");
  log.Print(tcs.m_tag.m_mapping_id);
  log.Print("\n");
  log.Print("Texture coordinates: %d, dimension %d\n", tcs.m_T.Count(), tcs.m_dim);

  const int count = tcs.m_T.Count() < 5 ? tcs.m_T.Count() : 5;
  for (int i = 0; i < count; i++)
  {
    const ON_3fPoint& t = tcs.m_T[i];
    if (2 == tcs.m_dim)
      log.Print("  %d: %g, %g\n", i, t.x, t.y);
    else
      log.Print("  %d: %g, %g, %g\n", i, t.x, t.y, t.z);
  }
}

int main(int argc, const char* argv[])
{
  if (argc < 2)
  {
    printf("Usage: SampleCachedTextureCoordinates <file.3dm>\n");
    return 1;
  }

  ON::Begin();

  ON_TextLog log(stdout);

  ONX_Model model;
  if (!model.Read(argv[1], &log))
  {
    log.Print("Error reading %s\n", argv[1]);
    ON::End();
    return 1;
  }

  ONX_ModelComponentIterator it(model, ON_ModelComponent::Type::ModelGeometry);
  for (const ON_ModelComponent* component = it.FirstComponent(); nullptr != component; component = it.NextComponent())
  {
    const ON_ModelGeometryComponent* model_geometry = ON_ModelGeometryComponent::Cast(component);
    if (nullptr == model_geometry)
      continue;

    const ON_Mesh* mesh = ON_Mesh::Cast(model_geometry->Geometry(nullptr));
    const ON_3dmObjectAttributes* attributes = model_geometry->Attributes(nullptr);
    if (nullptr == mesh || nullptr == attributes)
      continue;

    // The object's material, either from the object itself or from its layer
    const ON_ModelComponentReference& material_ref = model.MaterialFromAttributes(*attributes);
    const ON_Material* material = ON_Material::FromModelComponentRef(material_ref, nullptr);
    if (nullptr == material || material->m_textures.Count() < 1)
      continue;

    // The object's texture mappings for the material's render engine
    const ON_MappingRef* mapping_ref = attributes->m_rendering_attributes.MappingRef(material->MaterialPlugInId());

    // Evaluate the object's texture mappings for every texture in the material
    // and cache the resulting texture coordinates on the mesh
    mesh->SetCachedTextureCoordinatesFromMaterial(model, *material, mapping_ref);

    log.Print("Object: ");
    log.Print(attributes->m_uuid);
    log.Print("\n");

    // Read back the texture coordinates each texture uses
    for (int i = 0; i < material->m_textures.Count(); i++)
    {
      const ON_Texture& texture = material->m_textures[i];
      log.Print("Texture: %ls\n", static_cast<const wchar_t*>(texture.m_image_file_reference.FullPath()));

      const ON_TextureCoordinates* tcs = mesh->GetCachedTextureCoordinates(model, texture, mapping_ref);
      if (nullptr != tcs)
        PrintTextureCoordinates(log, *tcs);
      else
        log.Print("No texture coordinates\n");
    }
    log.Print("\n");
  }

  ON::End();

  return 0;
}
