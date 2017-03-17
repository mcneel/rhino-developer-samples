#include "stdafx.h"
#include "SampleEventWatcherPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleUndoable command
//

#pragma region SampleUndoable command

class CCommandSampleUndoable : public CRhinoCommand
{
public:
  CCommandSampleUndoable() {}
  ~CCommandSampleUndoable() {}
  UUID CommandUUID()
  {
    // {CF4F6A90-CBF0-4E5D-A9E9-AEF3CC92C2EB}
    static const GUID SampleUndoableCommand_UUID =
    { 0xCF4F6A90, 0xCBF0, 0x4E5D, { 0xA9, 0xE9, 0xAE, 0xF3, 0xCC, 0x92, 0xC2, 0xEB } };
    return SampleUndoableCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleUndoable"; }
  const wchar_t* LocalCommandName() const { return L"SampleUndoable"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleUndoable object
static class CCommandSampleUndoable theSampleUndoableCommand;

CRhinoCommand::result CCommandSampleUndoable::RunCommand(const CRhinoCommandContext& context)
{
  return SampleEventWatcherPlugIn().MoveObjects(context);
}

#pragma endregion

//
// END SampleUndoable command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
