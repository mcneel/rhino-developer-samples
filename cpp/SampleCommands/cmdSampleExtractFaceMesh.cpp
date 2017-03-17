#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtractFaceMesh command
//

class CCommandSampleExtractFaceMesh : public CRhinoCommand
{
public:
  CCommandSampleExtractFaceMesh() {}
  ~CCommandSampleExtractFaceMesh() {}
  UUID CommandUUID()
  {
    // {BEE6EB2A-9AD5-4930-BDF5-0169DC072715}
    static const GUID SampleExtractFaceMeshCommand_UUID =
    { 0xBEE6EB2A, 0x9AD5, 0x4930, { 0xBD, 0xF5, 0x01, 0x69, 0xDC, 0x07, 0x27, 0x15 } };
    return SampleExtractFaceMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExtractFaceMesh"; }
  const wchar_t* LocalCommandName() { return L"SampleExtractFaceMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExtractFaceMesh object
static class CCommandSampleExtractFaceMesh theSampleExtractFaceMeshCommand;

CRhinoCommand::result CCommandSampleExtractFaceMesh::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select faces");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect();
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  int i;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const ON_BrepFace* pF = go.Object(i).Face();
    if (pF)
    {
      const ON_Mesh* pM = pF->Mesh(ON::render_mesh);
      if (pM)
        context.m_doc.AddMeshObject(*pM);
    }
  }
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleExtractFaceMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
