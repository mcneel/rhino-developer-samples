#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSelectByName command
//

#pragma region SampleSelectByName command

class CCommandSampleSelectByName : public CRhinoCommand
{
public:
  CCommandSampleSelectByName() {}
  ~CCommandSampleSelectByName() {}
  UUID CommandUUID()
  {
    // {EDEE31B8-5402-4B82-A68B-105880A61D10}
    static const GUID SampleSelectByNameCommand_UUID =
    { 0xEDEE31B8, 0x5402, 0x4B82, { 0xA6, 0x8B, 0x10, 0x58, 0x80, 0xA6, 0x1D, 0x10 } };
    return SampleSelectByNameCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSelectByName"; }
  const wchar_t* LocalCommandName() const { return L"SampleSelectByName"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSelectByName object
static class CCommandSampleSelectByName theSampleSelectByNameCommand;

CRhinoCommand::result CCommandSampleSelectByName::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Object name to select");
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  ON_wString name = gs.String();
  name.TrimLeftAndRight();
  if (name.IsEmpty())
    return CRhinoCommand::nothing;

  CRhinoObjectIterator it(CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_and_reference_objects);

  int num_selected = 0;
  CRhinoObject* obj = 0;
  for (obj = it.First(); obj; obj = it.Next())
  {
    if (obj->Attributes().m_name.WildCardMatchNoCase(name))
    {
      obj->Select(true, true, true);
      num_selected++;
    }
  }

  if (0 == num_selected)
    RhinoApp().Print(L"0 objects selected\n");
  else if (1 == num_selected)
    RhinoApp().Print(L"1 object selected\n");
  else
    RhinoApp().Print(L"%d objects selected\n", num_selected);

  if (num_selected)
    context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSelectByName command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
