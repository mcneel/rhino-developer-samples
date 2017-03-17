#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePurgeGroups command
//

class CCommandSamplePurgeGroups : public CRhinoCommand
{
public:
  CCommandSamplePurgeGroups() {}
  ~CCommandSamplePurgeGroups() {}
  UUID CommandUUID()
  {
    // {3A9F8B60-7411-4360-A7E0-12FE8CEE660F}
    static const GUID SamplePurgeGroupsCommand_UUID =
    { 0x3A9F8B60, 0x7411, 0x4360, { 0xA7, 0xE0, 0x12, 0xFE, 0x8C, 0xEE, 0x66, 0x0F } };
    return SamplePurgeGroupsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePurgeGroups"; }
  const wchar_t* LocalCommandName() { return L"SamplePurgeGroups"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePurgeGroups object
static class CCommandSamplePurgeGroups theSamplePurgeGroupsCommand;

CRhinoCommand::result CCommandSamplePurgeGroups::RunCommand(const CRhinoCommandContext& context)
{
  int deleted_count = 0;

  CRhinoGroupTable& group_table = context.m_doc.m_group_table;
  const int group_count = group_table.GroupCount();

  int group_index = 0;
  for (group_index = 0; group_index < group_count; group_index++)
  {
    const CRhinoGroup* group = group_table[group_index];
    if (0 == group)
      continue;

    if (group->IsDeleted())
      continue;

    if (group->IsReferenceComponent())
      continue;

    bool bHasMembers = false;

    CRhinoObjectIterator it(CRhinoObjectIterator::undeleted_objects, CRhinoObjectIterator::active_and_reference_objects);
    it.IncludeLights(true);

    CRhinoObject* obj = 0;
    for (obj = it.First(); obj; obj = it.Next())
    {
      if (obj->Attributes().IsInGroup(group_index))
      {
        bHasMembers = true;
        break;
      }
    }

    if (bHasMembers && group_table.DeleteGroup(group_index))
      deleted_count++;
  }

  return CRhinoCommand::success;
}

//
// END SamplePurgeGroups command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
