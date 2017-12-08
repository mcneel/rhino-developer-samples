#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN CSampleGumballCylinder class
//

class CSampleGumballCylinder
{
public:
  CSampleGumballCylinder();
  CSampleGumballCylinder(ON_Plane plane, double radius, double height);

  bool Create(ON_Plane plane, double radius, double height);
  bool IsValid() const;

  ON_3dPoint Center() const;
  double Radius() const;
  bool SetRadius(double radius);
  double Height() const;
  bool SetHeight(double height);

  ON_Plane RadiusPlane();
  ON_Plane HeightPlane();

  double InitialRadius() const;
  double InitialHeight() const;

  void Draw(CRhinoDisplayPipeline& dp);

  ON_Brep BrepCylinder() const;

private:
  double m_initial_radius;
  double m_initial_height;
  ON_Circle m_circle;
  ON_Cylinder m_cylinder;
  ON_Brep m_brep;
  ON_Color m_color;
};

////////////////////////////////////////////////////////////////

CSampleGumballCylinder::CSampleGumballCylinder()
  : m_initial_radius(ON_UNSET_VALUE)
  , m_initial_height(ON_UNSET_VALUE)
  , m_color(ON_UNSET_COLOR)
{
  if (Create(ON_Plane::World_xy, 1.0, 1.0))
  {
    m_initial_radius = 1.0;
    m_initial_height = 1.0;
  }
}

CSampleGumballCylinder::CSampleGumballCylinder(ON_Plane plane, double radius, double height)
  : m_initial_radius(ON_UNSET_VALUE)
  , m_initial_height(ON_UNSET_VALUE)
  , m_color(ON_UNSET_COLOR)
{
  if (Create(plane, radius, height))
  {
    m_initial_radius = radius;
    m_initial_height = height;
  }
}

bool CSampleGumballCylinder::Create(ON_Plane plane, double radius, double height)
{
  bool rc = false;
  if (plane.IsValid() && radius > 0.0 && height > 0.0)
  {
    m_circle = ON_Circle(plane, radius);
    m_cylinder = ON_Cylinder(m_circle, height);
    rc = m_cylinder.IsValid();
    if (rc)
    {
      ON_BrepCylinder(m_cylinder, true, true, &m_brep);
      rc = m_brep.IsValid();
    }
  }
  return rc;
}

bool CSampleGumballCylinder::IsValid() const
{
  return m_circle.IsValid() && m_cylinder.IsValid() && m_brep.IsValid();
}

ON_3dPoint CSampleGumballCylinder::Center() const
{
  return m_circle.plane.origin;
}

double CSampleGumballCylinder::Radius() const
{
  return m_circle.radius;
}

bool CSampleGumballCylinder::SetRadius(double radius)
{
  return Create(m_circle.plane, radius, m_cylinder.Height());
}

double CSampleGumballCylinder::Height() const
{
  return m_cylinder.Height();
}

bool CSampleGumballCylinder::SetHeight(double height)
{
  return Create(m_circle.plane, m_circle.radius, height);
}

ON_Plane CSampleGumballCylinder::RadiusPlane()
{
  ON_3dPoint center = m_circle.PointAt(0.0);
  ON_3dVector xaxis = m_circle.TangentAt(0.0);
  xaxis.Unitize();
  ON_3dVector zaxis = m_circle.plane.zaxis;
  zaxis.Unitize();
  return ON_Plane(center, xaxis, zaxis);
}

ON_Plane CSampleGumballCylinder::HeightPlane()
{
  ON_3dPoint center = m_circle.plane.origin + m_cylinder.height[1] * m_circle.plane.zaxis;
  return ON_Plane(center, m_circle.plane.xaxis, m_circle.plane.yaxis);
}

ON_Brep CSampleGumballCylinder::BrepCylinder() const
{
  return m_brep;
}

void CSampleGumballCylinder::Draw(CRhinoDisplayPipeline& dp)
{
  if (m_color == ON_UNSET_COLOR)
    m_color = RhinoApp().AppSettings().AppearanceSettings().m_feedback_color;

  if (IsValid())
    dp.DrawBrep(m_brep, m_color);
}

double CSampleGumballCylinder::InitialRadius() const
{
  return m_initial_radius;
}

double CSampleGumballCylinder::InitialHeight() const
{
  return m_initial_height;
}

//
// END CSampleGumballCylinder class
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN CSampleGumballCylinderGetPoint class
//

class CSampleGumballCylinderGetPoint : public CRhinoGetPoint
{
public:
  CSampleGumballCylinderGetPoint(
    CSampleGumballCylinder& cylinder, 
    CRhinoGumballDisplayConduit& radius_dc, 
    CRhinoGumballDisplayConduit& height_dc
  );

  // Call instead of GetPoint()
  CRhinoGet::result MoveArrows();

  // OnMouseDown override
  void OnMouseDown(
    CRhinoViewport& vp, 
    UINT flags, 
    const ON_3dPoint& point, 
    const ON_2iPoint* 
    pick_point
  ) override;

  // OnMouseMove override
  void OnMouseMove(
    CRhinoViewport& vp, 
    UINT flags, 
    const ON_3dPoint& point, 
    const ON_2iPoint* pick_point
  ) override;

  // DynamicDraw override
  void DynamicDraw(
    CRhinoDisplayPipeline& dp, 
    const ON_3dPoint& point
  ) override;

private:
  CSampleGumballCylinder& m_cylinder;
  CRhinoGumballDisplayConduit& m_initial_radius_dc;
  CRhinoGumballDisplayConduit& m_initial_height_dc;
  ON_3dPoint m_basept;
  ON_3dPoint m_refpt;
};

////////////////////////////////////////////////////////////////

CSampleGumballCylinderGetPoint::CSampleGumballCylinderGetPoint(
  CSampleGumballCylinder& cylinder, 
  CRhinoGumballDisplayConduit& radius_dc, 
  CRhinoGumballDisplayConduit& height_dc
)
  : m_cylinder(cylinder)
  , m_initial_radius_dc(radius_dc)
  , m_initial_height_dc(height_dc)
  , m_basept(ON_UNSET_POINT)
  , m_refpt(ON_UNSET_POINT)
{
}

// OnMouseDown override
void CSampleGumballCylinderGetPoint::OnMouseDown(
  CRhinoViewport& vp, 
  UINT flags, 
  const ON_3dPoint& point, 
  const ON_2iPoint* pick_point
)
{
  m_basept = ON_UNSET_POINT;
  m_refpt = ON_UNSET_POINT;

  if (
    nullptr == pick_point ||
    m_initial_radius_dc.m_pick_result.m_gumball_mode != GUMBALL_MODE::gb_mode_nothing ||
    m_initial_height_dc.m_pick_result.m_gumball_mode != GUMBALL_MODE::gb_mode_nothing
    )
  {
    CRhinoGetPoint::OnMouseDown(vp, flags, point, pick_point);
    return;
  }

  // Setup a pick context
  CRhinoPickContext pick_context;
  pick_context.m_view = vp.ParentView();
  pick_context.m_pick_style = CRhinoPickContext::point_pick;
  vp.SetClippingRegionTransformation(pick_point->x, pick_point->y, pick_context.m_pick_region);
  vp.VP().GetFrustumLine(pick_point->x, pick_point->y, pick_context.m_pick_line);
  pick_context.m_bSetClippingPlanes = false;

  // Try picking one of the gumballs
    if (m_initial_radius_dc.PickGumball(pick_context, this))
  {
    m_basept = m_cylinder.RadiusPlane().origin;
    m_refpt = m_initial_radius_dc.m_pick_result.PickPoint(false);
  }
  else if (m_initial_height_dc.PickGumball(pick_context, this))
  {
    m_basept = m_cylinder.HeightPlane().origin;
    m_refpt = m_initial_height_dc.m_pick_result.PickPoint(false);
  }

  CRhinoGetPoint::OnMouseDown(vp, flags, point, pick_point);
}

// OnMouseMove override
void CSampleGumballCylinderGetPoint::OnMouseMove(
  CRhinoViewport& vp, 
  UINT flags, 
  const ON_3dPoint& point, 
  const ON_2iPoint* pick_point
)
{
  if (
    nullptr == pick_point ||
    !m_basept.IsValid() ||
    !m_refpt.IsValid()
    )
  {
    CRhinoGetPoint::OnMouseMove(vp, flags, point, pick_point);
    return;
  }

  ON_Line world_line;
  if (vp.VP().GetFrustumLine(pick_point->x, pick_point->y, world_line))
  {
    ON_3dVector v2 = point - m_basept;
    double len2 = v2.Length();

    ON_3dVector v1 = m_refpt - m_basept;
    double len1 = v1.Length();

    if (fabs(len2) >= ON_ZERO_TOLERANCE && fabs(len1) >= ON_ZERO_TOLERANCE)
    {
      double scale = len2 / len1;
      if (m_initial_radius_dc.m_pick_result.m_gumball_mode != GUMBALL_MODE::gb_mode_nothing)
      {
        m_initial_radius_dc.UpdateGumball(point, world_line);
        m_cylinder.SetRadius(m_cylinder.InitialRadius() * scale);
      }
      else if (m_initial_height_dc.m_pick_result.m_gumball_mode != GUMBALL_MODE::gb_mode_nothing)
      {
        m_initial_height_dc.UpdateGumball(point, world_line);
        m_cylinder.SetHeight(m_cylinder.InitialHeight() * scale);
      }
    }
  }

  CRhinoGetPoint::OnMouseMove(vp, flags, point, pick_point);
}

// DynamicDraw override
void CSampleGumballCylinderGetPoint::DynamicDraw(
  CRhinoDisplayPipeline& dp, 
  const ON_3dPoint& point
)
{
  m_cylinder.Draw(dp);

  // Disable default GetPoint drawing by not calling the base class
  // implementation. All aspects of gumball display are handled by 
  // CRhinoGumballDisplayConduit
}

CRhinoGet::result CSampleGumballCylinderGetPoint::MoveArrows()
{
  SetGetPointCursor(RhinoApp().m_arrow_command_cursor);
  return GetPoint(nullptr, true);
}

//
// END CSampleGumballCylinderGetPoint class
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGumballCylinder command
//

class CCommandSampleGumballCylinder : public CRhinoTestCommand
{
public:
  CCommandSampleGumballCylinder();
  ~CCommandSampleGumballCylinder() = default;
  UUID CommandUUID() override
  {
    // {40D9D926-1A0E-448E-B7BA-C1416BE049AF}
    static const GUID SampleGumballCylinderCommand_UUID =
    { 0x40D9D926, 0x1A0E, 0x448E,{ 0xB7, 0xBA, 0xC1, 0x41, 0x6B, 0xE0, 0x49, 0xAF } };
    return SampleGumballCylinderCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleGumballCylinder"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&) override;

private:
  CRhinoGumballAppearance GumballAppearanceSettings();

private:
  double m_initial_radius;
  double m_initial_height;
};

// The one and only CCommandSampleGumballCylinder object
static class CCommandSampleGumballCylinder theSampleGumballCylinderCommand;

CCommandSampleGumballCylinder::CCommandSampleGumballCylinder()
  : m_initial_radius(1.0)
  , m_initial_height(1.0)
{
}

CRhinoCommand::result CCommandSampleGumballCylinder::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Base point of cylinder");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  CRhinoView* view = gp.View();
  if (nullptr == view)
    return CRhinoCommand::failure;

  ON_3dPoint center = gp.Point();
  ON_Plane plane = view->ActiveViewport().ConstructionPlane().m_plane;
  plane.SetOrigin(center);

  CSampleGumballCylinder cylinder(plane, m_initial_radius, m_initial_height);

  CRhinoGumball radius_go;
  radius_go.m_appearance = GumballAppearanceSettings();

  CRhinoGumball height_go;
  height_go.m_appearance = GumballAppearanceSettings();

  CRhinoGumballDisplayConduit radius_dc;
  CRhinoGumballDisplayConduit height_dc;

  radius_dc.Enable();
  height_dc.Enable();

  while (true)
  {
    radius_go.SetFromPlane(cylinder.RadiusPlane());
    height_go.SetFromPlane(cylinder.HeightPlane());

    radius_dc.SetBaseGumball(radius_go);
    height_dc.SetBaseGumball(height_go);

    radius_dc.EnableGumballDraw(true);
    height_dc.EnableGumballDraw(true);

    CSampleGumballCylinderGetPoint gc(cylinder, radius_dc, height_dc);
    gc.SetCommandPrompt(L"Drag arrows. Press Enter when done");
    gc.AcceptNothing(TRUE);
    CRhinoGet::result res = gc.MoveArrows();

    radius_dc.EnableGumballDraw(false);
    height_dc.EnableGumballDraw(false);

    if (res == CRhinoGet::nothing)
    {
      m_initial_radius = cylinder.Radius();
      m_initial_height = cylinder.Height();
      if (cylinder.Create(plane, m_initial_radius, m_initial_height))
        context.m_doc.AddBrepObject(cylinder.BrepCylinder());
      break;
    }

    if (res != CRhinoGet::point)
      break;

    double radius = cylinder.Radius();
    double height = cylinder.Height();
    // Ths sets the initial radius and height
    cylinder = CSampleGumballCylinder(plane, radius, height);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

CRhinoGumballAppearance CCommandSampleGumballCylinder::GumballAppearanceSettings()
{
  CRhinoGumballAppearance ga;
  ga.m_bEnableRelocate = false;
  ga.m_bEnableMenu = false;
  ga.m_nEnableFreeTranslate = false;
  ga.m_bEnableXTranslate = false;
  ga.m_bEnableYTranslate = false;
  ga.m_bEnableZTranslate = true; // yay!
  ga.m_bEnableXYTranslate = false;
  ga.m_bEnableYZTranslate = false;
  ga.m_bEnableZXTranslate = false;
  ga.m_bEnableXScale = false;
  ga.m_bEnableYScale = false;
  ga.m_bEnableZScale = false;
  ga.m_bEnableXRotate = false;
  ga.m_bEnableYRotate = false;
  ga.m_bEnableZRotate = false;
  ga.m_bEnableXExtrude = false;
  ga.m_bEnableYExtrude = false;
  ga.m_bEnableZExtrude = false;
  return ga;
}

//
// END SampleGumballCylinder command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
