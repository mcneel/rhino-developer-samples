#include "StdAfx.h"
#include "SampleRectangleObject.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomGrips command
//

#pragma region SampleCustomGrips command

class CCommandSampleCustomGrips : public CRhinoCommand
{
public:
  CCommandSampleCustomGrips() {}
  ~CCommandSampleCustomGrips() {}
  UUID CommandUUID()
  {
    // {C28CAF32-E5C1-4628-944D-34C47DE267BC}
    static const GUID SampleCustomGripsCommand_UUID =
    { 0xC28CAF32, 0xE5C1, 0x4628, { 0x94, 0x4D, 0x34, 0xC4, 0x7D, 0xE2, 0x67, 0xBC } };
    return SampleCustomGripsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCustomGrips"; }
  const wchar_t* LocalCommandName() const { return L"SampleCustomGrips"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCustomGrips object.  
static class CCommandSampleCustomGrips theSampleCustomGripsCommand;

CRhinoCommand::result CCommandSampleCustomGrips::RunCommand(const CRhinoCommandContext& context)
{
  ON_3dPoint rect[5];

  CArgsRhinoGetPlane args;
  args.SetCornerMode(CArgsRhinoGetPlane::mode_corners);
  args.SetAllowRounded(FALSE);
  args.SetAllowDeformable(FALSE);

  CRhinoCommand::result rc = RhinoGetRectangle(args, rect);

  if (rc == CRhinoCommand::success)
  {
    ON_Polyline pline;
    rect[4] = rect[0];
    pline.Create(3, FALSE, 5, 3, (double*)&rect);

    ON_PolylineCurve* pline_curve = new ON_PolylineCurve(pline);

    CSampleRectangleObject* rect_object = new CSampleRectangleObject();
    rect_object->SetCurve(pline_curve);

    if (context.m_doc.AddObject(rect_object))
      context.m_doc.Redraw();
    else
      delete rect_object;
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCustomGrips command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
