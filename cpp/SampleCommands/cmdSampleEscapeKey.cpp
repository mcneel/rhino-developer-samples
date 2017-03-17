#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleEscapeKey command
//

class CSampleEscapeKeyHandler
{
public:
  CSampleEscapeKeyHandler(bool bHookNow = false);
  ~CSampleEscapeKeyHandler();

public:
  bool Start();
  void Stop();
  bool EscapeKeyPressed() const;
  void ClearEscapeKeyPressedFlag();

protected:
  static LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam);
  static HHOOK m_KeyboardHookProc;
  static bool m_escape_pressed;
};

bool CSampleEscapeKeyHandler::m_escape_pressed = false;
HHOOK CSampleEscapeKeyHandler::m_KeyboardHookProc = NULL;

CSampleEscapeKeyHandler::CSampleEscapeKeyHandler(bool bStartNow)
{
  if (bStartNow)
    Start();
}

CSampleEscapeKeyHandler::~CSampleEscapeKeyHandler()
{
  Stop();
}

bool CSampleEscapeKeyHandler::Start()
{
  if (NULL == m_KeyboardHookProc)
    m_KeyboardHookProc = ::SetWindowsHookEx(WH_KEYBOARD, CSampleEscapeKeyHandler::HookProc, RhinoApp().RhinoInstanceHandle(), ::AfxGetThread()->m_nThreadID );
  ClearEscapeKeyPressedFlag();
  return(NULL != m_KeyboardHookProc);
}

void CSampleEscapeKeyHandler::Stop()
{
  if (m_KeyboardHookProc)
    UnhookWindowsHookEx(m_KeyboardHookProc);
  m_KeyboardHookProc = NULL;
}

bool CSampleEscapeKeyHandler::EscapeKeyPressed() const
{
  RhinoApp().Wait(0);
  return m_escape_pressed;
}

void CSampleEscapeKeyHandler::ClearEscapeKeyPressedFlag()
{
  m_escape_pressed = false;
}

LRESULT CALLBACK CSampleEscapeKeyHandler::HookProc(int code, WPARAM wParam, LPARAM lParam)
{
  // On esc_handler key down....
  if (code == HC_ACTION && wParam == VK_ESCAPE && !(lParam & 0x80000000))
  {
    m_escape_pressed = true;
    UnhookWindowsHookEx(m_KeyboardHookProc);
    m_KeyboardHookProc = NULL;
    return 0; // Eat the esc_handler key
  }
  // Call next hook proc including standard windows proc.
  return CallNextHookEx(m_KeyboardHookProc, code, wParam, lParam);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleEscapeKey : public CRhinoCommand
{
public:
  CCommandSampleEscapeKey() {}
  ~CCommandSampleEscapeKey() {}
  UUID CommandUUID()
  {
    // {55972E8-C866-4432-AD29-515298811890}
    static const GUID SampleEscapeKeyCommand_UUID =
    { 0x55972E8, 0xC866, 0x4432,{ 0xAD, 0x29, 0x51, 0x52, 0x98, 0x81, 0x18, 0x90 } };
    return SampleEscapeKeyCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleEscapeKey"; }
  const wchar_t* LocalCommandName() const { return L"SampleEscapeKey"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleEscapeKey object
static class CCommandSampleEscapeKey theSampleEscapeKeyCommand;

CRhinoCommand::result CCommandSampleEscapeKey::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CSampleEscapeKeyHandler esc_handler;
  esc_handler.Start();

  ON_wString prompt;
  int i = 0;
  while (i < 1000)
  {
    if (esc_handler.EscapeKeyPressed())
    {
      esc_handler.Stop();
      RhinoApp().Print(L"Command canceled.\n");
      break;
    }
    prompt.Format(L"Count = %d\n", ++i);
    RhinoApp().SetCommandPromptMessage(prompt);
  }

  RhinoApp().SetCommandPrompt(0);

  return CRhinoCommand::success;
}

//
// END SampleEscapeKey command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleEscapeKeySimple command
//

class CCommandSampleEscapeKeySimple : public CRhinoCommand
{
public:
  CCommandSampleEscapeKeySimple() {}
  ~CCommandSampleEscapeKeySimple() {}
  UUID CommandUUID()
  {
    // {ECB5023-8915-41D2-BF8F-908F270B4113}
    static const GUID SampleEscapeKeySimpleCommand_UUID =
    { 0xECB5023, 0x8915, 0x41D2,{ 0xBF, 0x8F, 0x90, 0x8F, 0x27, 0x0B, 0x41, 0x13 } };
    return SampleEscapeKeySimpleCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleEscapeKeySimple"; }
  const wchar_t* LocalCommandName() const { return L"SampleEscapeKeySimple"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

public:
  enum class EscapeKeyResult : int
  {
    QuitRhino = -1,
    EscapeKeyNotPressed = 0,
    EscapeKeyPressed = 1
  };

  EscapeKeyResult EscapeKeyPressed();
};

// The one and only CCommandSampleEscapeKeySimple object
static class CCommandSampleEscapeKeySimple theSampleEscapeKeySimpleCommand;

CRhinoCommand::result CCommandSampleEscapeKeySimple::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  ON_wString prompt;
  int i = 0;
  while (i < 1000)
  {
    if (EscapeKeyPressed() != EscapeKeyResult::EscapeKeyNotPressed)
    {
      RhinoApp().Print(L"Command canceled.\n");
      break;
    }
    prompt.Format(L"Count = %d\n", ++i);
    RhinoApp().SetCommandPromptMessage(prompt);
  }

  RhinoApp().SetCommandPrompt(0);

  return CRhinoCommand::success;
}

CCommandSampleEscapeKeySimple::EscapeKeyResult CCommandSampleEscapeKeySimple::EscapeKeyPressed()
{
  AFX_MANAGE_STATE(RhinoApp().RhinoModuleState());

  MSG msg;
  memset(&msg, 0, sizeof(msg));
  while (::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE))
  {
    if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
      return EscapeKeyResult::EscapeKeyPressed;

    if (!RhinoApp().PumpMessage())
      return EscapeKeyResult::QuitRhino;
  }

  return EscapeKeyResult::EscapeKeyNotPressed;
}


//
// END SampleEscapeKeySimple command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
