#include "stdafx.h"

static ON_UUID RhFindDisplayMode(const wchar_t* mode_id)
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


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExportDisplayMode command
//

#pragma region SampleExportDisplayMode command

class CCommandSampleExportDisplayMode : public CRhinoCommand
{
public:
  CCommandSampleExportDisplayMode() {}
  ~CCommandSampleExportDisplayMode() {}
  UUID CommandUUID()
  {
    // {7B62187B-DBD-4CD4-BCAC-23FEC6F174E2}
    static const GUID SampleExportDisplayModeCommand_UUID =
    { 0x7B62187B, 0xDBD, 0x4CD4, { 0xBC, 0xAC, 0x23, 0xFE, 0xC6, 0xF1, 0x74, 0xE2 } };
    return SampleExportDisplayModeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExportDisplayMode"; }
  const wchar_t* LocalCommandName() const { return L"SampleExportDisplayMode"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExportDisplayMode object
static class CCommandSampleExportDisplayMode theSampleExportDisplayModeCommand;

CRhinoCommand::result CCommandSampleExportDisplayMode::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Display mode to export");
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  ON_UUID attrs_id = RhFindDisplayMode(gs.String());
  if (ON_UuidIsNil(attrs_id))
  {
    RhinoApp().Print(L"Display mode not found.\n");
    return CRhinoCommand::nothing;
  }

  DisplayAttrsMgrListDesc* pAttrsDesc = CRhinoDisplayAttrsMgr::FindDisplayAttrsDesc(attrs_id);
  if (0 == pAttrsDesc)
    return CRhinoCommand::failure;

  ON_wString filename;
  if (context.IsInteractive())
  {
    const wchar_t* pszDefExt = L".ini";
    const wchar_t* pszFilter = L"Rhino Display Mode (*.ini)|*.ini||";
    DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());

#if defined(WIN64)
    CFileDialog dlg(FALSE, pszDefExt, 0, dwFlags, pszFilter, pParentWnd, 0, TRUE);
#else
    CFileDialog dlg(FALSE, pszDefExt, 0, dwFlags, pszFilter, pParentWnd);
#endif

    dlg.m_ofn.lpstrTitle = L"Save";
    dlg.m_ofn.nFilterIndex = 1;
    if (dlg.DoModal() == IDCANCEL)
      return CRhinoCommand::cancel;

    filename = dlg.GetPathName();
  }
  else
  {
    CRhinoGetString gn;
    gn.SetCommandPrompt(L"Display Mode file name");
    gn.GetString();
    if (gn.CommandResult() != CRhinoCommand::success)
      return gs.CommandResult();

    filename = gn.String();
  }

  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  CRhinoIniProfileContext pc(CRhinoProfileContext::cl_scheme_current_user);
  pc.SetIniFileName(filename);
  bool rc = pAttrsDesc->SaveProfile(L"DisplayMode", pc);

  return (rc) ? CRhinoCommand::success : CRhinoCommand::failure;
}

#pragma endregion

//
// END SampleExportDisplayMode command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN ImportDisplayMode command
//

#pragma region ImportDisplayMode command

class CCommandImportDisplayMode : public CRhinoCommand
{
public:
  CCommandImportDisplayMode() {}
  ~CCommandImportDisplayMode() {}
  UUID CommandUUID()
  {
    // {F04E2C6F-D219-4A20-AA69-D3ABD59A6327}
    static const GUID ImportDisplayModeCommand_UUID =
    { 0xF04E2C6F, 0xD219, 0x4A20, { 0xAA, 0x69, 0xD3, 0xAB, 0xD5, 0x9A, 0x63, 0x27 } };
    return ImportDisplayModeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"ImportDisplayMode"; }
  const wchar_t* LocalCommandName() const { return L"ImportDisplayMode"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandImportDisplayMode object
static class CCommandImportDisplayMode theImportDisplayModeCommand;

CRhinoCommand::result CCommandImportDisplayMode::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString filename;

  if (context.IsInteractive())
  {
    const wchar_t* pszDefExt = L".ini";
    const wchar_t* pszFilter = L"Rhino Display Mode (*.ini)|*.ini||";
    DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());

#if defined(WIN64)
    CFileDialog dlg(TRUE, pszDefExt, 0, dwFlags, pszFilter, pParentWnd, 0, TRUE);
#else
    CFileDialog dlg(TRUE, pszDefExt, 0, dwFlags, pszFilter, pParentWnd);
#endif

    dlg.m_ofn.lpstrTitle = L"Open";
    dlg.m_ofn.nFilterIndex = 1;
    if (dlg.DoModal() == IDCANCEL)
      return CRhinoCommand::cancel;

    filename = dlg.GetPathName();
  }
  else
  {
    CRhinoGetString gs;
    gs.SetCommandPrompt(L"Display Mode file name");
    gs.GetString();
    if (gs.CommandResult() != CRhinoCommand::success)
      return gs.CommandResult();

    filename = gs.String();
  }

  filename.TrimLeftAndRight();
  if (filename.IsEmpty() || !CRhinoFileUtilities::FileExists(filename))
    return CRhinoCommand::nothing;

  CRhinoIniProfileContext pc(CRhinoProfileContext::cl_scheme_current_user);
  pc.SetIniFileName(filename);

  bool rc = false;

  CGetRhinoProfileContextIterator it(L"DisplayMode", pc);
  if (it.Iterator())
  {
    it.Iterator()->IterateAllSections();
    ON_ClassArray<ON_wString> sections = it.Iterator()->Sections();
    if (sections.Count() > 0)
    {
      ON_wString uuid_str = sections[0];
      ON_UUID uuid = ON_UuidFromString(uuid_str);
      DisplayAttrsMgrListDesc* pAttrsDesc = CRhinoDisplayAttrsMgr::FindDisplayAttrsDesc(uuid);
      if (pAttrsDesc)
      {
        pAttrsDesc = CRhinoDisplayAttrsMgr::AppendNewEntry();
        pAttrsDesc->m_pAttrs->SetUuid(uuid);
      }
      pAttrsDesc->LoadProfile(L"DisplayMode", pc);
      rc = (0 != pAttrsDesc->m_pAttrs);
    }
  }

  return (rc) ? CRhinoCommand::success : CRhinoCommand::failure;
}

#pragma endregion

//
// END ImportDisplayMode command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

