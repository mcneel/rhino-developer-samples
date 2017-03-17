#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleClosestAxisPoint command
//

class CRhGetPlanarCurve : public CRhinoGetObject
{
public:
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const;
};

bool CRhGetPlanarCurve::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const
{
  UNREFERENCED_PARAMETER(object);
  UNREFERENCED_PARAMETER(component_index);

  bool rc = false;
  if (geometry)
  {
    const ON_Curve* curve = ON_Curve::Cast(geometry);
    if (curve)
    {
      if (curve->IsPlanar(0, ON_ZERO_TOLERANCE) && curve->IsInPlane(ON_xy_plane, ON_ZERO_TOLERANCE))
        rc = true;
    }
  }
  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleClosestAxisPoint : public CRhinoCommand
{
public:
  CCommandSampleClosestAxisPoint() {}
  ~CCommandSampleClosestAxisPoint() {}
  UUID CommandUUID()
  {
    // {3D753554-2B6B-4E08-8EA3-E90856D0F011}
    static const GUID SampleClosestAxisPointCommand_UUID =
    { 0x3D753554, 0x2B6B, 0x4E08, { 0x8E, 0xA3, 0xE9, 0x08, 0x56, 0xD0, 0xF0, 0x11 } };
    return SampleClosestAxisPointCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleClosestAxisPoint"; }
  const wchar_t* LocalCommandName() { return L"SampleClosestAxisPoint"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleClosestAxisPoint object
static class CCommandSampleClosestAxisPoint theSampleClosestAxisPointCommand;

CRhinoCommand::result CCommandSampleClosestAxisPoint::RunCommand(const CRhinoCommandContext& context)
{
  CRhGetPlanarCurve go;
  go.SetCommandPrompt(L"Select planar curves");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoGetOption ga;
  ga.SetCommandPrompt(L"Closest axis");
  int x_idx = ga.AddCommandOption(RHCMDOPTNAME(L"Xaxis"));
  /*int y_idx =*/ ga.AddCommandOption(RHCMDOPTNAME(L"Yaxis"));
  ga.GetOption();
  if (ga.CommandResult() != CRhinoCommand::success)
    return ga.CommandResult();

  const CRhinoCommandOption* opt = ga.Option();
  if (0 == opt)
    return CRhinoCommand::failure;

  int idx = opt->m_option_index;

  int i, num_added = 0;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& ref = go.Object(i);

    const CRhinoObject* obj = ref.Object();
    if (0 == obj)
      continue;

    const ON_Curve* crv = ref.Curve();
    if (0 == crv)
      continue;

    CRhinoObject* p = const_cast<CRhinoObject*>(obj);
    if (0 == p)
      continue;

    ON_SimpleArray<const CRhinoObject*> geom;
    geom.Append(p);

    ON_BoundingBox bbox;
    ON_Plane plane = ON_xy_plane;
    if (!RhinoGetTightBoundingBox(geom, bbox, false, &plane))
      continue;

    ON_Line line;
    if (idx == x_idx)
    {
      line.from = bbox.Corner(0, 0, 0);
      line.to = bbox.Corner(0, 1, 0);
    }
    else
    {
      line.from = bbox.Corner(0, 0, 0);
      line.to = bbox.Corner(1, 0, 0);
    }

    ON_LineCurve linecrv(line);

    double tol = context.m_doc.AbsoluteTolerance();
    ON_SimpleArray<ON_X_EVENT> x_events;
    int x_count = crv->IntersectCurve(&linecrv, x_events, tol, tol);

    if (x_count)
    {
      ON_X_EVENT x = x_events[0];
      if (x.IsPointEvent() || x.IsOverlapEvent())
      {
        context.m_doc.AddPointObject(x.m_A[0]);
        num_added++;
      }
    }
  }

  if (1 == num_added)
    RhinoApp().Print(L"1 point added.\n");
  else
    RhinoApp().Print(L"%d points added.\n", num_added);

  return CRhinoCommand::success;
}

//
// END SampleClosestAxisPoint command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
