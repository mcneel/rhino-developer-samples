#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtendLines command
//

#pragma region SampleExtendLines command

class CRhinoGetLineCurve : public CRhinoGetObject
{
public:
  bool CustomGeometryFilter(const CRhinoObject* obj, const ON_Geometry* geom, ON_COMPONENT_INDEX ci) const
  {
    UNREFERENCED_PARAMETER(obj);
    UNREFERENCED_PARAMETER(ci);
    if (0 != ON_LineCurve::Cast(geom))
      return true;
    return false;
  }
};

////////////////////////////////////////////////////////////////

class CCommandSampleExtendLines : public CRhinoCommand
{
public:
  CCommandSampleExtendLines() {}
  ~CCommandSampleExtendLines() {}
  UUID CommandUUID()
  {
    // {7CFC7B7D-D26E-4244-AE51-93289307AA07}
    static const GUID SampleExtendLinesCommand_UUID =
    { 0x7CFC7B7D, 0xD26E, 0x4244, { 0xAE, 0x51, 0x93, 0x28, 0x93, 0x07, 0xAA, 0x07 } };
    return SampleExtendLinesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExtendLines"; }
  const wchar_t* LocalCommandName() const { return L"SampleExtendLines"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExtendLines object
static class CCommandSampleExtendLines theSampleExtendLinesCommand;

CRhinoCommand::result CCommandSampleExtendLines::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetLineCurve go;
  go.SetCommandPrompt(L"First line to extend");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObject* obj0 = go.Object(0).Object();
  const ON_LineCurve* line_crv0 = ON_LineCurve::Cast(go.Object(0).Curve());
  if (0 == obj0 || 0 == line_crv0)
    return CRhinoCommand::failure;

  go.SetCommandPrompt(L"Second line to extend");
  go.EnablePreSelect(FALSE);
  go.EnableDeselectAllBeforePostSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObject* obj1 = go.Object(0).Object();
  const ON_LineCurve* line_crv1 = ON_LineCurve::Cast(go.Object(0).Geometry());
  if (0 == obj1 || 0 == line_crv1)
    return CRhinoCommand::failure;

  ON_Line line0 = line_crv0->m_line;
  ON_Line line1 = line_crv1->m_line;

  ON_3dPointArray points(4);
  points.Append(line0.from);
  points.Append(line0.to);
  points.Append(line1.from);
  points.Append(line1.to);
  if (!RhinoArePointsCoplanar(points))
  {
    RhinoApp().Print(L"Lines are not coplanar.\n");
    return CRhinoCommand::nothing;
  }

  ON_3dVector v0 = (line0.to - line0.from);
  v0.Unitize();

  ON_3dVector v1 = (line1.to - line1.from);
  v1.Unitize();

  ON_Line ray0(line0.from, line0.from + v0);
  ON_Line ray1(line1.from, line1.from + v1);

  double s = 0, t = 0;
  if (!ON_Intersect(ray0, ray1, &s, &t))
  {
    RhinoApp().Print(L"Lines do not intersect.\n");
    return CRhinoCommand::nothing;
  }

  ON_3dPoint pt = line0.from + s * v0;

  if (pt.DistanceTo(line0.from) < pt.DistanceTo(line0.to))
    line0.from = pt;
  else
    line0.to = pt;

  ON_LineCurve new_line_crv0(line0);
  context.m_doc.ReplaceObject(CRhinoObjRef(obj0), new_line_crv0);

  if (pt.DistanceTo(line1.from) < pt.DistanceTo(line1.to))
    line1.from = pt;
  else
    line1.to = pt;

  ON_LineCurve new_line_crv1(line1);
  context.m_doc.ReplaceObject(CRhinoObjRef(obj1), new_line_crv1);

  //context.m_doc.AddPointObject( pt );

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleExtendLines command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
