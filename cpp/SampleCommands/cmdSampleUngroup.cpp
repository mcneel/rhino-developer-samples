#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleUngroup command
//

#pragma region SampleUngroup command

class CCommandSampleUngroup : public CRhinoCommand
{
public:
  CCommandSampleUngroup() = default;
  UUID CommandUUID() override
  {
    // {28D077B5-68E4-4159-836F-D7382EC931D1}
    static const GUID SampleUngroupCommand_UUID =
    { 0x28D077B5, 0x68E4, 0x4159,{ 0x83, 0x6F, 0xD7, 0x38, 0x2E, 0xC9, 0x31, 0xD1 } };
    return SampleUngroupCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleUngroup"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleUngroup object
static class CCommandSampleUngroup theSampleUngroupCommand;

CRhinoCommand::result CCommandSampleUngroup::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to ungroup");
  go.EnableGroupSelect(TRUE);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& obj_ref = go.Object(i);
    const CRhinoObject* obj = obj_ref.Object();
    if (nullptr != obj && obj->Attributes().TopGroup() > -1)
    {
      CRhinoObjectAttributes attrib(obj->Attributes());
      attrib.RemoveFromTopGroup();
      doc->ModifyObjectAttributes(obj_ref, attrib);
    }
  }
  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleUngroup command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
