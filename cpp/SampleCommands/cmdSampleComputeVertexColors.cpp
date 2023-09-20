
#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleComputeVertexColors command
//

#pragma region SampleComputeVertexColors command

class CCommandSampleComputeVertexColors : public CRhinoCommand
{
public:
  CCommandSampleComputeVertexColors() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x466BAD98, 0xCAB8, 0x4DA1, { 0x85, 0xE0, 0x96, 0xEF, 0x0A, 0xE6, 0x1D, 0xFB } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleComputeVertexColors"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  ON_Mesh* ComputeVertexColors(CRhinoDoc& doc, const CRhinoMeshObject* mesh_obj);
};

// The one and only CCommandSampleComputeVertexColors object
static class CCommandSampleComputeVertexColors theSampleComputeVertexColorsCommand;

CRhinoCommand::result CCommandSampleComputeVertexColors::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select a mesh with a texture map assigned");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const auto& obj_ref = go.Object(0);
  const auto* mesh_obj = CRhinoMeshObject::Cast(obj_ref.Object());
  if (nullptr == mesh_obj)
    return CRhinoCommand::failure;

  auto* mesh_with_colors = ComputeVertexColors(*doc, mesh_obj);
  if (nullptr == mesh_with_colors)
    return CRhinoCommand::failure;

  auto* mesh_with_colors_obj = new CRhinoMeshObject(mesh_obj->Attributes());
  mesh_with_colors_obj->SetMesh(mesh_with_colors);
  doc->ReplaceObject(obj_ref, mesh_with_colors_obj);

  doc->Redraw();

  return CRhinoCommand::success;
}

static int Round(double value)
{
  const double d = fabs(value);
  int n = (int)d;
  if (0.5 <= d - n)
    n = (int)ceil(d);
  return (0 > value) ? -n : n;
}

ON_Mesh* CCommandSampleComputeVertexColors::ComputeVertexColors(CRhinoDoc& doc, const CRhinoMeshObject* mesh_obj)
{
  if (nullptr == mesh_obj)
    return nullptr;

  const auto* mesh = mesh_obj->Mesh();
  if ((nullptr == mesh) || !mesh->HasTextureCoordinates())
    return nullptr;

  // Find the render material assigned to the mesh.
  ON_COMPONENT_INDEX ci; // Not needed to get the whole object material.
  const auto* pMaterial = mesh_obj->ObjectRdkMaterial(ci);
  if (nullptr == pMaterial)
  {
    RhinoApp().Print(L"The mesh does not have a render material\n");
    return nullptr;
  }

  // Get the texture in the bitmap slot.
  const auto* pTexture = pMaterial->FindChild(CS_MAT_BITMAP_TEXTURE);
  auto* pFileBased = dynamic_cast<const IRhRdkFileBasedContent*>(pTexture);
  if (nullptr == pFileBased)
  {
    RhinoApp().Print(L"The mesh's render material does not have a file-based texture in the bitmap slot\n");
    return nullptr;
  }

  // Get the texture's filename.
  auto filename = pFileBased->Filename();
  if (!CRhinoFileUtilities::FileExists(filename))
  {
    ON_wString found;
    if (::RhRdkFindFile(doc.RuntimeSerialNumber(), filename, found))
      filename = found;
  }

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"The mesh's bitmap texture file was not found\n");
    return nullptr;
  }

  // Read the texture file into a dib.
  CRhinoDib dib;
  if (!dib.ReadFromFile(doc.RuntimeSerialNumber(), filename))
  {
    RhinoApp().Print(L"The mesh's bitmap texture file could not be loaded\n");
    return nullptr;
  }

  // Turn the bitmap slot off so the user can see the vertex colors in the viewport.
  CRhRdkContentUndo cu(doc);
  cu.ModifyContent(*pMaterial);
  auto& m = pMaterial->BeginChange(RhRdkChangeContext::Program);
  m.SetChildSlotOn(CS_MAT_BITMAP_TEXTURE, false);
  m.EndChange();

  // Make a new mesh to set vertex colors on.
  auto* mesh_with_colors = new ON_Mesh(*mesh);

  // Calculate the vertex colors for the new mesh and set them.
  int size_x = dib.Width() - 1;
  int size_y = dib.Height() - 1;

  const int tc_count = mesh_with_colors->m_T.Count();
  mesh_with_colors->m_C.Reserve(tc_count);
  mesh_with_colors->m_C.SetCount(tc_count);

  float f = 0, n = 0;
  int r = 0, g = 0, b = 0, a = 0;

  for (int tc = 0; tc < tc_count; tc++)
  {
    f = modff(mesh_with_colors->m_T[tc].x, &n);
    if (f < 0)
      f = 1 + f;
    float tex_x = f * size_x;

    f = modff(mesh_with_colors->m_T[tc].y, &n);
    if (f < 0)
      f = 1 + f;
    float tex_y = f * size_y;

    dib.GetPixel(Round(tex_x), Round(tex_y), r, g, b, a);
    mesh_with_colors->m_C[tc].SetRGBA(r, g, b, a);
  }

  return mesh_with_colors;
}

#pragma endregion

//
// END SampleComputeVertexColors command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
