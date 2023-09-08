#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawZebraPreview command
//

class CSampleDrawZebraPreviewConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawZebraPreviewConduit(const ON_Brep* brep, const ON_MeshParameters& mp, const ON_Color& color)
    : m_brep(brep)
    , m_mp(mp)
    , m_color(color)
    , CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_POSTDRAWOBJECTS)
  {
  }

  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
  {
    switch (nActiveChannel)
    {
    case CSupportChannels::SC_CALCBOUNDINGBOX:
    {
      if (m_brep)
        m_pChannelAttrs->m_BoundingBox.Union(m_brep->BoundingBox());
    }
    break;

    case CSupportChannels::SC_POSTDRAWOBJECTS:
    {
      if (m_brep)
      {
        const CRhinoZebraAnalysisSettings& zas = RhinoApp().AppSettings().ZebraAnalysisSettings();
        dp.DrawZebraPreview(m_brep, zas, m_mp, m_color, &m_cache);
      }
    }
    break;
    }

    return true;
  }

private:
  const ON_Brep* m_brep;
  const ON_MeshParameters& m_mp;
  const ON_Color m_color;
  mutable CRhinoCacheHandle m_cache;
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleDrawZebraPreview : public CRhinoCommand
{
public:
  CCommandSampleDrawZebraPreview() = default;
  UUID CommandUUID() override
  {
    // {BDA0D7EA-17F0-4F79-B99B-FB9D0468774C}
    static const GUID SampleDrawZebraPreviewCommand_UUID =
    { 0xbda0d7ea, 0x17f0, 0x4f79, { 0xb9, 0x9b, 0xfb, 0x9d, 0x4, 0x68, 0x77, 0x4c } };
    return SampleDrawZebraPreviewCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDrawZebraPreview"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&) override;
};

// The one and only CCommandSampleDrawZebraPreview object
static class CCommandSampleDrawZebraPreview theSampleDrawZebraPreviewCommand;

CRhinoCommand::result CCommandSampleDrawZebraPreview::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_Sphere sphere;
  CArgsRhinoGetSphere args;
  CRhinoCommand::result rc = RhinoGetSphere(args, sphere);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_Brep* brep = ON_BrepSphere(sphere);
  if (nullptr == brep)
    return CRhinoCommand::failure;

  const ON_MeshParameters& mp = doc->Properties().AnalysisMeshSettings();
  const ON_Color& color = RhinoApp().AppSettings().SelectedObjectColor();

  CSampleDrawZebraPreviewConduit conduit(brep, mp, color);
  conduit.Enable(doc->RuntimeSerialNumber());
  doc->Regen();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  doc->Regen();

  delete brep; // Don't leak...
  brep = nullptr;

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDrawZebraPreview command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
