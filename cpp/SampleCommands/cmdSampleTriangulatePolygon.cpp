#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTriangulatePolygon command
//

class CCommandSampleTriangulatePolygon : public CRhinoCommand
{
public:
  CCommandSampleTriangulatePolygon() {}
  ~CCommandSampleTriangulatePolygon() {}
  UUID CommandUUID()
  {
    // {558C0C33-496-42BE-B8B1-D9607AFCEABF}
    static const GUID SampleTriangulatePolygonCommand_UUID =
    { 0x558C0C33, 0x496, 0x42BE, { 0xB8, 0xB1, 0xD9, 0x60, 0x7A, 0xFC, 0xEA, 0xBF } };
    return SampleTriangulatePolygonCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleTriangulatePolygon"; }
  const wchar_t* LocalCommandName() { return L"SampleTriangulatePolygon"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleTriangulatePolygon object
static class CCommandSampleTriangulatePolygon theSampleTriangulatePolygonCommand;

CRhinoCommand::result CCommandSampleTriangulatePolygon::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select closed planar polygon to triangulate");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.SetGeometryFilter(CRhinoGetObject::closed_curve);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& ref = go.Object(0);

  ON_3dPointArray vertices;

  const ON_PolylineCurve* pc = ON_PolylineCurve::Cast(ref.Curve());
  if (pc)
  {
    vertices = pc->m_pline;
  }
  else
  {
    const ON_NurbsCurve* nc = ON_NurbsCurve::Cast(ref.Curve());
    if (nc)
      nc->IsPolyline(&vertices);
  }

  if (vertices.Count() < 5)
  {
    RhinoApp().Print(L"Curve not polygon with at least four sides.\n");
    return CRhinoCommand::nothing;
  }

  int* triangles = (int*)onmalloc((vertices.Count() - 3) * sizeof(int) * 3);
  if (0 == triangles)
    return CRhinoCommand::failure; // out of memory

  memset(triangles, 0, (vertices.Count() - 3) * sizeof(int) * 3);

  int rc = RhinoTriangulate3dPolygon(vertices.Count() - 1, 3, (const double*)vertices.Array(), 3, triangles);
  if (0 == rc)
  {
    int i;
    for (i = 0; i < vertices.Count() - 3; i++)
    {
      ON_Polyline pline;
      pline.Append(vertices[triangles[i * 3]]);
      pline.Append(vertices[triangles[i * 3 + 1]]);
      pline.Append(vertices[triangles[i * 3 + 2]]);
      pline.Append(pline[0]);
      context.m_doc.AddCurveObject(pline);
    }
    context.m_doc.Redraw();
  }

  onfree(triangles);

  return CRhinoCommand::success;
}

//
// END SampleTriangulatePolygon command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
