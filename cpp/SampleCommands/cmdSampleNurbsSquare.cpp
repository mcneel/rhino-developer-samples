#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleNurbsSquare command
//

#pragma region SampleNurbsSquare command

class CCommandSampleNurbsSquare : public CRhinoCommand
{
public:
  CCommandSampleNurbsSquare() = default;
  UUID CommandUUID() override
  {
    // {AA0040C6-BE5F-46B1-BAC2-DF8E1D8CF203}
    static const GUID SampleNurbsSquareCommand_UUID =
    { 0xAA0040C6, 0xBE5F, 0x46B1,{ 0xBA, 0xC2, 0xDF, 0x8E, 0x1D, 0x8C, 0xF2, 0x03 } };
    return SampleNurbsSquareCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleNurbsSquare"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleNurbsSquare object
static class CCommandSampleNurbsSquare theSampleNurbsSquareCommand;

CRhinoCommand::result CCommandSampleNurbsSquare::RunCommand(const CRhinoCommandContext& context)
{
  // The degree must be >= 1
  int degree = 1;

  // The order = degree + 1
  int order = degree + 1;

  // The number of control points must be >= (degree + 1)
  int cv_count = 2;

  // Create a non-rational degree 1 NURBS curve with 2x2 control points
  ON_NurbsSurface nurbs(3, FALSE, order, order, cv_count, cv_count);

  // Add "Euclidean",or world 3-D, locations for the control points
  nurbs.SetCV(0, 0, ON_3dPoint(0, 0, 0));
  nurbs.SetCV(0, 1, ON_3dPoint(0, 10, 0));
  nurbs.SetCV(1, 0, ON_3dPoint(10, 0, 0));
  nurbs.SetCV(1, 1, ON_3dPoint(10, 10, 0));

  // The number of knots must be = degree + cv_count - 1
  nurbs.SetKnot(0, 0, 0);
  nurbs.SetKnot(0, 1, 10);
  nurbs.SetKnot(1, 0, 0);
  nurbs.SetKnot(1, 1, 10);

  if (nurbs.IsValid())
  {
    context.m_doc.AddSurfaceObject(nurbs);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleNurbsSquare command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
