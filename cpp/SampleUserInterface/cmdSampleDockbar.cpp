#include "StdAfx.h"
#include "SampleUserInterfacePlugIn.h"
#include "SampleRhinoDockbar.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDockbar command
//

class CCommandSampleDockbar : public CRhinoCommand
{
public:
  CCommandSampleDockbar() {}
  ~CCommandSampleDockbar() {}
  UUID CommandUUID()
  {
    // {307DFF17-729A-44F5-A324-25489796342D}
    static const GUID SampleDockbarCommand_UUID =
    { 0x307DFF17, 0x729A, 0x44F5, { 0xA3, 0x24, 0x25, 0x48, 0x97, 0x96, 0x34, 0x2D } };
    return SampleDockbarCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDockbar"; }
  const wchar_t* LocalCommandName() const { return L"SampleDockbar"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDockbar object
static class CCommandSampleDockbar theSampleDockbarCommand;

CRhinoCommand::result CCommandSampleDockbar::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);
  RhinoUiDockBarManager().ShowDockBar(CSampleRhinoDockbar().DockBarID(), true, false);
  return CRhinoCommand::success;
}

//
// END SampleDockbar command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
