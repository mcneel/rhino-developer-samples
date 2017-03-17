#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimAngular3 command
//

class CCommandSampleDimAngular3 : public CRhinoCommand
{
public:
  CCommandSampleDimAngular3() {}
  ~CCommandSampleDimAngular3() {}
  UUID CommandUUID()
  {
    // {E9A7C297-EA26-4ADC-8986-ABFBFCAE402F}
    static const GUID SampleDimAngular3Command_UUID =
    { 0xE9A7C297, 0xEA26, 0x4ADC, { 0x89, 0x86, 0xAB, 0xFB, 0xFC, 0xAE, 0x40, 0x2F } };
    return SampleDimAngular3Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDimAngular3"; }
  const wchar_t* LocalCommandName() const { return L"SampleDimAngular3"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDimAngular3 object
static class CCommandSampleDimAngular3 theSampleDimAngular3Command;

CRhinoCommand::result CCommandSampleDimAngular3::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select arc");
  go.SetGeometryFilter(ON::curve_object);
  go.EnablePreSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  const ON_Curve* curve = obj_ref.Curve();
  if (0 == curve)
    return CRhinoCommand::failure;

  ON_Arc arc;
  if (!curve->IsArc(0, &arc))
  {
    RhinoApp().Print(L"Curve is not an arc.\n");
    return CRhinoCommand::nothing;
  }

  ON_3dPoint p3d0 = arc.StartPoint();
  ON_3dPoint p3d1 = arc.EndPoint();
  ON_3dVector v = arc.MidPoint() - arc.Center();
  ON_3dPoint p3d2 = arc.MidPoint() + (v * 0.25); // arbitrary point

  ON_Plane plane = arc.Plane();

  ON_2dPoint p2d0, p2d1, p2d2;
  plane.ClosestPointTo(p3d0, &p2d0.x, &p2d0.y);
  plane.ClosestPointTo(p3d1, &p2d1.x, &p2d1.y);
  plane.ClosestPointTo(p3d2, &p2d2.x, &p2d2.y);

  ON_3dPoint point0 = plane.PointAt(p2d0.x, p2d0.y);
  ON_3dPoint point1 = plane.PointAt(p2d1.x, p2d1.y);
  ON_3dPoint point2 = plane.PointAt(p2d2.x, p2d2.y);

  CRhinoAngularDimension* dim = new CRhinoAngularDimension();
  dim->CreateFromPoints(plane.Origin(), point0, point1, point2);

  context.m_doc.AddObject(dim);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}


//
// END SampleDimAngular3 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
