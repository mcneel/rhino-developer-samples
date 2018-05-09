// cmdSampleRdkMaterialCustomUI.cpp : command file
//

#include "StdAfx.h"
#include "SampleRdkMaterialCustomUIPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRdkMaterialCustomUI command
//

#pragma region SampleRdkMaterialCustomUI command

// Do NOT put the definition of class CCommandSampleRdkMaterialCustomUI in a header
// file. There is only ONE instance of a CCommandSampleRdkMaterialCustomUI class
// and that instance is the static theSampleRdkMaterialCustomUICommand that appears
// immediately below the class definition.

class CCommandSampleRdkMaterialCustomUI : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleRdkMaterialCustomUI is created below.
  // No copy constructor or operator= is required.
  // Values of member variables persist for the duration of the application.

  // CCommandSampleRdkMaterialCustomUI::CCommandSampleRdkMaterialCustomUI()
  // is called exactly once when static theSampleRdkMaterialCustomUICommand is created.
  CCommandSampleRdkMaterialCustomUI() = default;

  // CCommandSampleRdkMaterialCustomUI::~CCommandSampleRdkMaterialCustomUI()
  // is called exactly once when static theSampleRdkMaterialCustomUICommand is destroyed.
  // The destructor should not make any calls to the Rhino SDK. 
  // If your command has persistent settings, then override 
  // CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleRdkMaterialCustomUI() = default;

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work. Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID() override
  {
    // {F4210068-53C6-4245-BAA7-40929E8C2DFC}
    static const GUID SampleRdkMaterialCustomUICommand_UUID =
    { 0xF4210068, 0x53C6, 0x4245, { 0xBA, 0xA7, 0x40, 0x92, 0x9E, 0x8C, 0x2D, 0xFC } };
    return SampleRdkMaterialCustomUICommand_UUID;
  }

  // Returns the English command name.
  // If you want to provide a localized command name, then override 
  // CRhinoCommand::LocalCommandName.
  const wchar_t* EnglishCommandName() override { return L"SampleRdkMaterialCustomUI"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleRdkMaterialCustomUI object
// Do NOT create any other instance of a CCommandSampleRdkMaterialCustomUI class.
static class CCommandSampleRdkMaterialCustomUI theSampleRdkMaterialCustomUICommand;

CRhinoCommand::result CCommandSampleRdkMaterialCustomUI::RunCommand(const CRhinoCommandContext& context)
{
  // CCommandSampleRdkMaterialCustomUI::RunCommand() is called when the user
  // runs the "SampleRdkMaterialCustomUI".

  // TODO: Add command code here.

  // Rhino command that display a dialog box interface should also support
  // a command-line, or scriptable interface.

  ON_wString str;
  str.Format(L"The \"%ls\" command is under construction.\n", EnglishCommandName());
  if (context.IsInteractive())
    RhinoMessageBox(str, SampleRdkMaterialCustomUIPlugIn().PlugInName(), MB_OK);
  else
    RhinoApp().Print(str);

  // TODO: Return one of the following values:
  //   CRhinoCommand::success:  The command worked.
  //   CRhinoCommand::failure:  The command failed because of invalid input, inability
  //                            to compute the desired result, or some other reason
  //                            computation reason.
  //   CRhinoCommand::cancel:   The user interactively canceled the command 
  //                            (by pressing ESCAPE, clicking a CANCEL button, etc.)
  //                            in a Get operation, dialog, time consuming computation, etc.

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRdkMaterialCustomUI command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
