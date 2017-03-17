#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePerspectiveColors command
//

#pragma region SamplePerspectiveColors conduit

class CSamplePerspectiveColorsConduit : public CRhinoDisplayConduit
{
public:
  CSamplePerspectiveColorsConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate);

public:
  ON_Color m_background_color;
  ON_Color m_thick_line_color;
  ON_Color m_thin_line_color;
  ON_Color m_grid_x_axis_color;
  ON_Color m_grid_y_axis_color;
  ON_Color m_grid_z_axis_color;

private:
  bool OnInitFrameBuffer(CRhinoDisplayPipeline& dp, bool& bTerminate);
  bool OnDrawBackground(CRhinoDisplayPipeline& dp, bool& bTerminate);
};

CSamplePerspectiveColorsConduit::CSamplePerspectiveColorsConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_INITFRAMEBUFFER | CSupportChannels::SC_DRAWBACKGROUND)
{
  m_background_color = ON_UNSET_COLOR;
  m_thick_line_color = ON_UNSET_COLOR;
  m_thin_line_color = ON_UNSET_COLOR;
  m_grid_x_axis_color = ON_UNSET_COLOR;
  m_grid_y_axis_color = ON_UNSET_COLOR;
  m_grid_z_axis_color = ON_UNSET_COLOR;
}

bool CSamplePerspectiveColorsConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
  if (dp.IsPrinting())
    return true;

  if (CSupportChannels::SC_INITFRAMEBUFFER == nChannel)
    return OnInitFrameBuffer(dp, bTerminate);
  else if (CSupportChannels::SC_DRAWBACKGROUND == nChannel)
    return OnDrawBackground(dp, bTerminate);

  return true;
}

bool CSamplePerspectiveColorsConduit::OnInitFrameBuffer(CRhinoDisplayPipeline& dp, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(bTerminate);

  // This should only be called once in the initial frame buffer init,
  // but just to be safe make sure it doesn't do anything in a nested call.
  if (dp.NestLevel() > 0)
    return true;

  const CRhinoViewport* viewport = dp.GetRhinoVP();
  if (viewport && viewport->VP().Projection() == ON::perspective_view)
  {
    m_pChannelAttrs->m_eFrameBufferFillMode = FBFM_SOLID_COLOR;
    m_pChannelAttrs->m_SolidColor = m_background_color;
  }
  return true;
}

bool CSamplePerspectiveColorsConduit::OnDrawBackground(CRhinoDisplayPipeline& dp, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(bTerminate);

  const CRhinoViewport* viewport = dp.GetRhinoVP();
  if (viewport && viewport->VP().Projection() == ON::perspective_view)
  {
    dp.DrawConstructionPlane(
      viewport->ConstructionPlane(),
      true, true, true,
      m_thick_line_color,
      m_thin_line_color,
      m_grid_x_axis_color,
      m_grid_y_axis_color,
      m_grid_z_axis_color
    );

    m_pChannelAttrs->m_bGridDrawn = true;
    m_pChannelAttrs->m_bAxesDrawn = true;
  }

  return true;
}

#pragma endregion


#pragma region SamplePerspectiveColors command

class CCommandSamplePerspectiveColors : public CRhinoCommand
{
public:
  CCommandSamplePerspectiveColors();
  ~CCommandSamplePerspectiveColors() {}
  UUID CommandUUID()
  {
    // {68772174-2630-40AE-AFD1-7BD2EC42D56C}
    static const GUID SamplePerspectiveColorsCommand_UUID =
    { 0x68772174, 0x2630, 0x40AE, { 0xAF, 0xD1, 0x7B, 0xD2, 0xEC, 0x42, 0xD5, 0x6C } };
    return SamplePerspectiveColorsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePerspectiveColors"; }
  const wchar_t* LocalCommandName() const { return L"SamplePerspectiveColors"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CSamplePerspectiveColorsConduit m_conduit;
};

// The one and only CCommandSamplePerspectiveColors object
static class CCommandSamplePerspectiveColors theSamplePerspectiveColorsCommand;

CCommandSamplePerspectiveColors::CCommandSamplePerspectiveColors()
{
  m_conduit.m_background_color = RGB(255, 255, 255);
  m_conduit.m_thick_line_color = RGB(191, 191, 255);
  m_conduit.m_thin_line_color = RGB(63, 63, 255);
  m_conduit.m_grid_x_axis_color = RGB(255, 0, 0);
  m_conduit.m_grid_y_axis_color = RGB(0, 255, 0);
  m_conduit.m_grid_z_axis_color = RGB(0, 0, 255);
}

CRhinoCommand::result CCommandSamplePerspectiveColors::RunCommand(const CRhinoCommandContext& context)
{
  if (m_conduit.IsEnabled())
    m_conduit.Disable();
  else
    m_conduit.Enable(context.m_doc.RuntimeSerialNumber());

  context.m_doc.Regen();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePerspectiveColors command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
