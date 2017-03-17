#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSetObjectDisplayMode command
//

#pragma region SampleSetObjectDisplayMode command

class CCommandSampleSetObjectDisplayMode : public CRhinoCommand
{
public:
  CCommandSampleSetObjectDisplayMode() {}
  ~CCommandSampleSetObjectDisplayMode() {}
  UUID CommandUUID()
  {
    // {9D1000AB-C5F2-415B-A2CF-EEB7F94F0B39}
    static const GUID SampleSetObjectDisplayModeCommand_UUID =
    { 0x9D1000AB, 0xC5F2, 0x415B, { 0xA2, 0xCF, 0xEE, 0xB7, 0xF9, 0x4F, 0x0B, 0x39 } };
    return SampleSetObjectDisplayModeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSetObjectDisplayMode"; }
  const wchar_t* LocalCommandName() const { return L"SampleSetObjectDisplayMode"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  ON_UUID FindDisplayMode(const wchar_t* mode_id);
};

// The one and only CCommandSampleSetObjectDisplayMode object
static class CCommandSampleSetObjectDisplayMode theSampleSetObjectDisplayModeCommand;

CRhinoCommand::result CCommandSampleSetObjectDisplayMode::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to set display mode");
  go.EnableGroupSelect();
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Display mode to set");
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  ON_UUID attrs_id = FindDisplayMode(gs.String());
  if (ON_UuidIsNil(attrs_id))
  {
    RhinoApp().Print(L"Display mode not found.\n");
    return CRhinoCommand::nothing;
  }

  DisplayAttrsMgrListDesc* pAttrsDesc = CRhinoDisplayAttrsMgr::FindDisplayAttrsDesc(attrs_id);
  if (0 == pAttrsDesc)
    return CRhinoCommand::failure;

  CRhinoView* view = RhinoApp().ActiveView();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_DisplayMaterialRef dmr;
  dmr.m_viewport_id = view->ActiveViewport().VP().ViewportId();
  dmr.m_display_material_id = attrs_id;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& object_ref = go.Object(i);
    const CRhinoObject* object = object_ref.Object();
    if (0 == object)
      return CRhinoCommand::failure;

    CRhinoObjectAttributes attribs = object->Attributes();
    if (!attribs.FindDisplayMaterialRef(dmr, &dmr))
    {
      attribs.AddDisplayMaterialRef(dmr);
      context.m_doc.ModifyObjectAttributes(object_ref, attribs);
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

ON_UUID CCommandSampleSetObjectDisplayMode::FindDisplayMode(const wchar_t* mode_id)
{
  ON_UUID attrs_id = ON_nil_uuid;

  if (0 == mode_id || 0 == mode_id[0])
    return attrs_id;

  DisplayAttrsMgrList attrs_list;
  int attrs_count = CRhinoDisplayAttrsMgr::GetDisplayAttrsList(attrs_list);
  if (0 == attrs_count)
    return attrs_id;

  ON_wString str(mode_id);
  str.TrimLeftAndRight();

  ON_UUID uuid = ON_UuidFromString(str);
  bool bUuid = ON_UuidIsNotNil(uuid);

  for (int i = 0; i < attrs_count; i++)
  {
    if (!attrs_list[i].m_bAllowObjectAssignment)
      continue;

    const CDisplayPipelineAttributes* attrs = attrs_list[i].m_pAttrs;
    if (0 == attrs)
      continue;

    bool rc = false;
    if (bUuid)
      rc = (ON_UuidCompare(uuid, attrs->Id()) == 0);
    else
    {
      rc = (str.CompareNoCase(attrs->LocalName()) == 0);
      if (!rc)
        rc = (str.CompareNoCase(attrs->EnglishName()) == 0);
      if (!rc)
        rc = (str.CompareNoCase(attrs->Name()) == 0);
    }
    if (rc)
    {
      attrs_id = attrs->Id();
      break;
    }
  }

  return attrs_id;
}

#pragma endregion

//
// END SampleSetObjectDisplayMode command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
