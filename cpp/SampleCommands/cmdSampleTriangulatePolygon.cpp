#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTriangulatePolygon command
//

class CCommandSampleTriangulatePolygon : public CRhinoCommand
{
public:
  CCommandSampleTriangulatePolygon() = default;
  ~CCommandSampleTriangulatePolygon() = default;
  UUID CommandUUID() override
  {
    // {558C0C33-496-42BE-B8B1-D9607AFCEABF}
    static const GUID SampleTriangulatePolygonCommand_UUID =
    { 0x558C0C33, 0x496, 0x42BE, { 0xB8, 0xB1, 0xD9, 0x60, 0x7A, 0xFC, 0xEA, 0xBF } };
    return SampleTriangulatePolygonCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleTriangulatePolygon"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleTriangulatePolygon object
static class CCommandSampleTriangulatePolygon theSampleTriangulatePolygonCommand;

CRhinoCommand::result CCommandSampleTriangulatePolygon::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select closed planar polygon to triangulate");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::closed_curve);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* curve = go.Object(0).Curve();
  if (nullptr == curve || !curve->IsClosed())
    return CRhinoCommand::failure;

  ON_SimpleArray<ON_3dPoint> points;
  if (0 == curve->IsPolyline(&points, nullptr))
    return CRhinoCommand::failure;

  doc->UnselectAll();

  if (points[0] == points[points.Count() - 1])
    points.SetCount(points.Count() - 1);

  const int point_count = points.Count();
  ON_SimpleArray<int> triangles((point_count - 2) * 3);
  triangles.SetCount(triangles.Capacity());

  int rc = RhinoTriangulate3dPolygon(point_count, 3, (const double*)points.Array(), 3, triangles.Array());
  if (rc != 0)
    return CRhinoCommand::failure;

  const int triangle_count = triangles.Count() / 3;
  for (int i = 0; i < triangle_count; i++)
  {
    ON_Polyline pline;
    pline.Append(points[triangles[3 * i]]);
    pline.Append(points[triangles[(3 * i) + 1]]);
    pline.Append(points[triangles[(3 * i) + 2]]);
    pline.Append(pline[0]);
    doc->AddCurveObject(pline);
  }
  doc->Redraw();

  return CRhinoCommand::success;
}

//
// END SampleTriangulatePolygon command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
