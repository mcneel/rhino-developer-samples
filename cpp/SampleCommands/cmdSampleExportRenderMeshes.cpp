
#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExportRenderMeshes command
//

#pragma region SampleExportRenderMeshes command

class CCommandSampleExportRenderMeshes : public CRhinoCommand
{
public:
  CCommandSampleExportRenderMeshes() = default;
  ~CCommandSampleExportRenderMeshes() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x5DDA7654, 0x1B65, 0x4664, { 0xBF, 0xAB, 0xA0, 0xE3, 0xBD, 0x03, 0xF6, 0x2D } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleExportRenderMeshes"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;

private:
  bool DoExport(CRhinoDoc& doc, const CRhinoObjRef& obj_ref, const ON_Mesh& mesh, const ON_Material& material);
};

// The one and only CCommandSampleExportRenderMeshes object
static class CCommandSampleExportRenderMeshes theSampleExportRenderMeshesCommand;

CRhinoCommand::result CCommandSampleExportRenderMeshes::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_Viewport vp;
  if (doc->ActiveView())
  {
    vp = doc->ActiveView()->ActiveViewport().View().m_vp;
  }

  int count = 0;

  CRhinoObjectIterator it(*doc, CRhinoObjectIterator::normal_or_locked_objects, CRhinoObjectIterator::active_and_reference_objects);
  for (const CRhinoObject* obj = obj = it.First(); nullptr != obj; obj = it.Next())
  {
    const CRhinoObjRef objref(obj);

    const auto render_meshes_for_object = obj->RenderMeshes(ON::render_mesh, true);
    if (render_meshes_for_object)
    {
      for (const auto& mesh_instance : *render_meshes_for_object)
      {
        if (mesh_instance)
        {
          const auto spMesh = mesh_instance->Geometry().Mesh();
          const auto spMaterial = mesh_instance->Material();

          if (spMesh && spMaterial)
          {
            if (DoExport(*doc, objref, *spMesh, spMaterial->ToOnMaterial()))
            {
              count++;
            }
          }
        }
      }
    }
  }

  if (0==count)
    return nothing;

  return success;
}

bool CCommandSampleExportRenderMeshes::DoExport(CRhinoDoc& doc, const CRhinoObjRef& obj_ref, const ON_Mesh& mesh, const ON_Material& material)
{
  // TODO: Add export code here.

  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(obj_ref);
  UNREFERENCED_PARAMETER(mesh);
  UNREFERENCED_PARAMETER(material);

  return false;
}

#pragma endregion

//
// END SampleExportRenderMeshes command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
