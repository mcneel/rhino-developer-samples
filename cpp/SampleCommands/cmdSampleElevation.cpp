#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleElevation command
//

class CCommandSampleElevation : public CRhinoCommand
{
public:
  CCommandSampleElevation() {}
  ~CCommandSampleElevation() {}
  UUID CommandUUID()
  {
    // {F6E3A8A0-EBAA-4007-BE6E-FAE945470EA5}
    static const GUID SampleElevationCommand_UUID =
    { 0xF6E3A8A0, 0xEBAA, 0x4007, { 0xBE, 0x6E, 0xFA, 0xE9, 0x45, 0x47, 0x0E, 0xA5 } };
    return SampleElevationCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleElevation"; }
  const wchar_t* LocalCommandName() { return L"SampleElevation"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleElevation object
static class CCommandSampleElevation theSampleElevationCommand;

CRhinoCommand::result CCommandSampleElevation::RunCommand(const CRhinoCommandContext& context)
{
  double x = 0.0, y = 0.0;
  ON_wString sx, sy, sz;

  // Pick a brep to evaluate
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface or polysurface to evaluate");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Get the brep geometry
  const CRhinoObjRef& ref = go.Object(0);
  const ON_Brep* brep = ref.Brep();
  if (!brep)
    return CRhinoCommand::failure;

  // Prompt for an X coordinate value
  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Value of X coordinate");
  gn.SetDefaultNumber(x);
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  x = gn.Number();
  RhinoFormatNumber(x, sx);

  // Prompt for a Y coordinate value
  gn.SetCommandPrompt(L"Value of Y coordinate");
  gn.SetDefaultNumber(y);
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  y = gn.Number();
  RhinoFormatNumber(y, sy);

  // Now that we have all of the input, we want to intersect
  // a line curve with the brep. To determine the magnitude of
  // the line, we will first get the brep's bounding box.
  ON_BoundingBox bbox;
  if (!brep->GetTightBoundingBox(bbox))
    return CRhinoCommand::failure;

  // Calculate the height of the bounding box
  ON_3dVector v = bbox.Corner(0, 0, 1) - bbox.Corner(0, 0, 0);

  // Starting point of line
  ON_3dPoint p0(x, y, 0.0);

  // Ending point of line. To make sure the line
  // completely intersects the brep, we will double
  // the height.
  ON_3dPoint p1(x, y, v.Length() * 2);

  // Create the line curve
  ON_LineCurve line(p0, p1);

  // Intersect the line with the brep
  ON_SimpleArray<ON_Curve*> curves;
  ON_3dPointArray points;
  bool rc = RhinoCurveBrepIntersect(line, *brep, context.m_doc.AbsoluteTolerance(), curves, points);
  if (false == rc || 0 == points.Count())
  {
    RhinoApp().Print(L"No maximum surface Z coordinate at %s,%s found.\n", sx, sy);
    return CRhinoCommand::nothing;
  }

  // Because it is possible that our line might intersect
  // the brep in more than one place, find the intersection
  // point that is farthest from our input coordinate.
  int i;
  ON_3dPoint pt = p0;
  for (i = 0; i < points.Count(); i++)
  {
    if (p0.DistanceTo(points[i]) > p0.DistanceTo(pt))
      pt = points[i];
  }

  // Print results
  RhinoFormatNumber(pt.z, sz);
  RhinoApp().Print(L"Maximum surface Z coordinate at %s,%s is %s.\n", sx, sy, sz);

  // Optional, add a point object
  context.m_doc.AddPointObject(pt);

  // Delete any overlap intersection curves that might have been calculated
  for (i = 0; i < curves.Count(); i++)
  {
    delete curves[i];
    curves[i] = 0;
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleElevation command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
