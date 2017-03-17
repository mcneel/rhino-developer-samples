#include "StdAfx.h"
#include "SampleAutomationPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAutomation command
//

#pragma region SampleAutomation command

class CCommandSampleAutomation : public CRhinoCommand
{
public:
  CCommandSampleAutomation() {}
  ~CCommandSampleAutomation() {}
  UUID CommandUUID()
  {
    // {18EB7E00-B3CD-4501-85D2-AFCC62BB07E3}
    static const GUID SampleAutomationCommand_UUID =
    { 0x18EB7E00, 0xB3CD, 0x4501, { 0x85, 0xD2, 0xAF, 0xCC, 0x62, 0xBB, 0x7, 0xE3 } };
    return SampleAutomationCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleAutomation"; }
  const wchar_t* LocalCommandName() const { return L"SampleAutomation"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleAutomation object
static class CCommandSampleAutomation theSampleAutomationCommand;

CRhinoCommand::result CCommandSampleAutomation::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);
  RhinoApp().Print(L"%s plug-in loaded.\n", SampleAutomationPlugIn().PlugInName());
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleAutomation command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
