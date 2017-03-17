#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshPolysrf command
//

class CCommandSampleMeshPolysrf : public CRhinoCommand
{
public:
  CCommandSampleMeshPolysrf() {}
  ~CCommandSampleMeshPolysrf() {}
  UUID CommandUUID()
  {
    // {26BD88E9-A372-4011-930E-77F4223E2294}
    static const GUID SampleMeshPolysrfCommand_UUID =
    { 0x26BD88E9, 0xA372, 0x4011, { 0x93, 0x0E, 0x77, 0xF4, 0x22, 0x3E, 0x22, 0x94 } };
    return SampleMeshPolysrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMeshPolysrf"; }
  const wchar_t* LocalCommandName() { return L"SampleMeshPolysrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMeshPolysrf object
static class CCommandSampleMeshPolysrf theSampleMeshPolysrfCommand;

CRhinoCommand::result CCommandSampleMeshPolysrf::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select polysurface to mesh");
  go.SetGeometryFilter(CRhinoGetObject::polysrf_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Brep* brep = go.Object(0).Brep();
  if (0 == brep)
    return CRhinoCommand::failure;

  ON_MeshParameters mp = ON_MeshParameters::DefaultAnalysisMesh;
  mp.SetTolerance(0.1);

  for (int i = 0; i < brep->m_F.Count(); i++)
  {
    ON_Mesh mesh;
    if (brep->Face(i)->CreateMesh(mp, &mesh))
      context.m_doc.AddMeshObject(mesh);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleMeshPolysrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
