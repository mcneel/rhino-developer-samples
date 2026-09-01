#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMake2D command
//

#pragma region SampleMake2D command

class CCommandSampleMake2D : public CRhinoCommand
{
public:
  CCommandSampleMake2D() = default;
  ~CCommandSampleMake2D() = default;
  UUID CommandUUID() override
  {
    // {7B3D91A6-4C25-4E8B-9F0E-2D8A61C0F3B7}
    static const GUID SampleMake2DCommand_UUID =
    { 0x7B3D91A6, 0x4C25, 0x4E8B, { 0x9F, 0x0E, 0x2D, 0x8A, 0x61, 0xC0, 0xF3, 0xB7 } };
    return SampleMake2DCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleMake2D"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleMake2D object
static class CCommandSampleMake2D theSampleMake2DCommand;

CRhinoCommand::result CCommandSampleMake2D::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  // Select the objects to include in the hidden line drawing
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to test Make2D");
  go.SetGeometryFilter(
    CRhinoGetObject::point_object
    | CRhinoGetObject::pointset_object
    | CRhinoGetObject::curve_object
    | CRhinoGetObject::surface_object
    | CRhinoGetObject::polysrf_object
    | CRhinoGetObject::extrusion_object
    | CRhinoGetObject::mesh_object
    | CRhinoGetObject::subd_object
  );
  go.EnableGroupSelect(TRUE);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoView* view = RhinoApp().ActiveView();
  if (nullptr == view)
    return CRhinoCommand::failure;

  // The viewport supplies the projection that determines curve visibility
  ON_HiddenLineDrawing* hld = new ON_HiddenLineDrawing(doc->AbsoluteTolerance());
  if (!hld->SetViewport(view->ActiveViewport().VP()))
  {
    delete hld;
    return CRhinoCommand::failure;
  }

  hld->IncludeTangentEdges(false);
  hld->IncludeHiddenCurves(true);

  // Add the geometry. ON_HiddenLineDrawing supports ON_Brep, ON_Curve,
  // ON_Mesh, ON_Point and ON_PointCloud. Convert extrusions and SubDs to breps.
  ON_SimpleArray<ON_Brep*> converted_breps;
  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObject* obj = go.Object(i).Object();
    if (nullptr == obj || nullptr == obj->Geometry())
      continue;

    const ON_Geometry* geom = obj->Geometry();

    ON_Brep* converted_brep = nullptr;
    const ON_Extrusion* extrusion = ON_Extrusion::Cast(geom);
    const ON_SubD* subd = ON_SubD::Cast(geom);
    if (nullptr != extrusion)
      converted_brep = extrusion->BrepForm();
    else if (nullptr != subd)
      converted_brep = subd->GetSurfaceBrep(ON_SubDToBrepParameters::DefaultUnpacked, nullptr);

    if (nullptr != converted_brep)
    {
      converted_breps.Append(converted_brep);
      hld->AddObject(converted_brep, nullptr, obj->Id(), 0);
    }
    else if (nullptr == extrusion && nullptr == subd)
      hld->AddObject(geom, nullptr, obj->Id(), 0);
  }

  // Compute the hidden line drawing
  CWaitCursor wait;
  const bool draw_rc = hld->Draw(true, nullptr, nullptr);
  wait.Restore();
  if (!draw_rc)
  {
    RhinoApp().Print(L"Hidden line drawing failed.\n");
    delete hld;
    hld = nullptr;
    for (int i = 0; i < converted_breps.Count(); i++)
    {
      delete converted_breps[i];
      converted_breps[i] = nullptr;
    }
    return CRhinoCommand::failure;
  }

  // The results are in HLD coordinates. Flatten onto the world x-y plane
  // and move the drawing so its lower-left corner lands at the origin.
  ON_Xform flatten;
  flatten.PlanarProjection(ON_xy_plane);
  const ON_BoundingBox page_box = hld->GetBoundingBox(true);
  if (page_box.IsValid())
  {
    const ON_Xform translate = ON_Xform::TranslationTransformation(ON_3dVector(-page_box.m_min.x, -page_box.m_min.y, 0.0));
    flatten = translate * flatten;
  }

  ON_3dmObjectAttributes v_attribs, h_attribs;
  doc->GetDefaultObjectAttributes(v_attribs);
  v_attribs.m_name = L"V";
  doc->GetDefaultObjectAttributes(h_attribs);
  h_attribs.m_name = L"H";

  // Add the visible ("V") and hidden ("H") curve segments to the document
  for (int i = 0; i < hld->m_curve.Count(); i++)
  {
    const ON_HLDCurve* hld_curve = hld->m_curve[i];
    if (nullptr == hld_curve || nullptr == hld_curve->FullCurve())
      continue;

    if (ON_SIL_EVENT::kNoSilEvent == hld_curve->FullCurve()->m_type)
      continue;

    // ON_HLDCurve is an ON_CurveProxy; duplicate to get a real curve
    ON_Curve* crv = hld_curve->DuplicateCurve();
    if (nullptr == crv)
      continue;

    crv->Transform(flatten);

    if (ON_HLDCurve::VISIBILITY::kVisible == hld_curve->m_vis)
    {
      CRhinoCurveObject* crv_obj = new CRhinoCurveObject(v_attribs);
      crv_obj->SetCurve(crv);
      doc->AddObject(crv_obj);
    }
    else if (ON_HLDCurve::VISIBILITY::kHidden == hld_curve->m_vis)
    {
      CRhinoCurveObject* crv_obj = new CRhinoCurveObject(h_attribs);
      crv_obj->SetCurve(crv);
      doc->AddObject(crv_obj);
    }
    else
    {
      delete crv;
      crv = nullptr;
    }
  }

  // Add the points, generated by point and point cloud input
  const ON_SimpleArray<const ON_HLDPoint*>& hld_points = hld->GetHLDPoints();
  for (int i = 0; i < hld_points.Count(); i++)
  {
    const ON_HLDPoint* hld_pt = hld_points[i];
    if (nullptr == hld_pt || !hld_pt->IsValid())
      continue;

    ON_3dPoint pt = *hld_pt;
    pt.Transform(flatten);

    if (ON_HLDPoint::VISIBILITY::kVisible == hld_pt->m_vis)
    {
      CRhinoPointObject* pt_obj = new CRhinoPointObject(v_attribs);
      pt_obj->SetPoint(pt);
      doc->AddObject(pt_obj);
    }
    else if (ON_HLDPoint::VISIBILITY::kHidden == hld_pt->m_vis)
    {
      CRhinoPointObject* pt_obj = new CRhinoPointObject(h_attribs);
      pt_obj->SetPoint(pt);
      doc->AddObject(pt_obj);
    }
  }

  // Delete the drawing before the converted breps it references
  delete hld;
  hld = nullptr;
  for (int i = 0; i < converted_breps.Count(); i++)
  {
    delete converted_breps[i];
    converted_breps[i] = nullptr;
  }

  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMake2D command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
