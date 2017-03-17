#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOrientOnCrv command
//

class COrientOnCrvXform : public CRhinoGetXform
{
public:
  COrientOnCrvXform();
  ~COrientOnCrvXform() {}

  void SetPathCurve(const ON_Curve* path_curve) { m_path_curve = path_curve; }
  void SetBasePlane(const ON_Plane& base_plane) { m_base_plane = base_plane; }
  void EnablePerpMode(BOOL perp_mode) { m_perp_mode = perp_mode; }

  BOOL CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform);
  void MakeNormalPlane(const ON_3dPoint origin, const ON_3dVector& normal, ON_Plane& plane);

private:
  const ON_Curve* m_path_curve;
  ON_Plane m_base_plane;
  BOOL m_perp_mode;
};

COrientOnCrvXform::COrientOnCrvXform()
{
  m_path_curve = NULL;
  m_perp_mode = FALSE;
}

BOOL COrientOnCrvXform::CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform)
{
  UNREFERENCED_PARAMETER(vp);

  BOOL bResult = FALSE;

  if (m_path_curve)
  {
    double t = 0.0;
    if (m_path_curve->GetClosestPoint(pt, &t))
    {
      ON_3dPoint origin = m_path_curve->PointAt(t);
      ON_Plane dest_plane;
      if (m_perp_mode)
      {
        ON_3dVector tangent = m_path_curve->TangentAt(t);
        MakeNormalPlane(origin, tangent, dest_plane);
      }
      else
      {
        dest_plane.origin = origin;
        dest_plane.xaxis = m_path_curve->TangentAt(t);
        dest_plane.zaxis = m_base_plane.zaxis;
        dest_plane.yaxis = ON_CrossProduct(dest_plane.zaxis, dest_plane.xaxis);
        dest_plane.UpdateEquation();
      }
      xform.Rotation(m_base_plane, dest_plane);
      bResult = xform.IsValid() ? TRUE : FALSE;
    }
  }

  return bResult;
}

void COrientOnCrvXform::MakeNormalPlane(const ON_3dPoint origin, const ON_3dVector& normal, ON_Plane& plane)
{
  plane.origin = origin;

  ON_3dVector up(normal.x, normal.y, normal.z + 1.0);
  plane.xaxis = ON_CrossProduct(up, normal);
  if (plane.xaxis.IsTiny())
  {
    if (normal.z < 0.0)
    {
      plane.xaxis = ON_3dVector(1.0, 0.0, 0.0);
      plane.yaxis = ON_3dVector(0.0, -1.0, 0.0);
    }
    else
    {
      plane.xaxis = ON_3dVector(1.0, 0.0, 0.0);
      plane.yaxis = ON_3dVector(0.0, 1.0, 0.0);
    }
  }
  else
  {
    plane.xaxis.Unitize();
    plane.yaxis = ON_CrossProduct(normal, plane.xaxis);
    plane.yaxis.Unitize();
  }

  plane.UpdateEquation();
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleOrientOnCrv : public CRhinoCommand
{
public:
  CCommandSampleOrientOnCrv() {}
  ~CCommandSampleOrientOnCrv() {}
  UUID CommandUUID()
  {
    // {B4DEE780-1D75-4598-9A21-A533FDE3DF38}
    static const GUID SampleOrientOnCrvCommand_UUID =
    { 0xB4DEE780, 0x1D75, 0x4598, { 0x9A, 0x21, 0xA5, 0x33, 0xFD, 0xE3, 0xDF, 0x38 } };
    return SampleOrientOnCrvCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleOrientOnCrv"; }
  const wchar_t* LocalCommandName() { return L"SampleOrientOnCrv"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CRhinoCommand::result SelectObjects(CRhinoXformObjectList& object_list);
  CRhinoCommand::result GetBasePlane(ON_Plane& base_plane);
  CRhinoCommand::result GetPathCurve(const ON_Curve*& path_curve);
  void TransformObjects(CRhinoDoc& doc, CRhinoXformObjectList& list, ON_Xform& xform, BOOL bCopy);

private:
  BOOL m_perp_mode;

};

// The one and only CCommandSampleOrientOnCrv object
static class CCommandSampleOrientOnCrv theSampleOrientOnCrvCommand;

CRhinoCommand::result CCommandSampleOrientOnCrv::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoCommand::result rc = CRhinoCommand::success;

  CRhinoXformObjectList object_list;
  rc = SelectObjects(object_list);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_Plane base_plane;
  rc = GetBasePlane(base_plane);
  if (rc != CRhinoCommand::success)
    return rc;

  const ON_Curve* path_curve = NULL;
  rc = GetPathCurve(path_curve);
  if (rc != CRhinoCommand::success)
    return rc;

  BOOL bCopy = FALSE;

  COrientOnCrvXform get;
  get.SetCommandPrompt(L"New base point on curve");
  get.AddCommandOptionToggle(RHCMDOPTNAME(L"Copy"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bCopy, &bCopy);
  get.AddCommandOptionToggle(RHCMDOPTNAME(L"Perpendicular"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), m_perp_mode, &m_perp_mode);
  get.AppendObjects(object_list);
  get.Constrain(*path_curve);
  get.AcceptNothing();
  get.SetPathCurve(path_curve);
  get.SetBasePlane(base_plane);

  for (;;)
  {
    get.EnablePerpMode(m_perp_mode);
    CRhinoGet::result res = get.GetXform();

    if (res == CRhinoGet::point)
    {
      ON_Xform xform;
      if (get.CalculateTransform(get.View()->Viewport(), get.Point(), xform))
      {
        TransformObjects(context.m_doc, object_list, xform, TRUE);
        context.m_doc.Redraw();
      }

      if (bCopy)
        continue;
      break;
    }

    else if (res == CRhinoGet::option)
      continue;
    else if (res == CRhinoGet::nothing)
      return CRhinoCommand::nothing;
    else
      return CRhinoCommand::cancel;
  }

  return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleOrientOnCrv::SelectObjects(CRhinoXformObjectList& object_list)
{
  object_list.Empty();

  CRhinoGetObject get;
  get.SetCommandPrompt(L"Select objects to orient");
  get.EnablePreSelect();
  get.EnableSubObjectSelect(FALSE);
  get.AcceptNothing();
  get.GetObjects(1, 0);

  CRhinoCommand::result rc = get.CommandResult();
  if (rc == CRhinoCommand::success)
  {
    if (object_list.AddObjects(get) < 1)
      rc = CRhinoCommand::failure;
  }

  return rc;
}

CRhinoCommand::result CCommandSampleOrientOnCrv::GetBasePlane(ON_Plane& base_plane)
{
  CRhinoGetPoint get;
  get.SetCommandPrompt(L"Base point");
  get.AcceptNothing();
  get.GetPoint();

  CRhinoCommand::result rc = get.CommandResult();
  if (rc == CRhinoCommand::success)
  {
    base_plane = get.View()->Viewport().ConstructionPlane().m_plane;
    base_plane.origin = get.Point();
    base_plane.UpdateEquation();
    if (!base_plane.IsValid())
      rc = CRhinoCommand::cancel;
  }

  return rc;
}

CRhinoCommand::result CCommandSampleOrientOnCrv::GetPathCurve(const ON_Curve*& path_curve)
{
  CRhinoGetObject get;
  get.SetCommandPrompt(L"Select path curve");
  get.SetGeometryFilter(CRhinoGetObject::curve_object);
  get.EnableDeselectAllBeforePostSelect(false);
  get.EnablePreSelect(FALSE);
  get.AcceptNothing();
  get.GetObjects(1, 1);

  CRhinoCommand::result rc = get.CommandResult();
  if (rc == CRhinoCommand::success)
  {
    CRhinoObjRef objRef = get.Object(0);
    path_curve = objRef.Curve();
    if (!path_curve)
      rc = CRhinoCommand::failure;
  }

  return rc;
}

void CCommandSampleOrientOnCrv::TransformObjects(CRhinoDoc& doc, CRhinoXformObjectList& list, ON_Xform& xform, BOOL bCopy)
{
  int num_failed = 0;
  CRhinoObject* transformed_object = NULL;

  for (int i = 0; i < list.m_objects.Count(); i++)
  {
    CRhinoObjRef obj_ref(list.m_objects[i]);
    transformed_object = doc.TransformObject(obj_ref, xform, bCopy ? false : true);
    if (!transformed_object)
      num_failed++;
  }

  if (num_failed > 0)
  {
    if (num_failed == 1)
      RhinoApp().Print(L"Unable to transform 1 object.\n");
    else
      RhinoApp().Print(L"Unable to transform %d objects.\n", num_failed);
  }
}

//
// END SampleOrientOnCrv command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
