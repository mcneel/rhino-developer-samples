#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePicture command
//

#pragma region SamplePicture command

class CCommandSamplePicture : public CRhinoCommand
{
public:
  CCommandSamplePicture() = default;
  UUID CommandUUID() override
  {
    // {2C3E6264-6D41-44C5-B98D-C1FA32D71DE0}
    static const GUID SamplePictureCommand_UUID =
    { 0x2C3E6264, 0x6D41, 0x44C5, { 0xB9, 0x8D, 0xC1, 0xFA, 0x32, 0xD7, 0x1D, 0xE0 } };
    return SamplePictureCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePicture"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSamplePicture object
static class CCommandSamplePicture theSamplePictureCommand;

CRhinoCommand::result CCommandSamplePicture::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetFileDialog gf;
  gf.SetScriptMode(context.IsInteractive() ? FALSE : TRUE);
  BOOL bResult = gf.DisplayFileDialog(CRhinoGetFileDialog::open_bitmap_dialog);
  if (!bResult)
    return CRhinoCommand::cancel;

  ON_wString filename = gf.FileName();
  filename.TrimLeftAndRight();
  if (filename.IsEmpty())
    return CRhinoCommand::nothing;

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"The specified file cannot be found.\n");
    return CRhinoCommand::nothing;
  }

  CRhinoDib dib;
  if (!dib.ReadFromFile(context.m_doc.RuntimeSerialNumber(), filename))
  {
    RhinoApp().Print(L"The specified file cannot be identifed as a supported type.\n");
    return CRhinoCommand::nothing;
  }

  CArgsRhinoGetPlane args;
  args.SetCornerMode(CArgsRhinoGetPlane::mode_corners);
  args.SetAllowRounded(FALSE);
  args.SetAllowDeformable(FALSE);

  ON_3dPoint rect[5];
  CRhinoCommand::result rc = RhinoGetRectangle(args, rect);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_3dVector udir = rect[1] - rect[0];
  ON_3dVector vdir = rect[3] - rect[0];

  double aspect = (double)dib.Height() / (double)dib.Width();
  double width = udir.Length();
  double height = width * aspect;

  udir.Unitize();
  vdir.Unitize();

  rect[2] = rect[1] + vdir * width * aspect;
  rect[3] = rect[0] + vdir * width * aspect;

  ON_Plane plane(rect[0], udir, vdir);

  const CRhinoObject* obj = RhinoCreatePictureFrame(context.m_doc, plane, filename, false, true, width, height, true, true, true);
  if (nullptr == obj)
    return CRhinoCommand::failure;

  context.m_doc.Redraw();
  
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePicture command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
