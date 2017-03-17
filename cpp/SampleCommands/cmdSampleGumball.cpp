#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGumball command
//

class CSampleGumballGetXform : public CRhinoGetXform
{
public:
  CSampleGumballGetXform(CRhinoGumballDisplayConduit* dc);

  CRhinoGet::result MoveGumball();

  void OnMouseDown(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* p);
  void OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* p);
  void DynamicDraw(CRhinoDisplayPipeline& dp, const ON_3dPoint& pt);

  BOOL CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform);

private:
  CRhinoGumballDisplayConduit* m_dc;
};

CSampleGumballGetXform::CSampleGumballGetXform(CRhinoGumballDisplayConduit* dc)
  : m_dc(dc)
{
}

BOOL CSampleGumballGetXform::CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform)
{
  UNREFERENCED_PARAMETER(vp);
  UNREFERENCED_PARAMETER(pt);

  if (0 == m_dc)
    return FALSE;

  if (m_dc->m_drag_settings.m_bRelocateGumball)
    xform = m_dc->PreTransform();
  else
    xform = m_dc->TotalTransform();

  return xform.IsValid() ? TRUE : FALSE;
}

void CSampleGumballGetXform::OnMouseDown(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* p)
{
  UNREFERENCED_PARAMETER(flags);
  UNREFERENCED_PARAMETER(pt);

  if (0 == p || 0 == m_dc || m_dc->m_pick_result.m_gumball_mode != gb_mode_nothing)
    return;

  m_dc->m_pick_result.SetToDefaultPickResult();

  CRhinoPickContext pick_context;
  pick_context.m_view = vp.ParentView();
  pick_context.m_pick_style = CRhinoPickContext::point_pick;
  if (vp.SetClippingRegionTransformation(p->x, p->y, pick_context.m_pick_region))
  {
    vp.VP().GetFrustumLine(p->x, p->y, pick_context.m_pick_line);
    pick_context.UpdateClippingPlanes();
    m_dc->PickGumball(pick_context, this);
  }
}

void CSampleGumballGetXform::OnMouseMove(CRhinoViewport& vp, UINT flags, const ON_3dPoint& pt, const CPoint* p)
{
  if (0 == p || 0 == m_dc || m_dc->m_pick_result.m_gumball_mode == gb_mode_nothing)
    return;

  m_dc->CheckShiftAndControlKeys();

  ON_Line world_line;
  if (!vp.VP().GetFrustumLine(p->x, p->y, world_line))
    world_line = ON_Line(ON_UNSET_POINT, ON_UNSET_POINT);

  bool rc = m_dc->UpdateGumball(pt, world_line);
  if (rc)
    CRhinoGetXform::OnMouseMove(vp, flags, pt, p);
}

void CSampleGumballGetXform::DynamicDraw(CRhinoDisplayPipeline&, const ON_3dPoint&)
{
  // Disable default drawing by not calling the base class implementation.
  // All aspects of gumball display are handled by CRhinoGumballDisplayConduit
}

CRhinoGet::result CSampleGumballGetXform::MoveGumball()
{
  if (0 == m_dc)
    return CRhinoGet::cancel;

  if (!m_dc->PreTransform().IsIdentity())
  {
    m_bHaveXform = TRUE;
    m_xform = m_dc->PreTransform();
  }

  SetBasePoint(m_dc->BaseGumball().m_frame.m_plane.Origin(), false);

  const_cast<CRhinoXformObjectList&>(ObjectList()).EnableDisplayFeedback(true);
  if (!m_xform.IsIdentity())
    const_cast<CRhinoXformObjectList&>(ObjectList()).UpdateDisplayFeedbackXform(m_xform);

  SetGetPointCursor(RhinoApp().m_default_cursor);
  CRhinoGet::result res = GetPoint(0, true);

  const_cast<CRhinoXformObjectList&>(ObjectList()).EnableDisplayFeedback(false);

  return res;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleGumball : public CRhinoTransformCommand
{
public:
  CCommandSampleGumball() {}
  ~CCommandSampleGumball() {}
  UUID CommandUUID()
  {
    // {7C053725-678-48D2-A28B-DB52347079D4}
    static const GUID SampleGumballCommand_UUID =
    { 0x7C053725, 0x678, 0x48D2, { 0xA2, 0x8B, 0xDB, 0x52, 0x34, 0x70, 0x79, 0xD4 } };
    return SampleGumballCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGumball"; }
  const wchar_t* LocalCommandName() const { return L"SampleGumball"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  ON_BoundingBox GetBoundingBox(const CRhinoXformObjectList& list);

};

// The one and only CCommandSampleGumball object
static class CCommandSampleGumball theSampleGumballCommand;

CRhinoCommand::result CCommandSampleGumball::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoXformObjectList list;
  CRhinoCommand::result cmdrc = SelectObjects(L"Select objects", list);
  if (cmdrc != CRhinoCommand::success)
    return cmdrc;

  ON_BoundingBox bbox = GetBoundingBox(list);
  if (!bbox.IsValid())
    return CRhinoCommand::failure;

  CRhinoGumball gb;
  gb.SetFromBoundingBox(bbox);

  // Turn off rotation
  gb.m_appearance.m_bEnableXRotate = false;
  gb.m_appearance.m_bEnableYRotate = false;
  gb.m_appearance.m_bEnableZRotate = false;

  // Turn off scale
  gb.m_appearance.m_bEnableXScale = false;
  gb.m_appearance.m_bEnableYScale = false;
  gb.m_appearance.m_bEnableZScale = false;

  CRhinoGumballDisplayConduit dc;

  while (true)
  {
    dc.SetBaseGumball(gb);
    dc.EnableGumballDraw(true);
    dc.Enable(context.m_doc.RuntimeSerialNumber());
    context.m_doc.Redraw();

    CSampleGumballGetXform gp(&dc);
    if (dc.PreTransform().IsIdentity())
    {
      gp.SetCommandPrompt(L"Drag gumball");
      gp.AcceptNothing(FALSE);
    }
    else
    {
      gp.SetCommandPrompt(L"Drag gumball. Press Enter when done");
      gp.AcceptNothing(TRUE);
    }

    gp.AppendObjects(list);
    gp.MoveGumball();
    dc.EnableGumballDraw(false);
    dc.Disable();

    cmdrc = gp.CommandResult();
    if (cmdrc != CRhinoCommand::success)
      break;

    CRhinoGet::result getrc = gp.Result();
    if (getrc == CRhinoGet::point)
    {
      if (!dc.m_drag_settings.m_bRelocateGumball)
      {
        ON_Xform xform = dc.TotalTransform();
        dc.SetPreTransform(xform);
      }

      // update location of base gumball
      CRhinoGumballFrame dc_frame = dc.Gumball().m_frame;
      CRhinoGumballFrame gb_frame = gb.m_frame;
      gb_frame.m_plane = dc_frame.m_plane;
      gb_frame.m_scale_grip_distance = dc_frame.m_scale_grip_distance;
      gb.m_frame = gb_frame;
      continue;
    }

    break;
  }

  dc.EnableGumballDraw(false);
  dc.Disable();

  if (!dc.PreTransform().IsIdentity())
  {
    ON_Xform xform = dc.PreTransform();
    TransformObjects(context.m_doc, list, xform, false, false);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

ON_BoundingBox CCommandSampleGumball::GetBoundingBox(const CRhinoXformObjectList& list)
{
  ON_BoundingBox bbox;
  int i;

  for (i = 0; i < list.m_objects.Count(); i++)
  {
    const CRhinoObject* obj = list.m_objects[i];
    if (obj)
      bbox.Union(obj->BoundingBox());
  }

  for (i = 0; i < list.m_grips.Count(); i++)
  {
    const CRhinoObject* obj = list.m_grips[i];
    if (obj)
      bbox.Union(obj->BoundingBox());
  }

  return bbox;
}

//
// END SampleGumball command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
