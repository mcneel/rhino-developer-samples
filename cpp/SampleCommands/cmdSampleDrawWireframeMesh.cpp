#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawWireframeMesh command
//

class CSampleDrawWireframeMesh : public CRhinoDisplayConduit
{
public:
  CSampleDrawWireframeMesh(unsigned int runtime_object_serial_number);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate);

private:
  unsigned int m_runtime_object_serial_number;
};

CSampleDrawWireframeMesh::CSampleDrawWireframeMesh(unsigned int runtime_object_serial_number)
  : CRhinoDisplayConduit(CSupportChannels::SC_DRAWOBJECT)
{
  m_runtime_object_serial_number = runtime_object_serial_number;
}

bool CSampleDrawWireframeMesh::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(bTerminate);

  if (nChannel == CSupportChannels::SC_DRAWOBJECT)
  {
    if (m_pChannelAttrs->m_pObject)
    {
      if (m_pChannelAttrs->m_pObject->RuntimeSerialNumber() == m_runtime_object_serial_number)
      {
        if (dp.DisplayAttrs()->m_bShadeSurface)
        {
          const ON_Mesh* mesh = ON_Mesh::Cast(m_pChannelAttrs->m_pObject->Geometry());
          if (mesh)
          {
            m_pChannelAttrs->m_bDrawObject = false;
            dp.DrawWireframeMesh(*mesh, m_pDisplayAttrs->m_WireColor);
          }
        }
      }
    }
  }

  return true;
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


class CCommandSampleDrawWireframeMesh : public CRhinoCommand
{
public:
  CCommandSampleDrawWireframeMesh() {}
  ~CCommandSampleDrawWireframeMesh() {}
  UUID CommandUUID()
  {
    // {6A79E54E-FBDF-4D1D-8D0B-379FD7A228E6}
    static const GUID SampleDrawWireframeMeshCommand_UUID =
    { 0x6A79E54E, 0xFBDF, 0x4D1D, { 0x8D, 0x0B, 0x37, 0x9F, 0xD7, 0xA2, 0x28, 0xE6 } };
    return SampleDrawWireframeMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDrawWireframeMesh"; }
  const wchar_t* LocalCommandName() const { return L"SampleDrawWireframeMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDrawWireframeMesh object
static class CCommandSampleDrawWireframeMesh theSampleDrawWireframeMeshCommand;

CRhinoCommand::result CCommandSampleDrawWireframeMesh::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select mesh to draw wireframe");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoMeshObject* mesh_obj = CRhinoMeshObject::Cast(go.Object(0).Object());
  if (0 == mesh_obj)
    return CRhinoCommand::failure;

  CSampleDrawWireframeMesh conduit(mesh_obj->RuntimeSerialNumber());
  conduit.Enable(context.m_doc.RuntimeSerialNumber());
  context.m_doc.Regen();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  context.m_doc.Regen();

  return CRhinoCommand::success;
}

//
// END SampleDrawWireframeMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
