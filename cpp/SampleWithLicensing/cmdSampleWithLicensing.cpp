#include "StdAfx.h"
#include "SampleWithLicensingPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleWithLicensing command
//

#pragma region SampleWithLicensing command

class CCommandSampleWithLicensing : public CRhinoCommand
{
public:
  CCommandSampleWithLicensing() {}
  ~CCommandSampleWithLicensing() {}
  UUID CommandUUID()
  {
    // {9A95C5F8-9309-4FE5-A97F-6EA783AB9024}
    static const GUID SampleWithLicensingCommand_UUID =
    { 0x9A95C5F8, 0x9309, 0x4FE5, { 0xA9, 0x7F, 0x6E, 0xA7, 0x83, 0xAB, 0x90, 0x24 } };
    return SampleWithLicensingCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleWithLicensing"; }
  const wchar_t* LocalCommandName() const { return L"SampleWithLicensing"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

static class CCommandSampleWithLicensing theSampleWithLicensingCommand;

CRhinoCommand::result CCommandSampleWithLicensing::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString wStr;
  wStr.Format(L"The \"%s\" command is under construction.\n", EnglishCommandName());
  if (context.IsInteractive())
    RhinoMessageBox(wStr, PlugIn()->PlugInName(), MB_OK);
  else
    RhinoApp().Print(wStr);
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleWithLicensing command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
