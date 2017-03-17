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
  CCommandSampleObjectCursor() {}
  ~CCommandSampleObjectCursor() {}
  UUID CommandUUID()
  {
    // {1E0B30A9-4015-4205-ADD5-E812D950283F}
    static const GUID SampleObjectCursorCommand_UUID =
    { 0x1E0B30A9, 0x4015, 0x4205, { 0xAD, 0xD5, 0xE8, 0x12, 0xD9, 0x50, 0x28, 0x3F } };
    return SampleObjectCursorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleObjectCursor"; }
  const wchar_t* LocalCommandName() const { return L"SampleObjectCursor"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleObjectCursor object
static class CCommandSampleObjectCursor theSampleObjectCursorCommand;

CRhinoCommand::result CCommandSampleObjectCursor::RunCommand(const CRhinoCommandContext& context)
{
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

  context.m_doc.DeleteObject(go.Object(0));
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleObjectCursor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
