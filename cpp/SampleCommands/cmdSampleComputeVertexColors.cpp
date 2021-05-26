
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
  auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh with texture map assigned");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const auto& obj_ref = go.Object(0);
  const auto* mesh_obj = CRhinoMeshObject::Cast(obj_ref.Object());
  if (nullptr == mesh_obj)
    return CRhinoCommand::failure;

  auto* mesh_with_colors = ComputeVertexColors(*pDoc, mesh_obj);
  if (nullptr == mesh_with_colors)
    return CRhinoCommand::failure;

  auto* mesh_with_colors_obj = new CRhinoMeshObject(mesh_obj->Attributes());
  mesh_with_colors_obj->SetMesh(mesh_with_colors);
  pDoc->ReplaceObject(obj_ref, mesh_with_colors_obj);

  pDoc->Redraw();

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
  if (nullptr == mesh || false == mesh->HasTextureCoordinates())
    return nullptr;

  const ON_Material& material = doc.m_material_table[mesh_obj->Attributes().m_material_index];
  const int texture_count = material.m_textures.Count();
  if (0 == texture_count)
    return nullptr;

  ON_wString filename;
  for (int ti = 0; ti < texture_count; ti++)
  {
    filename = material.m_textures[ti].m_image_file_reference.FullPath();
    if (!filename.IsEmpty())
      break;
  }

  if (filename.IsEmpty())
    return nullptr;

  CRhinoDib dib;
  if (!dib.ReadFromFile(doc.RuntimeSerialNumber(), filename))
    return nullptr;

  int size_x = dib.Width() - 1;
  int size_y = dib.Height() - 1;

  auto* mesh_with_colors = new ON_Mesh(*mesh);

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
