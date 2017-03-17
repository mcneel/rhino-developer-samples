#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSelFilter command
//

class CCommandSampleSelFilter : public CRhinoSelCommand
{
public:
  CCommandSampleSelFilter() { m_filter = 0; }
  ~CCommandSampleSelFilter() {}
  UUID CommandUUID()
  {
    // {7AD25042-A709-49DB-B9B1-11F7C28CB324}
    static const GUID SampleSelFilterCommand_UUID =
    { 0x7AD25042, 0xA709, 0x49DB, { 0xB9, 0xB1, 0x11, 0xF7, 0xC2, 0x8C, 0xB3, 0x24 } };
    return SampleSelFilterCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSelFilter"; }
  const wchar_t* LocalCommandName() { return L"SampleSelFilter"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CRhinoCommand::result GetFilter();
  bool SelFilter(const CRhinoObject*);

private:
  ON_SimpleArray<unsigned int> m_sel_window_filter_helper;
  unsigned int m_filter;
};

// The one and only CCommandSampleSelFilter object
static class CCommandSampleSelFilter theSampleSelFilterCommand;

static int compare_int(const unsigned int* a, const unsigned int* b)
{
  return (*b - *a);
}

CRhinoCommand::result CCommandSampleSelFilter::RunCommand(const CRhinoCommandContext& context)
{
  m_sel_window_filter_helper.SetCount(0);

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Drag a window to select objects");
  gp.AddCommandOption(RHCMDOPTNAME(L"Filter"));
  gp.SetGetPointCursor(RhinoApp().m_default_cursor);
  gp.ConstrainToTargetPlane();
  for (;;)
  {
    CRhinoGet::result res = gp.Get2dRectangle();

    if (res == CRhinoGet::option)
    {
      CRhinoCommand::result rc = GetFilter();
      if (rc != CRhinoCommand::success)
        return rc;

      continue;
    }

    else if (res == CRhinoGet::rect2d)
      break;

    return CRhinoCommand::cancel;
  }

  CRect pick_rect = gp.Rectangle2d();
  CRhinoView* view = gp.View();
  if (0 == view)
    return CRhinoCommand::failure;

  CRhinoPickContext pick_context;
  pick_context.m_go = 0;
  pick_context.m_view = view;
  pick_context.m_pick_style = CRhinoPickContext::window_pick;
  pick_context.m_bPickGroups = true;
  if (view->ActiveViewport().DisplayModeIsShaded())
    pick_context.m_pick_mode = CRhinoPickContext::shaded_pick;

  if (view->Viewport().SetClippingRegionTransformation(pick_rect, pick_context.m_pick_region))
  {
    pick_context.UpdateClippingPlanes();
    POINT screen_point = pick_rect.BottomRight();
    view->ActiveViewport().VP().GetFrustumLine(screen_point.x, screen_point.y, pick_context.m_pick_line);

    CRhinoObjRefArray pick_list;
    int i, pick_count = context.m_doc.PickObjects(pick_context, pick_list);

    m_sel_window_filter_helper.Reserve(pick_count);

    for (i = 0; i < pick_count; i++)
    {
      const CRhinoObject* obj = pick_list[i].Object();
      if (obj)
        m_sel_window_filter_helper.Append(obj->RuntimeSerialNumber());
    }

    if (m_sel_window_filter_helper.Count() > 0)
    {
      m_sel_window_filter_helper.HeapSort(compare_int);
      return CRhinoSelCommand::RunCommand(context); // call base class
    }
  }

  return CRhinoCommand::nothing;
}

CRhinoCommand::result CCommandSampleSelFilter::GetFilter()
{
  bool bPt = (m_filter == 0) ? true : ((m_filter && CRhinoObject::point_object) ? true : false);
  bool bSet = (m_filter == 0) ? true : ((m_filter && CRhinoObject::pointset_object) ? true : false);
  bool bCrv = (m_filter == 0) ? true : ((m_filter && CRhinoObject::surface_object) ? true : false);
  bool bSrf = (m_filter == 0) ? true : ((m_filter && CRhinoObject::curve_object) ? true : false);
  bool bBrep = (m_filter == 0) ? true : ((m_filter && CRhinoObject::polysrf_object) ? true : false);
  bool bMesh = (m_filter == 0) ? true : ((m_filter && CRhinoObject::mesh_object) ? true : false);
  bool bText = (m_filter == 0) ? true : ((m_filter && CRhinoObject::annotation_object) ? true : false);
  bool bLight = (m_filter == 0) ? true : ((m_filter && CRhinoObject::light_object) ? true : false);
  bool bHatch = (m_filter == 0) ? true : ((m_filter && CRhinoObject::hatch_object) ? true : false);

  CRhinoGetOption go;
  go.AcceptNothing();
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Points"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bPt, &bPt);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"PointClouds"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bSet, &bSet);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Curves"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bCrv, &bCrv);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Surfaces"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bSrf, &bSrf);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Polysurfaces"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bBrep, &bBrep);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Meshes"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bMesh, &bMesh);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Annotations"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bText, &bText);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Lights"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bLight, &bLight);
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Hatches"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bHatch, &bHatch);
  for (;;)
  {
    CRhinoGet::result res = go.GetOption();
    if (res == CRhinoGet::option)
      continue;
    else if (res == CRhinoGet::nothing)
      break;

    return CRhinoCommand::cancel;
  }

  m_filter = 0;
  if (bPt)    m_filter |= CRhinoObject::point_object;
  if (bSet)   m_filter |= CRhinoObject::pointset_object;
  if (bCrv)   m_filter |= CRhinoObject::curve_object;
  if (bSrf)   m_filter |= CRhinoObject::surface_object;
  if (bBrep)  m_filter |= CRhinoObject::polysrf_object;
  if (bMesh)  m_filter |= CRhinoObject::mesh_object;
  if (bText)  m_filter |= CRhinoObject::annotation_object;
  if (bLight) m_filter |= CRhinoObject::light_object;
  if (bHatch) m_filter |= CRhinoObject::hatch_object;

  return CRhinoCommand::success;
}

bool CCommandSampleSelFilter::SelFilter(const CRhinoObject* object)
{
  bool rc = false;
  if (object)
  {
    unsigned int u = object->RuntimeSerialNumber();
    if (m_sel_window_filter_helper.BinarySearch(&u, compare_int) >= 0)
    {
      if (m_filter == 0)
        rc = true;
      else if (object->ObjectType() & m_filter)
        rc = true;
    }
  }
  return rc;
}

//
// END SampleSelFilter command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
