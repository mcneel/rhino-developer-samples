#include "stdafx.h"
#include "SampleEventWatcherPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleNotUndoable command
//

#pragma region SampleNotUndoable command

class CCommandSampleNotUndoable : public CRhinoCommand
{
public:
  CCommandSampleNotUndoable();
  ~CCommandSampleNotUndoable() = default;
  UUID CommandUUID() override
  {
    // {E9E5A33A-46C2-448A-9945-6C76DDD6DDE0}
    static const GUID SampleNotUndoableCommand_UUID =
    { 0xE9E5A33A, 0x46C2, 0x448A, { 0x99, 0x45, 0x6C, 0x76, 0xDD, 0xD6, 0xDD, 0xE0 } };
    return SampleNotUndoableCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleNotUndoable"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleNotUndoable object
static class CCommandSampleNotUndoable theSampleNotUndoableCommand;

CCommandSampleNotUndoable::CCommandSampleNotUndoable()
{
  // Make this command un-undoable...
  EnableUndo(false);
}

CRhinoCommand::result CCommandSampleNotUndoable::RunCommand(const CRhinoCommandContext& context)
{
  return SampleEventWatcherPlugIn().MoveObjects(context);
}

#pragma endregion

//
// END SampleNotUndoable command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
