#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimRadius command
//

class CSampleGetDimRadialPoint : public CRhinoGetPoint
{
public:
  CSampleGetDimRadialPoint();
  ~CSampleGetDimRadialPoint();

  // CRhinoGetPoint overrrides
  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& drag_point, const CPoint* screen_point);
  void DynamicDraw(HDC hdc, CRhinoViewport& vp, const ON_3dPoint& drag_point);

  // Zeros the pointer here so the dimension object won't be deleted 
  // when this class is deleted.
  CRhinoRadialDimension* DetachDim();

  void SetCurve(const ON_Curve* curve);
  void SetPlane(const ON_Plane& plane);
  void SetDistanceScale(double model_scale);
  void SetMode(int mode); // radius or diameter

  void LockPoint(ON_3dPoint point);
  bool Locked() const;

private:
  // Dimension to update at runtime
  CRhinoRadialDimension* m_dim_object; 

  const ON_Curve* m_curve;
  bool m_bCurvePlanar;
  ON_Plane m_curve_plane;

  ON_NurbsCurve* m_flat_curve;
  ON_Plane m_last_plane;

  ON_3dPoint m_drag_point;
  bool m_bLocked;

  int m_mode; // radius or diameter
};

CSampleGetDimRadialPoint::CSampleGetDimRadialPoint()
{
  m_dim_object = new CRhinoRadialDimension();
  m_dim_object->m_radial_dimension.m_type = ON::dtDimRadius;
  m_dim_object->m_radial_dimension.m_plane = ON_xy_plane;
  m_last_plane = ON_xy_plane;
  m_mode = ON::dtDimDiameter;
  m_flat_curve = 0;
  m_curve = 0;
  m_bCurvePlanar = false;
  m_bLocked = false;
}

CSampleGetDimRadialPoint::~CSampleGetDimRadialPoint()
{
  if (0 != m_dim_object)
  {
    delete m_dim_object;
    m_dim_object = 0;
  }
  if (0 != m_flat_curve)
  {
    delete m_flat_curve;
    m_flat_curve = 0;
  }
}

void CSampleGetDimRadialPoint::SetMode(int mode)
{
  if (0 != m_dim_object)
  {
    ON_RadialDimension2& radial_dimension = m_dim_object->m_radial_dimension;
    if (mode == ON::dtDimRadius)
    {
      m_dim_object->SetUserText(radial_dimension.DefaultRadiusText());
      m_mode = mode;
      radial_dimension.m_type = ON::dtDimRadius;
    }
    else if (mode == ON::dtDimDiameter)
    {
      m_dim_object->SetUserText(radial_dimension.DefaultDiameterText());
      m_mode = mode;
      radial_dimension.m_type = ON::dtDimDiameter;
    }
  }
}

CRhinoRadialDimension* CSampleGetDimRadialPoint::DetachDim()
{
  CRhinoRadialDimension* rc = m_dim_object;
  m_dim_object = 0;
  return rc;
}

void CSampleGetDimRadialPoint::SetPlane(const ON_Plane& plane)
{
  if (0 != m_dim_object)
    m_dim_object->SetPlane(plane);
}

void CSampleGetDimRadialPoint::SetDistanceScale(double model_scale)
{
  if (0 != m_dim_object)
    m_dim_object->SetDistanceScale(model_scale);
}

void CSampleGetDimRadialPoint::SetCurve(const ON_Curve* curve)
{
  if (0 != curve)
  {
    m_curve = curve;
    if (m_curve->IsPlanar(&m_curve_plane))
    {
      m_bCurvePlanar = true;
    }
    else
    {
      m_bCurvePlanar = false;
      ClearConstraints();
    }
  }
}

void CSampleGetDimRadialPoint::LockPoint(ON_3dPoint p)
{
  m_drag_point = p;
  m_bLocked = true;
}

bool CSampleGetDimRadialPoint::Locked() const
{
  return m_bLocked;
}

bool ComparePlanes(const ON_Plane& p0, const ON_Plane& p1)
{
  ON_3dVector v = p0.Origin() - p1.Origin();
  if (!v.IsTiny())
    return false;
  v = p0.Xaxis() - p1.Xaxis();
  if (!v.IsTiny())
    return false;
  v = p0.Yaxis() - p1.Yaxis();
  if (!v.IsTiny())
    return false;
  return true;
}

void CSampleGetDimRadialPoint::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& drag_point, const CPoint* screen_point)
{
  if (0 == m_dim_object || 0 == m_curve)
    return;

  ON_Plane plane = vp.ConstructionPlane().m_plane;

  if (m_bCurvePlanar && m_curve_plane.zaxis.IsParallelTo(plane.zaxis))
  {
    plane.origin = m_curve_plane.origin;
    Constrain(plane);
    if (!ComparePlanes(m_last_plane, plane) || 0 == m_flat_curve)
    {
      if (0 != m_flat_curve)
      {
        delete m_flat_curve;
        m_flat_curve = 0;
      }

      m_flat_curve = static_cast<ON_NurbsCurve*>(m_curve->NurbsCurve());
      m_last_plane = plane;
    }
  }
  else // Not planar or not parallel to the construction plane
  {
    ClearConstraints();

    if (!ComparePlanes(m_last_plane, plane))
    {
      if (0 != m_flat_curve)
      {
        delete m_flat_curve;
        m_flat_curve = 0;
      }
      m_last_plane = plane;
    }

    if (0 == m_flat_curve)
    {
      m_flat_curve = ON_NurbsCurve::New();
      m_curve->GetNurbForm(*m_flat_curve);

      ON_Xform xf;
      xf.PlanarProjection(plane);
      m_flat_curve->Transform(xf);
    }
  }

  if (0 == m_flat_curve)
    return;


  ON_3dPoint dp = drag_point;
  ON_2dPoint p2, pd;
  ON_3dPoint p, pp, pc;

  // This loop is for the case when the drag point is farther from the curve 
  // than the radius being dimensioned and the intention is to run the leader
  // from the curve through the center point and beyond.
  for (int i = 0; i < 2; i++)
  {
    if (m_bLocked)
      dp = m_drag_point;

    double t = 0.0, tn = 0.0;
    if (!m_flat_curve->GetClosestPoint(dp, &tn))
      return;

    // Flat curve is a nurb form of curve, projected to cplane
    if (!m_flat_curve->GetCurveParameterFromNurbFormParameter(tn, &t))
      return;

    // Point on the original curve
    p = m_curve->PointAt(t);        // arrow point on original 3d curve
    // Projected to plane
    pp = m_flat_curve->PointAt(tn); // projected arrow point on 2d curve
    // Make plane go through point on curve
    plane.Translate(p - pp);

    ON_3dVector kappa = m_flat_curve->CurvatureAt(tn);
    double len = kappa.Length();
    if (len < ON_SQRT_EPSILON || len > 1e100)
    {
      kappa.Set(0.0, 0.0, 0.0);
      len = 0.0;
    }

    // Center point at end of radius vector
    if (len != 0.0)
    {
      kappa.Unitize();
      kappa *= 1.0 / len;
    }

    pc = plane.ClosestPointTo(pp + kappa);

    double d0 = p.DistanceTo(drag_point);
    double dc = p.DistanceTo(pc);
    if (d0 > dc && (p - pc) * (p - drag_point) > 0.0)
      dp = pc + (pc - drag_point);
    else
      break;
  }

  plane.SetOrigin(pc);
  plane.UpdateEquation();

  // Re-project the point on curve 
  plane.ClosestPointTo(p, &p2.x, &p2.y);          // Arrow point on dim plane
  plane.ClosestPointTo(drag_point, &pd.x, &pd.y); // Dragged (knee) point on dim plane
  
  // Tune up knee - arrow direction goes through center
  ON_2dVector vk = pd;
  double kd = vk.Length();
  vk.Unitize();
  ON_2dVector va = p2;
  va.Unitize();
  double sign = 1.0;
  if (vk * va < 0.0)
    sign = -1.0;
  pd = va * kd * sign;

  m_dim_object->SetPlane(plane);
  m_dim_object->UpdateDimPoints(p2, pd);
  m_dim_object->UpdateText();

  CRhinoGetPoint::OnMouseMove(vp, flags, drag_point, screen_point);
}

void CSampleGetDimRadialPoint::DynamicDraw(HDC hdc, CRhinoViewport& vp, const ON_3dPoint& drag_point)
{
  if (0 != m_dim_object && 0 != vp.m_dp)
    m_dim_object->Draw(*vp.m_dp);

  CRhinoGetPoint::DynamicDraw(hdc, vp, drag_point);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

CRhinoCommand::result SampleGetDimRadial(
        CRhinoDoc& doc, 
        ON::eAnnotationType RadiusOrDiameter, 
        bool bInteractive, 
        CRhinoObjRef& point_ref, 
        CRhinoRadialDimension*& output_dimension
        )
{
  CRhinoGet::result rc = CRhinoGet::cancel;
  CRhinoObjRef object_ref;
  ON_Curve* dim_curve = 0;

  int style_index = 0;
  int count = 0;
  bool bHaveObject = false;
  double model_scale = 1.0;
  ON_Xform world_to_page(1.0);

  while (count < 100 && !bHaveObject)
  {
    CRhinoGetObject go;

    if (ON::dtDimDiameter == RadiusOrDiameter)
      go.SetCommandPrompt(L"Select curve for diameter dimension");
    else
      go.SetCommandPrompt(L"Select curve for radius dimension");

    go.SetGeometryFilter(ON::curve_object);
    
    ON_wString style_name = doc.m_dimstyle_table.CurrentDimStyle().Name();
    CRhinoCommandOptionValue style_option(style_name, style_name);
    go.AddCommandOption(RHCMDOPTNAME(L"Style"), style_option);

    go.EnableInactiveDetailPick();
    
    rc = go.GetObjects(1, 1);
    
    switch( rc)
    {
    case CRhinoGet::object:
      {
        object_ref = go.Object(0);

        const ON_Curve* curve = object_ref.Curve();
        if (0 == curve)
          return CRhinoCommand::failure;

        // If the curve was selected in an inactive detail,
        // get the detail scale and transformation to paper coordinates
        unsigned int serial_no = object_ref.SelectionViewDetailSerialNumber();
        if (serial_no > 0)
        {
          const CRhinoDetailViewObject* detail_object = CRhinoDetailViewObject::Cast(doc.LookupObjectByRuntimeSerialNumber(serial_no));
          if (0 != detail_object && detail_object->Viewport().VP().Projection() == ON::parallel_view)
          {
            detail_object->GetModelLength(1.0, model_scale);
            detail_object->GetPageXforms(&world_to_page, 0);
          }
        }

        // If an arc is picked, isolate the arc and only dimension to it.  
        // If a free-form curve is picked, allow sliding along different radii.
        if (1 == object_ref.SelectionMethod())
        {
          double t = 0.0;
          if (curve == object_ref.CurveParameter(&t))
          {
            ON_Arc arc;
            if (curve->IsArcAt(t, 0, &arc))
              dim_curve = new ON_ArcCurve(arc);
          }
        }

        if (curve->IsLinear() || curve->IsPolyline())
        {
          RhinoApp().Print(L"Curve cannot be linear.\n");
          object_ref.Object()->Select(false);
          curve = 0;
          doc.Redraw();
        }
        else
        {
          if (0 == dim_curve)
            dim_curve = curve->DuplicateCurve();
          if (0 != dim_curve)
          {
            if (model_scale != 0.0)
              dim_curve->Transform(world_to_page);
            bHaveObject = true;
          }
        }
      }
      break;

    case CRhinoGet::option:
      style_index = RhinoSelectDimstyle(doc, bInteractive);
      if (style_index >= 0)
        doc.m_dimstyle_table.SetCurrentDimStyleIndex(style_index);
      break;

    default:
      return CRhinoCommand::cancel;
    }
  }  

  if (0 == dim_curve)
    return CRhinoCommand::failure;


  CRhinoCommand::result cmd_rc = CRhinoCommand::failure;

  CSampleGetDimRadialPoint gp;
  
  bool bDone = false;
  while (!bDone)
  {
    if (model_scale != 1.0)
      gp.SetDistanceScale(model_scale);

    gp.ClearCommandOptions();
    gp.SetCommandPrompt(L"Dimension location");
    gp.SetMode(RadiusOrDiameter);
    gp.SetCurve(dim_curve);

    CRhinoView* view = RhinoApp().ActiveView();
    if (0 != view)
      gp.SetPlane(view->Viewport().ConstructionPlane().m_plane);

    int opt_poc = gp.Locked() ? -1 : gp.AddCommandOption(RHCMDOPTNAME(L"PointOnCurve"));

    rc = gp.GetPoint();

    switch (rc)
    {
    case CRhinoGet::point:
      {
        gp.OnMouseMove(gp.View()->Viewport(), 0, gp.Point(), 0);
        output_dimension = gp.DetachDim();
        point_ref = object_ref;
        cmd_rc = CRhinoCommand::success;
      }
      bDone = true;
      break;

    case CRhinoGet::option:
      {
        const CRhinoCommandOption* option = gp.Option();
        if (0 != option && option->m_option_index == opt_poc)
        {
          CRhinoGetPoint gpoc;
          gpoc.SetCommandPrompt(L"Point on curve");
          gpoc.Constrain(*dim_curve);
          if (gpoc.GetPoint() != gpoc.point)
          {
            cmd_rc = CRhinoCommand::cancel;
            break;
          }        

          double t = 0.0;
          const ON_Curve* curve = gpoc.PointOnCurve(&t);
          if (0 != curve && (curve->PointAt(t)-gpoc.Point()).IsTiny())
            gp.LockPoint(curve->PointAt(t));
        }
      }
      break;

    case CRhinoGet::cancel:
      cmd_rc = CRhinoCommand::cancel;
      bDone = true;
      break;
    }
  }

  if (0 != dim_curve)
    delete dim_curve; // don't leak...

  return cmd_rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleDimRadius : public CRhinoCommand
{
public:
  CCommandSampleDimRadius() {}
  ~CCommandSampleDimRadius() {}
  UUID CommandUUID()
  {
    // {53F63164-B4A6-4C5E-AA47-A4847569B5F1}
    static const GUID SampleDimRadiusCommand_UUID =
    { 0x53F63164, 0xB4A6, 0x4C5E, { 0xAA, 0x47, 0xA4, 0x84, 0x75, 0x69, 0xB5, 0xF1 } };
    return SampleDimRadiusCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDimRadius"; }
  const wchar_t* LocalCommandName() const { return L"SampleDimRadius"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

private:
  int GetFrontDisplayOrder(CRhinoDoc& doc);
};

// The one and only CCommandSampleDimRadius object
static class CCommandSampleDimRadius theSampleDimRadiusCommand;

CRhinoCommand::result CCommandSampleDimRadius::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoObjRef point_ref;
  CRhinoRadialDimension* dim_object = 0;
  CRhinoCommand::result rc = SampleGetDimRadial(context.m_doc, ON::dtDimRadius, context.IsInteractive(), point_ref, dim_object);

  if (rc == CRhinoCommand::success && 0 != dim_object)
  {
    ON_3dmObjectAttributes attributes;
    context.m_doc.GetDefaultObjectAttributes(attributes);

    ON_UUID uuid;
    ON_CreateUuid(uuid);
    attributes.m_uuid = uuid;
    
    // If text mask, bring to front
    if (dim_object->Style().DrawTextMask())
    {
      int display_order = GetFrontDisplayOrder(context.m_doc);
      attributes.m_display_order = display_order + 1;
    }

    dim_object->ModifyAttributes(attributes);

    context.m_doc.AddObject(dim_object);
    context.m_doc.Redraw();
  }

  return rc;
}

int CCommandSampleDimRadius::GetFrontDisplayOrder(CRhinoDoc& doc)
{
  CRhinoObjectIterator it(doc);
  int rc = 0;
  for (CRhinoObject* obj = it.First(); 0 != obj; obj = it.Next())
  {
    int display_order = obj->Attributes().m_display_order;
    if (display_order > rc)
      rc = display_order;
  }
  return rc;
}

//
// END SampleDimRadius command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
