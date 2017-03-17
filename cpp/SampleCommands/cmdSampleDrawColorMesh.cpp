#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawColorMesh command
//

class CConduitDrawColorMesh : public CRhinoDisplayConduit
{
public:
  CConduitDrawColorMesh(const ON_Mesh* pMesh);
  ~CConduitDrawColorMesh() {}

  bool ExecConduit(
    CRhinoDisplayPipeline& dp, // pipeline executing this conduit
    UINT nChannel,             // current channel within the pipeline
    bool& bTerminate           // channel termination flag
  );

private:
  const ON_Mesh* m_pMesh;
};

CConduitDrawColorMesh::CConduitDrawColorMesh(const ON_Mesh* pMesh)
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_POSTDRAWOBJECTS),
  m_pMesh(pMesh)
{
}

bool CConduitDrawColorMesh::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(bTerminate);

  // Determine which channel we're currently executing in...
  switch (nChannel)
  {
    case CSupportChannels::SC_CALCBOUNDINGBOX:
    {
      if (m_pMesh && m_pMesh->IsValid())
      {
        ON_BoundingBox  bbox;
        m_pMesh->GetTightBoundingBox(bbox, false);
        // Now adjust the channel attribute's bounding box with the accumulated one...
        m_pChannelAttrs->m_BoundingBox.Union(bbox);
      }
    }
    break;

    case CSupportChannels::SC_POSTDRAWOBJECTS:
    {
      if (m_pMesh && m_pMesh->IsValid())
      {
        if (!dp.DisplayAttrs()->m_bShadeSurface)
        {
          dp.DrawWireframeMesh(*m_pMesh, RGB(0, 0, 255));
        }
        else
        {
          CDisplayPipelineMaterial mat;
          dp.SetupDisplayMaterial(mat, RGB(0, 0, 255));
          dp.DrawShadedMesh(*m_pMesh, &mat);
        }
      }
    }
    break;
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleDrawColorMesh : public CRhinoCommand
{
public:
  CCommandSampleDrawColorMesh() {}
  ~CCommandSampleDrawColorMesh() {}
  UUID CommandUUID()
  {
    // {774D3D87-9CFC-48D1-BEAE-86A607F4360A}
    static const GUID SampleDrawColorMeshCommand_UUID =
    { 0x774D3D87, 0x9CFC, 0x48D1, { 0xBE, 0xAE, 0x86, 0xA6, 0x07, 0xF4, 0x36, 0x0A } };
    return SampleDrawColorMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDrawColorMesh"; }
  const wchar_t* LocalCommandName() { return L"SampleDrawColorMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDrawColorMesh object
static class CCommandSampleDrawColorMesh theSampleDrawColorMeshCommand;

CRhinoCommand::result CCommandSampleDrawColorMesh::RunCommand(const CRhinoCommandContext& context)
{
  ON_Sphere sphere;
  CArgsRhinoGetSphere args;
  CRhinoCommand::result rc = RhinoGetSphere(args, sphere);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_Mesh* pMesh = RhinoMeshSphere(sphere, 10, 10);
  if (!pMesh)
    return failure;

  CConduitDrawColorMesh conduit(pMesh);
  conduit.Enable(context.m_doc.RuntimeSerialNumber());
  context.m_doc.Regen();

  CRhinoGetPoint gp;
  gp.GetPoint();

  conduit.Disable();
  context.m_doc.Regen();

  delete pMesh;

  return CRhinoCommand::success;
}

//
// END SampleDrawColorMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
