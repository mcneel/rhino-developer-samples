#include "stdafx.h"

// Writes a curve to a file
static bool WriteCurveFile(FILE* fp, const ON_Curve* curve)
{
  if (nullptr == fp || nullptr == curve)
    return false;

  ON_BinaryFile archive(ON::archive_mode::write3dm, fp);

  int major_version = 1;
  int minor_version = 0;
  bool rc = archive.BeginWrite3dmChunk(TCODE_ANONYMOUS_CHUNK, major_version, minor_version);
  if (!rc)
    return false;

  for (;;)
  {
    // version 1.0 fields
    rc = (archive.WriteObject(curve) ? true : false);
    if (!rc) break;

    // todo...

    break;
  }

  if (!archive.EndWrite3dmChunk())
    rc = false;

  return rc;
}

// Reads a curve from a file
static bool ReadCurveFile(FILE* fp, ON_Curve*& curve)
{
  if (nullptr == fp)
    return false;

  ON_BinaryFile archive(ON::archive_mode::read3dm, fp);

  int major_version = 0;
  int minor_version = 0;
  bool rc = archive.BeginRead3dmChunk(TCODE_ANONYMOUS_CHUNK, &major_version, &minor_version);
  if (!rc)
    return false;

  for (;;)
  {
    rc = (1 == major_version);
    if (!rc) break;

    // version 1.0 fields
    ON_Object* object = 0;
    rc = (archive.ReadObject(&object) ? true : false);
    if (!rc) break;

    curve = ON_Curve::Cast(object);

    // todo...

    break;
  }

  if (!archive.EndRead3dmChunk())
    rc = false;

  return (rc && curve);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleWriteCurve command
//

#pragma region SampleWriteCurve command

class CCommandSampleWriteCurve : public CRhinoCommand
{
public:
  CCommandSampleWriteCurve() = default;
  UUID CommandUUID() override
  {
    // {2E6F3000-579A-40F9-AE14-663DC0276470}
    static const GUID SampleWriteCurveCommand_UUID =
    { 0x2E6F3000, 0x579A, 0x40F9, { 0xAE, 0x14, 0x66, 0x3D, 0xC0, 0x27, 0x64, 0x70 } };
    return SampleWriteCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleWriteCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleWriteCurve object
static class CCommandSampleWriteCurve theSampleWriteCurveCommand;

CRhinoCommand::result CCommandSampleWriteCurve::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to save to file");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* curve = go.Object(0).Curve();
  if (nullptr == curve)
    return CRhinoCommand::failure;

  ON_wString filename;
  if (context.IsInteractive())
  {
    const wchar_t* pszDefExt = L".curve";
    const wchar_t* pszFilter = L"Curve file (*.curve)|*.curve||";
    DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());

    CFileDialog dialog(FALSE, pszDefExt, nullptr, dwFlags, pszFilter, pParentWnd, 0, TRUE);
    dialog.m_ofn.lpstrTitle = L"Save";
    dialog.m_ofn.nFilterIndex = 1;
    if (dialog.DoModal() == IDCANCEL)
      return CRhinoCommand::cancel;

    filename = dialog.GetPathName();
  }
  else
  {
    CRhinoGetString gs;
    gs.SetCommandPrompt(L"Save file name");
    gs.GetString();
    if (gs.CommandResult() != CRhinoCommand::success)
      return gs.CommandResult();

    filename = gs.String();
  }

  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  bool rc = false;
  FILE* fp = ON::OpenFile(filename, L"wb");
  if (fp)
  {
    rc = WriteCurveFile(fp, curve);
    ON::CloseFile(fp);
  }

  if (rc)
    RhinoApp().Print(L"\"%ls\" saved successfully.\n", static_cast<const wchar_t*>(filename));
  else
    RhinoApp().Print(L"Error writing \"%ls\".\n", static_cast<const wchar_t*>(filename));
  
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleWriteCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleReadCurve command
//

#pragma region SampleReadCurve command

class CCommandSampleReadCurve : public CRhinoCommand
{
public:
  CCommandSampleReadCurve() = default;
  UUID CommandUUID() override
  {
    // {93E1F9AC-C364-482B-8CBB-82FD09C59F9D}
    static const GUID SampleReadCurveCommand_UUID =
    { 0x93E1F9AC, 0xC364, 0x482B, { 0x8C, 0xBB, 0x82, 0xFD, 0x09, 0xC5, 0x9F, 0x9D } };
    return SampleReadCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleReadCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleReadCurve object
static class CCommandSampleReadCurve theSampleReadCurveCommand;

CRhinoCommand::result CCommandSampleReadCurve::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString filename;
  if (context.IsInteractive())
  {
    const wchar_t* pszDefExt = L".curve";
    const wchar_t* pszFilter = L"Curve file (*.curve)|*.curve||";
    DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());

    CFileDialog dialog(TRUE, pszDefExt, nullptr, dwFlags, pszFilter, pParentWnd, 0, TRUE);
    dialog.m_ofn.lpstrTitle = L"Open";
    dialog.m_ofn.nFilterIndex = 1;
    if (dialog.DoModal() == IDCANCEL)
      return CRhinoCommand::cancel;

    filename = dialog.GetPathName();
  }
  else
  {
    CRhinoGetString gs;
    gs.SetCommandPrompt(L"Name of file to open");
    gs.GetString();
    if (gs.CommandResult() != CRhinoCommand::success)
      return gs.CommandResult();

    filename = gs.String();
  }

  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"File \"%ls\" not found.\n", static_cast<const wchar_t*>(filename));
    return CRhinoCommand::failure;
  }

  bool rc = false;
  ON_Curve* curve = nullptr;
  FILE* fp = ON::OpenFile(filename, L"rb");
  if (fp)
  {
    rc = ReadCurveFile(fp, curve);
    ON::CloseFile(fp);
  }

  if (rc && nullptr != curve)
  {
    CRhinoCurveObject* curve_object = new CRhinoCurveObject();
    curve_object->SetCurve(curve);
    context.m_doc.AddObject(curve_object);
    context.m_doc.Redraw();
  }
  else
  {
    RhinoApp().Print(L"Error reading \"%ls\".\n", static_cast<const wchar_t*>(filename));
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleReadCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
