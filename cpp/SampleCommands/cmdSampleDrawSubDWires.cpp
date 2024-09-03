#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawSubDWires command
//

class CSampleDrawSubDWiresConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawSubDWiresConduit(const ON_SubD& subd, ON_Color wire_color);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel) override;

private:
  const ON_SubD& m_subd;
  ON_Color m_wire_color;
};

CSampleDrawSubDWiresConduit::CSampleDrawSubDWiresConduit(const ON_SubD& subd, ON_Color wire_color)
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_DRAWOVERLAY)
  , m_subd(subd)
  , m_wire_color(wire_color)
{
}

bool CSampleDrawSubDWiresConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  if (nActiveChannel == CSupportChannels::SC_CALCBOUNDINGBOX)
  {
    ON_BoundingBox bbox;
    if (m_subd.GetTightBoundingBox(bbox, false))
      m_pChannelAttrs->m_BoundingBox.Union(bbox);
  }
  else if (nActiveChannel == CSupportChannels::SC_DRAWOVERLAY)
  {
    CDisplayPipelineAttributes* pDA = const_cast<CDisplayPipelineAttributes*>(dp.DisplayAttrs());
    if (nullptr != pDA)
    {
      CRhinoSubDDisplay subd_display = CRhinoSubDDisplay::Create(m_subd);
      CRhinoSubDDisplayAttributes subd_display_attrs;
      subd_display_attrs.m_bDrawControlNet = false;
      subd_display_attrs.m_bDrawSurfaceNet = false;
      subd_display_attrs.m_bDrawEdges = true;
      subd_display_attrs.m_bDrawSurface = false;

      ON_Color oldColors[4];
      float oldThicknesses[4];
      pDA->m_ObjectColor = m_wire_color;
      pDA->GetSubDWireAttributes(oldColors, oldThicknesses);

      // Set color
      for (int i = 0; i < 4; i++)
        pDA->SetSubDEdgeColor((CDisplayPipelineAttributes::SubDEdgeType)i, m_wire_color);

      dp.DrawSubD(subd_display, subd_display_attrs);

      // Reset color
      for (int i = 0; i < 4; i++)
        pDA->SetSubDEdgeColor((CDisplayPipelineAttributes::SubDEdgeType)i, oldColors[i]);
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleDrawSubDWires command

class CCommandSampleDrawSubDWires : public CRhinoCommand
{
public:
  CCommandSampleDrawSubDWires() = default;
  UUID CommandUUID() override
  {
    // {8A5A2555-3656-4144-B096-BE2876CF4030}
    static const GUID SampleDrawSubDWiresCommand_UUID =
    { 0x8A5A2555, 0x3656, 0x4144, { 0xB0, 0x96, 0xBE, 0x28, 0x76, 0xCF, 0x40, 0x30 } };
    return SampleDrawSubDWiresCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDrawSubDWires"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDrawSubDWires object
static class CCommandSampleDrawSubDWires theSampleDrawSubDWiresCommand;

CRhinoCommand::result CCommandSampleDrawSubDWires::RunCommand(const CRhinoCommandContext& context)
{
  ON_Sphere sphere(ON_3dPoint::Origin, 4.0);

  ON_SubD subd_sphere;
  ON_SubDComponentLocation subd_location = ON_SubDComponentLocation::Surface;
  if (nullptr == ON_SubD::CreateSubDQuadSphere(sphere, subd_location, 2, &subd_sphere))
    return CRhinoCommand::failure;

  CSampleDrawSubDWiresConduit conduit(subd_sphere, ON_Color::SaturatedGold);
  conduit.Enable(context.m_rhino_doc_sn);
  context.m_doc.Redraw();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDrawSubDWires command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
