#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleFindCrvOnSrf command
//

#pragma region SampleFindCrvOnSrf command

class CCommandSampleFindCrvOnSrf : public CRhinoCommand
{
public:
  CCommandSampleFindCrvOnSrf() {}
  ~CCommandSampleFindCrvOnSrf() {}
  UUID CommandUUID()
  {
    // {986B4A56-C2CB-4FCC-ADBB-C68E0BB4C6F1}
    static const GUID SampleFindCrvOnSrfCommand_UUID =
    { 0x986B4A56, 0xC2CB, 0x4FCC,{ 0xAD, 0xBB, 0xC6, 0x8E, 0x0B, 0xB4, 0xC6, 0xF1 } };
    return SampleFindCrvOnSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleFindCrvOnSrf"; }
  const wchar_t* LocalCommandName() const { return L"SampleFindCrvOnSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  bool IsCurveOnFace(const ON_BrepFace* face, const ON_Curve* crv, double tol, int num_points = 20);
};

// The one and only CCommandSampleFindCrvOnSrf object
static class CCommandSampleFindCrvOnSrf theSampleFindCrvOnSrfCommand;

CRhinoCommand::result CCommandSampleFindCrvOnSrf::RunCommand(const CRhinoCommandContext& context)
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

bool CCommandSampleFindCrvOnSrf::IsCurveOnFace(const ON_BrepFace* face, const ON_Curve* crv, double tol, int num_points)
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

#pragma endregion

//
// END SampleFindCrvOnSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
