#include "StdAfx.h"
#include "SampleCommandsPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectCursor command
//

#pragma region SampleObjectCursor command

class CCommandSampleObjectCursor : public CRhinoCommand
{
public:
  CCommandSampleObjectCursor() = default;
  ~CCommandSampleObjectCursor() = default;
  UUID CommandUUID() override
  {
    // {1E0B30A9-4015-4205-ADD5-E812D950283F}
    static const GUID SampleObjectCursorCommand_UUID =
    { 0x1E0B30A9, 0x4015, 0x4205, { 0xAD, 0xD5, 0xE8, 0x12, 0xD9, 0x50, 0x28, 0x3F } };
    return SampleObjectCursorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleObjectCursor"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleObjectCursor object
static class CCommandSampleObjectCursor theSampleObjectCursorCommand;

CRhinoCommand::result CCommandSampleObjectCursor::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object to delete");
  go.AcceptNothing(FALSE);

  HCURSOR hCursor = SampleCommandsPlugIn().SampleCursor();
  if (0 != hCursor)
    RhinoSetCursor(hCursor);

  go.GetObjects(1, 1);

  if (0 != hCursor)
    RhinoSetCursor(0);

  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  doc->DeleteObject(go.Object(0));
  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleObjectCursor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
