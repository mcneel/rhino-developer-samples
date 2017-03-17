#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleIntersectCrv command
//

class CCommandSampleIntersectCrv : public CRhinoCommand
{
public:
  CCommandSampleIntersectCrv() {}
  ~CCommandSampleIntersectCrv() {}
  UUID CommandUUID()
  {
    // {AAD79EBF-40AE-4170-9D90-A363B2A3B62A}
    static const GUID SampleIntersectCrvCommand_UUID =
    { 0xAAD79EBF, 0x40AE, 0x4170, { 0x9D, 0x90, 0xA3, 0x63, 0xB2, 0xA3, 0xB6, 0x2A } };
    return SampleIntersectCrvCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleIntersectCrv"; }
  const wchar_t* LocalCommandName() { return L"SampleIntersectCrv"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleIntersectCrv object
static class CCommandSampleIntersectCrv theSampleIntersectCrvCommand;

CRhinoCommand::result CCommandSampleIntersectCrv::RunCommand(const CRhinoCommandContext& context)
{
  // Select two curves to intersect
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select two curves");
  go.SetGeometryFilter(ON::curve_object);
  go.GetObjects(2, 2);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Validate input
  const ON_Curve* curveA = go.Object(0).Curve();
  const ON_Curve* curveB = go.Object(1).Curve();
  if (0 == curveA || 0 == curveB)
    return CRhinoCommand::failure;

  // Intersection tolerance
  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Intersection tolerance");
  gn.SetDefaultNumber(context.m_doc.AbsoluteTolerance());
  gn.SetLowerLimit(0.0, FALSE);
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Calculate the intersection
  double intersection_tolerance = gn.Number();
  ON_SimpleArray<ON_X_EVENT> events;
  int count = curveA->IntersectCurve(
    curveB,
    events,
    intersection_tolerance,
    0.0
  );

  // Process the results
  if (count > 0)
  {
    int i;
    for (i = 0; i < events.Count(); i++)
    {
      const ON_X_EVENT& e = events[i];
      context.m_doc.AddPointObject(e.m_A[0]);
      if (e.m_A[0].DistanceTo(e.m_B[0]) > ON_EPSILON)
      {
        context.m_doc.AddPointObject(e.m_B[0]);
        context.m_doc.AddCurveObject(ON_Line(e.m_A[0], e.m_B[0]));
      }
    }

    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleIntersectCrv command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
