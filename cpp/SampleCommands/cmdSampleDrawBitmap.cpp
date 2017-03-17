#include "stdafx.h"
#include "Resource.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawBitmap command
//

class CSampleDrawBitmapConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawBitmapConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel);
  BOOL LoadBitmap();

private:
  CRhinoDib m_dib;
  BOOL m_bDib;
  int m_width;
  int m_height;
};

CSampleDrawBitmapConduit::CSampleDrawBitmapConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_DRAWFOREGROUND)
{
  m_bDib = FALSE;
  m_width = m_height = 0;
}

bool CSampleDrawBitmapConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  UNREFERENCED_PARAMETER(bTerminateChannel);

  // If drawing objects
  if (nActiveChannel == CSupportChannels::SC_DRAWFOREGROUND)
  {
    if (LoadBitmap())
    {
      int l, r, b, t;
      dp.GetRhinoVP()->VP().GetScreenPort(&l, &r, &b, &t);
      l = r - m_width;
      t = b - m_height;
      dp.DrawBitmap(m_dib, l, t);
    }
  }
  return true;
}

BOOL CSampleDrawBitmapConduit::LoadBitmap()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (!m_bDib)
  {
    m_bDib = m_dib.LoadBmp(IDB_SAMPLE_BITMAP);
    m_width = (m_bDib) ? m_dib.Width() : 0;
    m_height = (m_bDib) ? m_dib.Height() : 0;
  }
  return m_bDib;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleDrawBitmap : public CRhinoCommand
{
public:
  CCommandSampleDrawBitmap() {}
  ~CCommandSampleDrawBitmap() {}
  UUID CommandUUID()
  {
    // {2D77453-C7B5-45C3-AB8A-6BFBC7BDB3F7}
    static const GUID SampleDrawBitmapCommand_UUID =
    { 0x2D77453, 0xC7B5, 0x45C3, { 0xAB, 0x8A, 0x6B, 0xFB, 0xC7, 0xBD, 0xB3, 0xF7 } };
    return SampleDrawBitmapCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDrawBitmap"; }
  const wchar_t* LocalCommandName() { return L"SampleDrawBitmap"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CSampleDrawBitmapConduit m_conduit;
};

// The one and only CCommandSampleDrawBitmap object
static class CCommandSampleDrawBitmap theSampleDrawBitmapCommand;

CRhinoCommand::result CCommandSampleDrawBitmap::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoCommandOptionValue enable_opts[] = { RHCMDOPTVALUE(L"Yes"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Toggle") };
  for (;;)
  {
    bool bEnable = m_conduit.IsEnabled();
    int current_index = bEnable ? 0 : 1;

    CRhinoGetOption go;
    go.SetCommandPrompt(L"Choose command option");
    go.AddCommandOptionList(RHCMDOPTNAME(L"Enable"), 3, enable_opts, current_index);
    go.AcceptNothing();

    CRhinoGet::result res = go.GetOption();
    if (res == CRhinoGet::option)
    {
      const CRhinoCommandOption* option = go.Option();
      if (0 == option)
        return CRhinoCommand::failure;

      current_index = option->m_list_option_current;
      if (0 == current_index)
      {
        if (!bEnable)
        {
          m_conduit.Enable(context.m_doc.RuntimeSerialNumber());
          context.m_doc.Regen();
        }
      }
      else if (1 == current_index)
      {
        if (bEnable)
        {
          m_conduit.Disable();
          context.m_doc.Regen();
        }
      }
      else // if( 2 == current_index )
      {
        if (bEnable)
          m_conduit.Disable();
        else
          m_conduit.Enable(context.m_doc.RuntimeSerialNumber());
        context.m_doc.Regen();
      }

      continue;
    }

    break;
  }

  return CRhinoCommand::success;
}

//
// END SampleDrawBitmap command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
