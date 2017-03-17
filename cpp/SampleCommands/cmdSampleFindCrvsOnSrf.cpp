#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleFindCrvsOnSrf command
//

class CCommandSampleFindCrvsOnSrf : public CRhinoCommand
{
public:
  CCommandSampleFindCrvsOnSrf() {}
  ~CCommandSampleFindCrvsOnSrf() {}
  UUID CommandUUID()
  {
    // {DFA8191-6AD6-4A07-8F14-0D9B6AE3E75B}
    static const GUID SampleFindCrvsOnSrfCommand_UUID =
    { 0xDFA8191, 0x6AD6, 0x4A07, { 0x8F, 0x14, 0x0D, 0x9B, 0x6A, 0xE3, 0xE7, 0x5B } };
    return SampleFindCrvsOnSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleFindCrvsOnSrf"; }
  const wchar_t* LocalCommandName() { return L"SampleFindCrvsOnSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  bool IsCurveOnFace(const ON_BrepFace* face, const ON_Curve* crv, double tol, int num_points = 20);
};

// The one and only CCommandSampleFindCrvsOnSrf object
static class CCommandSampleFindCrvsOnSrf theSampleFindCrvsOnSrfCommand;

CRhinoCommand::result CCommandSampleFindCrvsOnSrf::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(true);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_BrepFace* face = go.Object(0).Face();
  if (0 == face)
    return CRhinoCommand::failure;

  ON_BoundingBox face_bbox = face->BoundingBox();
  double tol = context.m_doc.AbsoluteTolerance();
  bool bRedraw = false;

  const CRhinoObject* obj = 0;
  CRhinoObjectIterator it(CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_and_reference_objects);
  for (obj = it.First(); obj; obj = it.Next())
  {
    const CRhinoCurveObject* crv_obj = CRhinoCurveObject::Cast(obj);
    if (0 == crv_obj)
      continue;

    ON_BoundingBox crv_bbox = crv_obj->BoundingBox();
    if (!face_bbox.Includes(crv_bbox))
      continue;

    if (!IsCurveOnFace(face, crv_obj->Curve(), tol))
      continue;

    crv_obj->Select(true);
    bRedraw = true;
  }

  if (bRedraw)
    context.m_doc.Redraw();

  return CRhinoCommand::success;
}

bool CCommandSampleFindCrvsOnSrf::IsCurveOnFace(const ON_BrepFace* face, const ON_Curve* crv, double tol, int num_points)
{
  if (0 == face || 0 == crv)
    return false;

  int i;
  for (i = 0; i < num_points; i++)
  {
    double t = crv->Domain().ParameterAt(double(i) / num_points);
    ON_3dPoint p = crv->PointAt(t);
    double u = 0, v = 0;
    if (!face->GetClosestPoint(p, &u, &v, tol))
      break;
    if (!RhinoIsPointOnFace(*face, tol, false, u, v))
      break;
  }

  if (i < num_points)
    return false;

  return true;
}

//
// END SampleFindCrvsOnSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
