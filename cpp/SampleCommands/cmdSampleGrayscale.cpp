#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGrayscale command
//

class CSampleGrayscaleConduit : public CRhinoDisplayConduit
{
public:
  CSampleGrayscaleConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel);

  bool Grayscale() const;
  void SetGrayscale(bool bGrayscale);

private:
  COLORREF GetAttributeColor(const ON_Color& color);
  COLORREF GetAttributeColor(COLORREF color);

private:
  bool m_bGrayscale;
};

CSampleGrayscaleConduit::CSampleGrayscaleConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_DRAWOBJECT)
{
  m_bGrayscale = true; // true = grayscale, false = black/white
}

bool CSampleGrayscaleConduit::Grayscale() const
{
  return m_bGrayscale;
}

void CSampleGrayscaleConduit::SetGrayscale(bool bGrayscale)
{
  m_bGrayscale = bGrayscale;
}

bool CSampleGrayscaleConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  UNREFERENCED_PARAMETER(dp);
  UNREFERENCED_PARAMETER(bTerminateChannel);

  if (nActiveChannel == CSupportChannels::SC_DRAWOBJECT)
  {
    const CRhinoObject* object = m_pChannelAttrs->m_pObject;
    if (object)
    {
      // Object
      m_pDisplayAttrs->m_ObjectColor = GetAttributeColor(m_pDisplayAttrs->m_ObjectColor);
      m_pDisplayAttrs->m_pMaterial->m_FrontMaterial.SetDiffuse(GetAttributeColor(m_pDisplayAttrs->m_pMaterial->m_FrontMaterial.Diffuse()));
      m_pDisplayAttrs->m_pMaterial->m_BackMaterial.SetDiffuse(GetAttributeColor(m_pDisplayAttrs->m_pMaterial->m_BackMaterial.Diffuse()));
      // Curve
      m_pDisplayAttrs->m_CurveColor = GetAttributeColor(m_pDisplayAttrs->m_CurveColor);
      // Surface and mesh
      m_pDisplayAttrs->m_WireColor = GetAttributeColor(m_pDisplayAttrs->m_WireColor);
      // Surface
      m_pDisplayAttrs->m_IsoColor = GetAttributeColor(m_pDisplayAttrs->m_IsoColor);
      m_pDisplayAttrs->m_IsoUColor = GetAttributeColor(m_pDisplayAttrs->m_IsoUColor);
      m_pDisplayAttrs->m_IsoVColor = GetAttributeColor(m_pDisplayAttrs->m_IsoVColor);
      m_pDisplayAttrs->m_IsoWColor = GetAttributeColor(m_pDisplayAttrs->m_IsoWColor);
      m_pDisplayAttrs->m_EdgeColor = GetAttributeColor(m_pDisplayAttrs->m_EdgeColor);
      // Mesh
      m_pDisplayAttrs->m_MeshWireColor = GetAttributeColor(m_pDisplayAttrs->m_MeshWireColor);
      m_pDisplayAttrs->m_NakedEdgeColor = GetAttributeColor(m_pDisplayAttrs->m_NakedEdgeColor);
      m_pDisplayAttrs->m_MeshEdgeColor = GetAttributeColor(m_pDisplayAttrs->m_MeshEdgeColor);
      m_pDisplayAttrs->m_MeshNakedEdgeColor = GetAttributeColor(m_pDisplayAttrs->m_MeshNakedEdgeColor);
      // Dimensions and text
      m_pDisplayAttrs->m_DotTextColor = GetAttributeColor(m_pDisplayAttrs->m_DotTextColor);
      m_pDisplayAttrs->m_DotBorderColor = GetAttributeColor(m_pDisplayAttrs->m_DotBorderColor);
    }
  }
  return true;
}

static COLORREF ConvertColor(COLORREF src, bool bGrayscale)
{
  unsigned int r = GetRValue(src);
  unsigned int g = GetGValue(src);
  unsigned int b = GetBValue(src);

  COLORREF cf;
  if (bGrayscale)
  {
    int luma = (int)(r * 0.3 + g * 0.59 + b * 0.11);
    cf = RGB(luma, luma, luma);
  }
  else
  {
    double average = (r + g + b) / 3;
    cf = (average < 128) ? RGB(0, 0, 0) : RGB(255, 255, 255);
  }

  return cf;
}

COLORREF CSampleGrayscaleConduit::GetAttributeColor(const ON_Color& color)
{
  return ConvertColor(color, m_bGrayscale);
}

COLORREF CSampleGrayscaleConduit::GetAttributeColor(COLORREF color)
{
  return ConvertColor(color, m_bGrayscale);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleGrayscale : public CRhinoCommand
{
public:
  CCommandSampleGrayscale();
  ~CCommandSampleGrayscale() {}
  UUID CommandUUID()
  {
    // {EAED35E9-14BC-4D35-B5F8-FB1BBEC390C0}
    static const GUID SampleGrayscaleCommand_UUID =
    { 0xEAED35E9, 0x14BC, 0x4D35, { 0xB5, 0xF8, 0xFB, 0x1B, 0xBE, 0xC3, 0x90, 0xC0 } };
    return SampleGrayscaleCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGrayscale"; }
  const wchar_t* LocalCommandName() { return L"SampleGrayscale"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CSampleGrayscaleConduit m_conduit;
  bool m_bGrayscale;
};

// The one and only CCommandSampleGrayscale object
static class CCommandSampleGrayscale theSampleGrayscaleCommand;

CCommandSampleGrayscale::CCommandSampleGrayscale()
{
  m_bGrayscale = true;
  m_conduit.SetGrayscale(m_bGrayscale);
}

CRhinoCommand::result CCommandSampleGrayscale::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoCommand::result rc = CRhinoCommand::success;

  CRhinoCommandOptionValue enable_opts[] = { RHCMDOPTVALUE(L"Yes"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Toggle") };

  for (;;)
  {
    int current_opt = m_conduit.IsEnabled() ? 0 : 1;

    CRhinoGetOption go;
    go.SetCommandPrompt(L"Choose grayscale option");
    go.AcceptNothing(true);
    int e_opt = go.AddCommandOptionList(RHCMDOPTNAME(L"Enable"), 3, enable_opts, current_opt);
    int g_opt = go.AddCommandOptionToggle(RHCMDOPTNAME(L"Grayscale"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), m_bGrayscale, &m_bGrayscale);

    CRhinoGet::result res = go.GetOption();

    if (res == CRhinoGet::nothing)
      break;

    else if (res != CRhinoGet::option)
    {
      rc = CRhinoCommand::cancel;
      break;
    }

    else
    {
      const CRhinoCommandOption* option = go.Option();
      if (0 == option)
      {
        rc = CRhinoCommand::failure;
        break;
      }

      if (e_opt == option->m_option_index)
      {
        bool bIsEnabled = m_conduit.IsEnabled();
        bool bEnable = false;
        switch (option->m_list_option_current)
        {
        case 0: // on
          bEnable = true;
          break;
        case 1: // off
          bEnable = false;
          break;
        default:
          bEnable = bIsEnabled;
          break;
        }

        if (bEnable && !bIsEnabled)
        {
          m_conduit.Enable(context.m_doc.RuntimeSerialNumber());
          context.m_doc.Regen();
        }
        else if (!bEnable && bIsEnabled)
        {
          m_conduit.Disable();
          context.m_doc.Regen();
        }
      }

      else if (g_opt == option->m_option_index)
      {
        m_conduit.SetGrayscale(m_bGrayscale);
        if (m_conduit.IsEnabled())
          context.m_doc.Regen();
      }
    }
  }

  return rc;
}

//
// END SampleGrayscale command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
