#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtrudeTapered command
//

class CRhGetClosedPlanarCurve : public CRhinoGetObject
{
public:
  bool CustomGeometryFilter(const CRhinoObject* obj, const ON_Geometry* geom, ON_COMPONENT_INDEX ci) const
  {
    UNREFERENCED_PARAMETER(ci);
    bool rc = false;
    const ON_Curve* crv = ON_Curve::Cast(geom);
    if (0 != crv && crv->IsClosed() && 0 != obj)
    {
      CRhinoDoc* doc = obj->Document();
      if (doc && crv->IsPlanar(0, doc->AbsoluteTolerance()))
        rc = true;
    }
    return rc;
  }
};


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleExtrudeTapered : public CRhinoCommand
{
public:
  CCommandSampleExtrudeTapered() {}
  ~CCommandSampleExtrudeTapered() {}
  UUID CommandUUID()
  {
    // {36E7E077-F350-4E9A-9660-25A1EBC2F105}
    static const GUID SampleExtrudeTaperedCommand_UUID =
    { 0x36E7E077, 0xF350, 0x4E9A, { 0x96, 0x60, 0x25, 0xA1, 0xEB, 0xC2, 0xF1, 0x05 } };
    return SampleExtrudeTaperedCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExtrudeTapered"; }
  const wchar_t* LocalCommandName() { return L"SampleExtrudeTapered"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExtrudeTapered object
static class CCommandSampleExtrudeTapered theSampleExtrudeTaperedCommand;

CRhinoCommand::result CCommandSampleExtrudeTapered::RunCommand(const CRhinoCommandContext& context)
{
  CRhGetClosedPlanarCurve go;
  go.SetCommandPrompt(L"Select planar closed curve to extrude");
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* crv = go.Object(0).Curve();
  if (0 == crv)
    return CRhinoCommand::failure;

  ON_Curve* dup_crv = crv->DuplicateCurve();
  double dist = 5.0;
  ON_3dVector dir = ON_zaxis;
  ON_3dPoint base = ON_origin;
  double angle = 5.0 * ON_DEGREES_TO_RADIANS;
  int type = 0;

  ON_Plane plane;
  if (dup_crv->IsPlanar(&plane, context.m_doc.AbsoluteTolerance()))
  {
    base = plane.origin;
    ON_3dVector v = plane.Normal();
    if (-1 == v.IsParallelTo(dir))
      dup_crv->Reverse();
  }

  ON_SimpleArray<ON_Brep*> breps;
  double tol = context.m_doc.AbsoluteTolerance();
  double ang_tol = context.m_doc.AngleToleranceRadians();
  bool rc = RhinoCreateTaperedExtrude(dup_crv, dist, dir, base, angle, type, tol, ang_tol, breps);
  delete dup_crv; // Don't leak...

  if (rc && breps.Count())
  {
    for (int i = 0; i < breps.Count(); i++)
    {
      CRhinoBrepObject* brep_obj = new CRhinoBrepObject();
      brep_obj->SetBrep(breps[i]);
      context.m_doc.AddObject(brep_obj);
    }
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleExtrudeTapered command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
