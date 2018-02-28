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
  CCommandSampleCustomGrips() = default;
  ~CCommandSampleCustomGrips() = default;
  UUID CommandUUID() override
  {
    // {2F2C77D8-0010-452D-A241-2DDD0C0AA1C9}
    static const GUID SampleCustomGripsCommand_UUID =
    { 0x2f2c77d8, 0x10, 0x452d,{ 0xa2, 0x41, 0x2d, 0xdd, 0xc, 0xa, 0xa1, 0xc9 } };
    return SampleCustomGripsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCustomGrips"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleCustomGrips object.  
static class CCommandSampleCustomGrips theSampleCustomGripsCommand;

CRhinoCommand::result CCommandSampleCustomGrips::RunCommand(const CRhinoCommandContext& context)
{
  CArgsRhinoGetPlane args;
  args.SetCornerMode(CArgsRhinoGetPlane::mode_corners);
  args.SetAllowRounded(FALSE);
  args.SetAllowDeformable(FALSE);

  ON_3dPoint rect[5];
  CRhinoCommand::result rc = RhinoGetRectangle(args, rect);

  if (rc == CRhinoCommand::success)
  {
    ON_Polyline polyline;
    rect[4] = rect[0];
    polyline.Create(3, FALSE, 5, 3, (double*)&rect);
    if (!polyline.IsValid())
      return CRhinoCommand::failure;

    ON_PolylineCurve* rectangle_curve = new ON_PolylineCurve(polyline);
    if (nullptr == rectangle_curve || !rectangle_curve->IsValid())
      return CRhinoCommand::failure;

    CSampleRectangleObject* rectangle_object = new CSampleRectangleObject();
    if (nullptr != rectangle_object)
    {
      rectangle_object->SetCurve(rectangle_curve);
      if (context.m_doc.AddObject(rectangle_object))
        context.m_doc.Redraw();
      else
        delete rectangle_object;
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCustomGrips command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
