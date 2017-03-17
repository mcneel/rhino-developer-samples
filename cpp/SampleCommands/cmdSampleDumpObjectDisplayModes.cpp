#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDumpObjectDisplayModes command
//

#pragma region SampleDumpObjectDisplayModes command

class CCommandSampleDumpObjectDisplayModes : public CRhinoCommand
{
public:
  CCommandSampleDumpObjectDisplayModes() {}
  ~CCommandSampleDumpObjectDisplayModes() {}
  UUID CommandUUID()
  {
    // {170391FA-554F-4F98-8995-2CC8A579553F}
    static const GUID SampleDumpObjectDisplayModesCommand_UUID =
    { 0x170391FA, 0x554F, 0x4F98, { 0x89, 0x95, 0x2C, 0xC8, 0xA5, 0x79, 0x55, 0x3F } };
    return SampleDumpObjectDisplayModesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDumpObjectDisplayModes"; }
  const wchar_t* LocalCommandName() const { return L"SampleDumpObjectDisplayModes"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDumpObjectDisplayModes object
static class CCommandSampleDumpObjectDisplayModes theSampleDumpObjectDisplayModesCommand;

CRhinoCommand::result CCommandSampleDumpObjectDisplayModes::RunCommand(const CRhinoCommandContext& context)
{
  // Dump the names and ids of display modes, that can be assigned
  // to objects, to the command line.

  DisplayAttrsMgrList attrs_list;
  int attrs_count = CRhinoDisplayAttrsMgr::GetDisplayAttrsList(attrs_list);
  if (attrs_count)
  {
    for (int i = 0; i < attrs_count; i++)
    {
      if (!attrs_list[i].m_bAllowObjectAssignment)
        continue;

      const CDisplayPipelineAttributes* attrs = attrs_list[i].m_pAttrs;
      if (0 == attrs)
        continue;

      if (0 == attrs->EnglishName() || 0 == attrs->LocalName())
        continue;

      ON_wString uuid_str, name = attrs->LocalName();
      ON_UuidToString(attrs->Id(), uuid_str);

      RhinoApp().Print(L"%s (%s)\n", name, uuid_str);
    }
  }
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDumpObjectDisplayModes command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
