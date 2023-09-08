#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawBrepConduit display conduit
//

class CSampleDrawBrepConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawBrepConduit(const CRhinoDoc& doc);
  ~CSampleDrawBrepConduit() = default;
  void SetBreps(const ON_SimpleArray<const ON_Brep*>& breps);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel) override;

private:
  const CRhinoDoc& m_doc;
  ON_SimpleArray<const ON_Brep*> m_breps;
};

CSampleDrawBrepConduit::CSampleDrawBrepConduit(const CRhinoDoc& doc)
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_PREDRAWOBJECTS)
  , m_doc(doc)
{
}

void CSampleDrawBrepConduit::SetBreps(const ON_SimpleArray<const ON_Brep*>& breps)
{
  m_breps.Empty();
  m_breps.Append(breps.Count(), breps.Array());
}

bool CSampleDrawBrepConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  if (nActiveChannel == CSupportChannels::SC_CALCBOUNDINGBOX)
  {
    ON_BoundingBox bbox;
    for (int i = 0; i < m_breps.Count(); i++)
    {
      if (nullptr != m_breps[i])
        m_breps[i]->GetTightBoundingBox(bbox, true);
      m_pChannelAttrs->m_BoundingBox.Union(bbox);
    }
  }
  else if (nActiveChannel == CSupportChannels::SC_PREDRAWOBJECTS)
  {
    const CDisplayPipelineAttributes* attributes = dp.DisplayAttrs();
    if (nullptr != attributes && attributes->m_bShadeSurface)
    {
      CDisplayPipelineMaterial material = *attributes->m_pMaterial;
      dp.SetupDisplayMaterial(material, &m_doc, nullptr);
      for (int i = 0; i < m_breps.Count(); i++)
      {
        if (nullptr != m_breps[i])
          dp.DrawShadedBrep(m_breps[i], &material, nullptr);
      }
    }

    ON_Color color = RhinoApp().AppSettings().ActiveLayerColor();
    for (int i = 0; i < m_breps.Count(); i++)
    {
      if (nullptr != m_breps[i])
        dp.DrawBrep(m_breps[i], color, 1, false, nullptr);
    }
  }

  return true;
}

//
// END SampleDrawBrepConduit display conduit
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawBrep command
//

#pragma region SampleDrawBrep command

class CCommandSampleDrawBrep : public CRhinoCommand
{
public:
  CCommandSampleDrawBrep() = default;
  UUID CommandUUID() override
  {
    // {5287211C-2494-47FF-B37E-48E8A536C08A}
    static const GUID SampleDrawBrepCommand_UUID =
    { 0x5287211C, 0x2494, 0x47FF, { 0xB3, 0x7E, 0x48, 0xE8, 0xA5, 0x36, 0xC0, 0x8A } };
    return SampleDrawBrepCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDrawBrep"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDrawBrep object
static class CCommandSampleDrawBrep theSampleDrawBrepCommand;

CRhinoCommand::result CCommandSampleDrawBrep::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_Sphere sphere;
  CArgsRhinoGetSphere args;
  CRhinoCommand::result rc = RhinoGetSphere(args, sphere);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_RevSurface* sphere_srf = sphere.RevSurfaceForm(true);
  if (nullptr == sphere_srf)
    return CRhinoCommand::failure;

  ON_Brep* brep = ON_Brep::New();
  brep->Create(sphere_srf);

  ON_SimpleArray<const ON_Brep*> breps;
  breps.Append(brep);

  CSampleDrawBrepConduit conduit(*doc);
  conduit.SetBreps(breps);
  conduit.Enable(doc->RuntimeSerialNumber());
  doc->Regen();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  doc->Redraw();

  delete brep; // Don't leak...
  brep = nullptr;

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDrawBrep command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
