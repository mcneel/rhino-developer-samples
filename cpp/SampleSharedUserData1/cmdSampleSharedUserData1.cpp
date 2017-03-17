#include "StdAfx.h"
#include "SampleSharedUserData1PlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSharedUserData1 command
//

#pragma region SampleSharedUserData1 command

class CCommandSampleSharedUserData1 : public CRhinoCommand
{
public:
  CCommandSampleSharedUserData1() {}
  ~CCommandSampleSharedUserData1() {}
  UUID CommandUUID()
  {
    // {33339333-D640-4154-A694-BE7CB762DB2F}
    static const GUID SampleSharedUserData1Command_UUID =
    { 0x33339333, 0xD640, 0x4154, { 0xA6, 0x94, 0xBE, 0x7C, 0xB7, 0x62, 0xDB, 0x2F } };
    return SampleSharedUserData1Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSharedUserData1"; }
  const wchar_t* LocalCommandName() const { return L"SampleSharedUserData1"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSharedUserData1 object
static class CCommandSampleSharedUserData1 theSampleSharedUserData1Command;

CRhinoCommand::result CCommandSampleSharedUserData1::RunCommand(const CRhinoCommandContext& context)
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
// END SampleSharedUserData1 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
