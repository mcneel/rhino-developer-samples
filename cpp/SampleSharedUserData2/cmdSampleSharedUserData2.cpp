#include "StdAfx.h"
#include "SampleSharedUserData2PlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSharedUserData2 command
//

#pragma region SampleSharedUserData2 command

class CCommandSampleSharedUserData2 : public CRhinoCommand
{
public:
  CCommandSampleSharedUserData2() {}
  ~CCommandSampleSharedUserData2() {}
  UUID CommandUUID()
  {
    // {36FA76F3-5CAF-48C0-9CD0-7E5B75DD33CB}
    static const GUID SampleSharedUserData2Command_UUID =
    { 0x36FA76F3, 0x5CAF, 0x48C0, { 0x9C, 0xD0, 0x7E, 0x5B, 0x75, 0xDD, 0x33, 0xCB } };
    return SampleSharedUserData2Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSharedUserData2"; }
  const wchar_t* LocalCommandName() const { return L"SampleSharedUserData2"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSharedUserData2 object
static class CCommandSampleSharedUserData2 theSampleSharedUserData2Command;

CRhinoCommand::result CCommandSampleSharedUserData2::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object");
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObject* object = go.Object(0).Object();
  if (0 == object)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Pick point");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  CRhinoGetString gs;
  gp.SetCommandPrompt(L"Description");
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  bool rc = AddPlugInUserData(context.m_doc, object, gp.Point(), gs.String());
  RhinoApp().Print(L"%s\n", rc ? L"Succeeded!" : L"Failed!");

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSharedUserData2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
