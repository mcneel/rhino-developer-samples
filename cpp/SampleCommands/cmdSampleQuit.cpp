#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleQuit command
//

#pragma region SampleQuit command

class CCommandSampleQuit : public CRhinoCommand
{
public:
  CCommandSampleQuit() = default;
  UUID CommandUUID() override
  {
    // {64B58B1F-C9C-4404-82FF-7E666305D19A}
    static const GUID SampleQuitCommand_UUID =
    { 0x64B58B1F, 0xC9C, 0x4404, { 0x82, 0xFF, 0x7E, 0x66, 0x63, 0x05, 0xD1, 0x9A } };
    return SampleQuitCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleQuit"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleQuit object
static class CCommandSampleQuit theSampleQuitCommand;

CRhinoCommand::result CCommandSampleQuit::RunCommand(const CRhinoCommandContext& context)
{
  context.m_doc.SetModifiedFlag(FALSE);
  ::PostMessage(RhinoApp().MainWnd(), WM_CLOSE, 0L, 0L);
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleQuit command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
