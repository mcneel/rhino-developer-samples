#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawSubD command
//

class CSampleDrawSubDConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawSubDConduit(const ON_SubD& subd, ON_Color color);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel) override;

private:
  const ON_SubD& m_subd;
  ON_Color m_color;
};

CSampleDrawSubDConduit::CSampleDrawSubDConduit(const ON_SubD& subd, ON_Color color)
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_PREDRAWOBJECTS)
  , m_subd(subd)
  , m_color(color)
{
}

bool CSampleDrawSubDConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  if (nActiveChannel == CSupportChannels::SC_CALCBOUNDINGBOX)
  {
    ON_BoundingBox bbox;
    if (m_subd.GetTightBoundingBox(bbox, false))
      m_pChannelAttrs->m_BoundingBox.Union(bbox);
  }
  else if (nActiveChannel == CSupportChannels::SC_PREDRAWOBJECTS)
  {
    CDisplayPipelineAttributes* pDA = const_cast<CDisplayPipelineAttributes*>(dp.DisplayAttrs());
    if (nullptr != pDA)
    {
      CRhinoSubDDisplay subd_display = CRhinoSubDDisplay::Create(m_subd);
      CRhinoSubDDisplayAttributes subd_display_attrs;
      subd_display_attrs.m_bDrawControlNet = false;
      subd_display_attrs.m_bDrawSurfaceNet = false;
      subd_display_attrs.m_bDrawEdges = false;
      subd_display_attrs.m_bDrawSurface = true;

      ON_Material mat;
      mat.m_diffuse = m_color;
      CDisplayPipelineMaterial material(mat);

      CDisplayPipelineMaterial* pOldMaterial = pDA->m_pMaterial;
      pDA->m_pMaterial = &material;

      dp.DrawSubD(subd_display, subd_display_attrs);

      pDA->m_pMaterial = pOldMaterial;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleDrawSubD command

class CCommandSampleDrawSubD : public CRhinoCommand
{
public:
  CCommandSampleDrawSubD() = default;
  UUID CommandUUID() override
  {
    // {EA9A6A06-2CAA-4971-9E89-6DD97177CE2F}
    static const GUID SampleDrawSubDCommand_UUID =
    { 0xEA9A6A06, 0x2CAA, 0x4971, { 0x9E, 0x89, 0x6D, 0xD9, 0x71, 0x77, 0xCE, 0x2F } };
    return SampleDrawSubDCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDrawSubD"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDrawSubD object
static class CCommandSampleDrawSubD theSampleDrawSubDCommand;

CRhinoCommand::result CCommandSampleDrawSubD::RunCommand(const CRhinoCommandContext& context)
{
  ON_Sphere sphere(ON_3dPoint::Origin, 4.0);

  ON_SubD subd_sphere;
  ON_SubDComponentLocation subd_location = ON_SubDComponentLocation::Surface;
  if (nullptr == ON_SubD::CreateSubDQuadSphere(sphere, subd_location, 2, &subd_sphere))
    return CRhinoCommand::failure;

  CSampleDrawSubDConduit conduit(subd_sphere, ON_Color::SaturatedMagenta);
  conduit.Enable(context.m_rhino_doc_sn);
  context.m_doc.Regen();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  context.m_doc.Regen();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDrawSubD command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
