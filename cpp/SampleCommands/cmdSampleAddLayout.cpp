#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAddLayout command
//

class CCommandSampleAddLayout : public CRhinoCommand
{
public:
  CCommandSampleAddLayout() {}
  ~CCommandSampleAddLayout() {}
  UUID CommandUUID()
  {
    // {4787C41-F962-4767-BC47-D43B93CF1A1E}
    static const GUID SampleAddLayoutCommand_UUID =
    { 0x4787C41, 0xF962, 0x4767, { 0xBC, 0x47, 0xD4, 0x3B, 0x93, 0xCF, 0x1A, 0x1E } };
    return SampleAddLayoutCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleAddLayout"; }
  const wchar_t* LocalCommandName() { return L"SampleAddLayout"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  CRhinoPageView* CreatePageView(CRhinoDoc& doc, int detail_count, const ON_wString& name, const ON_3dmPageSettings& page_settings);
  int NextPageViewNumber(CRhinoDoc& doc);
};

// The one and only CCommandSampleAddLayout object
static class CCommandSampleAddLayout theSampleAddLayoutCommand;

CRhinoCommand::result CCommandSampleAddLayout::RunCommand(const CRhinoCommandContext& context)
{
  AFX_MANAGE_STATE(RhinoApp().RhinoModuleState());

  ON_wString name;
  name.Format(L"Page %d", NextPageViewNumber(context.m_doc));

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"New Layout Name");
  gs.SetDefaultString(name);
  gs.GetString();
  if (CRhinoCommand::success != gs.CommandResult())
    return gs.CommandResult();

  int detail_count = 1;
  if (CRhinoCommand::success != RhinoGetInteger(L"Detail Count (0-4)", TRUE, detail_count, 0, 4))
    return CRhinoCommand::cancel;

  const double scale = ON::UnitScale(ON::LengthUnitSystem::Inches, ON::LengthUnitSystem::Millimeters);

  ON_3dmPageSettings page_settings;
  page_settings.m_width_mm = 11.0 * scale;
  page_settings.m_height_mm = 8.5 * scale;
  page_settings.m_page_number = 0;

  CRhinoPageView* page_view = CreatePageView(context.m_doc, detail_count, name, page_settings);
  if (nullptr == page_view)
    return CRhinoCommand::failure;

  RhinoApp().SetActiveView(page_view);
  page_view->Redraw();

  return CRhinoCommand::success;
}

CRhinoPageView* CCommandSampleAddLayout::CreatePageView(CRhinoDoc& doc, int detail_count, const ON_wString& name, const ON_3dmPageSettings& page_settings)
{
  if (page_settings.m_width_mm < ON_ZERO_TOLERANCE || page_settings.m_height_mm < ON_ZERO_TOLERANCE)
    return nullptr;

  CRhinoPageView* page_view = CRhinoPageView::FromRuntimeSerialNumber(doc.CreateRhinoPageView());
  if (nullptr == page_view)
    return nullptr;

  ON_3dmView view = page_view->MainViewport().View();
  view.m_name = name;
  view.m_page_settings = page_settings;
  page_view->MainViewport().SetView(view);
  RhinoDollyExtents(page_view);
  page_view->MainViewport().ClearUndoInformation();

  if (0 == detail_count)
    return page_view;

  double height_mm = page_settings.m_height_mm;
  double width_mm = page_settings.m_width_mm;

  double bottom = 10.0;
  double mid_height = height_mm / 2.0;
  double top = height_mm - 10.0;
  double left = 10.0;
  double mid_width = width_mm / 2.0;
  double right = width_mm - 10.0;
  ON_UnitSystem units_mm(ON::LengthUnitSystem::Millimeters);

  // Add top-left detail as a top view
  {
    ON_2dPoint corner1(left, top);
    ON_2dPoint corner2(mid_width, mid_height);
    if (1 == detail_count)
      corner2 = ON_2dPoint(right, bottom);
    else if (2 == detail_count || 3 == detail_count)
      corner2 = ON_2dPoint(right, mid_height);
    CRhinoDetailViewObject* detail_object = page_view->AddDetailView(corner1, corner2, &units_mm);
    if (detail_object)
    {
      detail_object->Viewport().m_v.m_bLockedProjection = false;
      detail_object->Viewport().SetToTopView(L"Top");
      detail_object->Viewport().m_v.m_vp.UnlockFrustumSymmetry();
      detail_object->DollyExtents();
    }
  }
  if (1 == detail_count)
    return page_view;

  // Add bottom-left detail as a front view
  {
    ON_2dPoint corner1(left, bottom);
    ON_2dPoint corner2(mid_width, mid_height);
    if (2 == detail_count)
    {
      corner1 = ON_2dPoint(left, mid_height);
      corner2 = ON_2dPoint(right, bottom);
    }

    CRhinoDetailViewObject* detail_object = page_view->AddDetailView(corner1, corner2, &units_mm);
    if (detail_object)
    {
      detail_object->Viewport().m_v.m_bLockedProjection = false;
      detail_object->Viewport().SetToFrontView(L"Front");
      detail_object->Viewport().m_v.m_vp.UnlockFrustumSymmetry();
      detail_object->DollyExtents();
    }
  }
  if (2 == detail_count)
    return page_view;

  // Add bottom-right detail as a right view
  {
    ON_2dPoint corner1(right, bottom);
    ON_2dPoint corner2(mid_width, mid_height);
    CRhinoDetailViewObject* detail_object = page_view->AddDetailView(corner1, corner2, &units_mm);
    if (detail_object)
    {
      detail_object->Viewport().m_v.m_bLockedProjection = false;
      detail_object->Viewport().SetToRightView(L"Right");
      detail_object->Viewport().m_v.m_vp.UnlockFrustumSymmetry();
      detail_object->DollyExtents();
    }
  }
  if (3 == detail_count)
    return page_view;

  // Add top-right detail as a perspective view
  {
    ON_2dPoint corner1(right, top);
    ON_2dPoint corner2(mid_width, mid_height);
    CRhinoDetailViewObject* detail_object = page_view->AddDetailView(corner1, corner2, &units_mm);
    if (detail_object)
    {
      detail_object->Viewport().m_v.m_bLockedProjection = false;
      detail_object->Viewport().SetToPerspectiveView(L"Perspective");
      detail_object->Viewport().m_v.m_vp.UnlockFrustumSymmetry();
      detail_object->DollyExtents();
    }
  }

  return page_view;
}

int CCommandSampleAddLayout::NextPageViewNumber(CRhinoDoc& doc)
{
  int rc = 1;
  ON_SimpleArray<CRhinoView*> view_list;
  doc.GetViewList(view_list, false, true);
  for (int i = 0; i <view_list.Count(); i++)
  {
    CRhinoView* view = view_list[i];
    if (view && view->RhinoViewType() == CRhinoView::page_view_type)
      rc++;
  }
  return rc;
}

//
// END SampleAddLayout command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
