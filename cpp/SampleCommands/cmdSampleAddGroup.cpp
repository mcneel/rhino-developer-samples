#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAddGroup command
//

#pragma region SampleAddGroup command

class CCommandSampleAddGroup : public CRhinoCommand
{
public:
  CCommandSampleAddGroup() {}
  ~CCommandSampleAddGroup() {}
  UUID CommandUUID()
  {
    // {1DF99446-9652-460D-9FFE-644CF7F38774}
    static const GUID SampleAddGroupCommand_UUID =
    { 0x1DF99446, 0x9652, 0x460D, { 0x9F, 0xFE, 0x64, 0x4C, 0xF7, 0xF3, 0x87, 0x74 } };
    return SampleAddGroupCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleAddGroup"; }
  const wchar_t* LocalCommandName() const { return L"SampleAddGroup"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleAddGroup object
static class CCommandSampleAddGroup theSampleAddGroupCommand;

CRhinoCommand::result CCommandSampleAddGroup::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to group");
  go.EnableGroupSelect(TRUE);
  go.EnableSubObjectSelect(FALSE);
  go.EnableReferenceObjectSelect(FALSE);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const int object_count = go.ObjectCount();
  ON_SimpleArray<const CRhinoObject*> group_members(object_count);

  const CRhinoObject* obj = 0;
  for (int i = 0; i < object_count; i++)
  {
    obj = go.Object(i).Object();
    if (obj)
      group_members.Append(obj);
  }

  int group_index = context.m_doc.m_group_table.AddGroup(ON_Group(), group_members);

  if (go.ObjectsWerePreSelected())
    context.m_doc.FlashObjectList(group_members, 0);

  return (group_index >= 0) ? CRhinoCommand::success : CRhinoCommand::failure;
}

#pragma endregion

//
// END SampleAddGroup command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
