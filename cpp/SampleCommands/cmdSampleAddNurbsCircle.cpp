#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAddNurbsCircle command
//

#pragma region SampleAddNurbsCircle command

class CCommandSampleAddNurbsCircle : public CRhinoCommand
{
public:
  CCommandSampleAddNurbsCircle() {}
  ~CCommandSampleAddNurbsCircle() {}
  UUID CommandUUID()
  {
    // {1E4E8E65-B040-4908-A495-E33143984611}
    static const GUID SampleAddNurbsCircleCommand_UUID =
    { 0x1E4E8E65, 0xB040, 0x4908, { 0xA4, 0x95, 0xE3, 0x31, 0x43, 0x98, 0x46, 0x11 } };
    return SampleAddNurbsCircleCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleAddNurbsCircle"; }
  const wchar_t* LocalCommandName() const { return L"SampleAddNurbsCircle"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleAddNurbsCircle object
static class CCommandSampleAddNurbsCircle theSampleAddNurbsCircleCommand;

CRhinoCommand::result CCommandSampleAddNurbsCircle::RunCommand(const CRhinoCommandContext& context)
{
  int dimension = 3;
  BOOL bIsRational = TRUE;
  int order = 3; // order = degree + 1
  int cv_count = 9;

  ON_NurbsCurve nc(dimension, bIsRational, order, cv_count);
  nc.SetCV(0, ON_4dPoint(1.0, 0.0, 0.0, 1.0));
  nc.SetCV(1, ON_4dPoint(0.707107, 0.707107, 0.0, 0.707107));
  nc.SetCV(2, ON_4dPoint(0.0, 1.0, 0.0, 1.0));
  nc.SetCV(3, ON_4dPoint(-0.707107, 0.707107, 0.0, 0.707107));
  nc.SetCV(4, ON_4dPoint(-1.0, 0.0, 0.0, 1.0));
  nc.SetCV(5, ON_4dPoint(-0.707107, -0.707107, 0.0, 0.707107));
  nc.SetCV(6, ON_4dPoint(0.0, -1.0, 0.0, 1.0));
  nc.SetCV(7, ON_4dPoint(0.707107, -0.707107, 0.0, 0.707107));
  nc.SetCV(8, ON_4dPoint(1.0, 0.0, 0.0, 1.0));
  nc.SetKnot(0, 0.0);
  nc.SetKnot(1, 0.0);
  nc.SetKnot(2, 0.5*ON_PI);
  nc.SetKnot(3, 0.5*ON_PI);
  nc.SetKnot(4, ON_PI);
  nc.SetKnot(5, ON_PI);
  nc.SetKnot(6, 1.5*ON_PI);
  nc.SetKnot(7, 1.5*ON_PI);
  nc.SetKnot(8, 2.0*ON_PI);
  nc.SetKnot(9, 2.0*ON_PI);

  if (nc.IsValid())
  {
    context.m_doc.AddCurveObject(nc);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleAddNurbsCircle command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
