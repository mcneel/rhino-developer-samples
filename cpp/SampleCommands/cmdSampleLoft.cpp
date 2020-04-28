#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLoft command
//

class CCommandSampleLoft : public CRhinoCommand
{
public:
  CCommandSampleLoft();
  UUID CommandUUID() override
  {
    // {EB83F47-D94A-4F5B-95F6-743C4C1D830B}
    static const GUID SampleLoftCommand_UUID =
    { 0xEB83F47, 0xD94A, 0x4F5B, { 0x95, 0xF6, 0x74, 0x3C, 0x4C, 0x1D, 0x83, 0x0B } };
    return SampleLoftCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleLoft"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
  CRhinoObject* ReplayHistory(const CRhinoHistoryRecord& history_record) override;

private:
  void ProcessInput(
    CArgsRhinoLoft& args, 
    const CRhinoObjRef& ref, 
    ON_ClassArray<CRhinoObjRef>* in_curves
  );

  CRhinoCommand::result CreateOutput(
    CRhinoDoc& doc, 
    CArgsRhinoLoft& args, 
    bool bInReplayHistory, 
    ON_SimpleArray<ON_Brep*>& out_breps
  );

  bool WriteHistory(
    CRhinoHistory& history, 
    const ON_ClassArray<CRhinoObjRef>& in_curves, 
    bool bSplitCreasedSurfaces
  );

  bool ReadHistory(
    const CRhinoHistoryRecord& history_record, 
    ON_ClassArray<CRhinoObjRef>& in_curves,
    bool& bSplitCreasedSurfaces
  );

private:
  const int m_history_version;
};

// The one and only CCommandSampleLoft object
static class CCommandSampleLoft theSampleLoftCommand;

CCommandSampleLoft::CCommandSampleLoft()
  :m_history_version(20200221)
{
}

CRhinoCommand::result CCommandSampleLoft::RunCommand(const CRhinoCommandContext& context)
{
  // Select curves to loft
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curves to loft");
  go.SetGeometryFilter(CRhinoGetObject::curve_object | CRhinoGetObject::edge_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::open_curve);
  go.EnablePreSelect(false);
  go.GetObjects(2, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Create loft arguments object
  const int crv_count = go.ObjectCount();
  if (crv_count < 2)
    return CRhinoCommand::failure;

  CArgsRhinoLoft args;
  args.m_loftcurves.SetCapacity(crv_count);

  // For history
  ON_ClassArray<CRhinoObjRef> in_curves(32);

  // Add curves to loft arguments object
  for (int i = 0; i < crv_count; i++)
  {
    const CRhinoObjRef& ref = go.Object(i);
    ProcessInput(args, ref, &in_curves);
  }

  // If we do not have enough loft curves,
  // clean up and bail.
  const int lc_count = args.m_loftcurves.Count();
  if (lc_count < 2)
  {
    for (int i = 0; i < lc_count; i++)
      delete args.m_loftcurves[i];
    return CRhinoCommand::failure;
  }

  // If the starting loft curve has a trim, 
  // set the start condition to tangent.
  if (args.m_loftcurves[0] && args.m_loftcurves[0]->m_trim)
  {
    args.m_start_condition = CArgsRhinoLoft::leTangent;
    args.m_bAllowStartTangent = TRUE;
  }

  // If the ending loft curve has a trim, 
  // set the end condition to tangent.
  if (args.m_loftcurves[lc_count - 1] && args.m_loftcurves[lc_count - 1]->m_trim)
  {
    args.m_end_condition = CArgsRhinoLoft::leTangent;
    args.m_bAllowEndTangent = TRUE;
  }

  // Do the loft calculation
  ON_SimpleArray<ON_Brep*> out_breps;
  CRhinoCommand::result rc = CreateOutput(context.m_doc, args, false, out_breps);

  // Delete the loft curves so we do not leak memory.
  for (int i = 0; i < args.m_loftcurves.Count(); i++)
    delete args.m_loftcurves[i];
  args.m_loftcurves.Empty();

  // If the loft operation failed, bail.
  const int out_brep_count = out_breps.Count();
  if (rc != CRhinoCommand::success || 0 == out_brep_count)
    return CRhinoCommand::failure;

  CRhinoBrepObject* brep_obj = nullptr;
  for (int i = 0; i < out_brep_count; i++)
  {
    brep_obj = new CRhinoBrepObject();
    brep_obj->SetBrep(out_breps[i]);
    out_breps[i] = nullptr;
    context.m_doc.AddObject(brep_obj);
  }

  if (1 == out_brep_count && nullptr != brep_obj)
  {
    CRhinoHistory history(*this);
    bool bSplitCreasedSurfaces = RhinoApp().AppSettings().GeneralSettings().m_bSplitCreasedSurfaces;
    if (WriteHistory(history, in_curves, bSplitCreasedSurfaces))
      context.m_doc.m_history_record_table.CreateObjectHistory(brep_obj, history);
  }

  context.m_doc.Redraw();

  return rc;
}

void CCommandSampleLoft::ProcessInput(
  CArgsRhinoLoft& args, 
  const CRhinoObjRef& ref, 
  ON_ClassArray<CRhinoObjRef>* in_curves
)
{
  const ON_Curve* crv = ref.Curve();
  if (nullptr == crv)
    return;

  // New loft curve
  CRhinoLoftCurve* lc = new CRhinoLoftCurve();

  // Duplicate the selected curve. Note,
  // the loft curve will delete this curve.
  lc->m_curve = crv->DuplicateCurve();
  lc->m_curve->RemoveShortSegments(ON_ZERO_TOLERANCE);

  // Set other loft curve parameters
  lc->m_bPlanar = (lc->m_curve->IsPlanar(&lc->m_plane) ? true : false);

  if (ref.SelectionPoint(
    lc->m_pick_point) &&
    lc->m_pick_point.IsValid() &&
    crv == ref.CurveParameter(&lc->m_pick_t) &&
    ON_IsValid(lc->m_pick_t)
    )
  {
    lc->m_pick_point = crv->PointAt(lc->m_pick_t);
  }
  else
  {
    lc->m_pick_point = ON_UNSET_POINT;
    lc->m_pick_t = ON_UNSET_VALUE;
  }

  // If referenced geometry is a surface edge,
  // assign associated brep trim.
  lc->m_trim = ref.Trim();

  // Append loft curve to loft argument
  args.m_loftcurves.Append(lc);

  // Append objref for history
  if (nullptr != in_curves)
    in_curves->Append(ref);
}

CRhinoCommand::result CCommandSampleLoft::CreateOutput(
  CRhinoDoc& doc, 
  CArgsRhinoLoft& args, 
  bool bInReplayHistory, 
  ON_SimpleArray<ON_Brep*>& out_breps
)
{
  CRhinoCommand::result rc = CRhinoCommand::success;

  // Make the final surface
  ON_SimpleArray<ON_NurbsSurface*> srf;
  RhinoSdkLoftSurface(args, srf);

  if (srf.Count() <= 0)
  {
    rc = CRhinoCommand::failure;
  }
  else
  {
    double tol = doc.AbsoluteTolerance();
    ON_Brep* brep = nullptr;
    ON_SimpleArray<ON_Brep*> breparray;
    for (int i = 0; i < srf.Count(); i++)
    {
      if (srf[i])
      {
        if (srf[i]->IsValid())
        {
          brep = ON_Brep::New();
          brep->NewFace(*srf[i]);
          breparray.Append(brep);
        }
        delete srf[i];
        srf[i] = nullptr;
      }
    }

    ON_Brep* result = nullptr;
    if (breparray.Count() > 1)
      result = RhinoJoinBreps(breparray, tol);

    if (result)
    {
      out_breps.Append(result);
      for (int i = 0; i < breparray.Count(); i++)
      {
        if (0 != breparray[i])
          delete breparray[i];
      }
    }
    else
    {
      out_breps.Reserve(breparray.Count());
      for (int i = 0; i < breparray.Count(); i++)
      {
        if (breparray[i])
          out_breps.Append(breparray[i]);
      }
    }
  }

  return rc;
}

bool CCommandSampleLoft::WriteHistory(
  CRhinoHistory& history, 
  const ON_ClassArray<CRhinoObjRef>& in_curves,
  bool bSplitCreasedSurfaces
)
{
  bool rc = (in_curves.Count() >= 2);
  if (rc)
    rc = history.SetObjRefValues(1, in_curves.Count(), in_curves.Array());
  if (rc)
    rc = history.SetBoolValue(2, bSplitCreasedSurfaces);
  if (rc)
    rc = history.SetHistoryVersion(m_history_version);
  return rc;
}

bool CCommandSampleLoft::ReadHistory(
  const CRhinoHistoryRecord& history_record, 
  ON_ClassArray<CRhinoObjRef>& in_curves,
  bool& bSplitCreasedSurfaces
)
{
  bool rc = (m_history_version == history_record.HistoryVersion());
  if (rc)
  {
    in_curves.SetCount(0);
    rc = history_record.GetRhinoObjRefList(1, in_curves);
  }
  if (rc)
    rc = history_record.m_hr.GetBoolValue(2, &bSplitCreasedSurfaces);
  return rc;
}

CRhinoObject* CCommandSampleLoft::ReplayHistory(
  const CRhinoHistoryRecord& history_record
)
{
  if (m_history_version != history_record.HistoryVersion())
    return nullptr;

  CRhinoBrepObject* brep_obj = nullptr;
  CRhinoDoc& doc = history_record.Document();

  // Rerun command
  ON_ClassArray<CRhinoObjRef> in_curves;
  bool bSplitCreasedSurfaces = false;
  if (ReadHistory(history_record, in_curves, bSplitCreasedSurfaces))
  {
    const int crv_count = in_curves.Count();
    if (crv_count >= 2)
    {
      // Create loft arguments object
      CArgsRhinoLoft args;
      args.m_loftcurves.SetCapacity(crv_count);

      // Add curves to loft arguments object
      for (int i = 0; i < crv_count; i++)
      {
        const CRhinoObjRef& ref = in_curves[i];
        ProcessInput(args, ref, nullptr);
      }

      const int lc_count = args.m_loftcurves.Count();
      if (lc_count != crv_count)
        return nullptr;

      // If the starting loft curve has a trim, 
      // set the start condition to tangent.
      if (args.m_loftcurves[0] && args.m_loftcurves[0]->m_trim)
      {
        args.m_start_condition = CArgsRhinoLoft::leTangent;
        args.m_bAllowStartTangent = TRUE;
      }

      // If the ending loft curve has a trim, 
      // set the end condition to tangent.
      if (args.m_loftcurves[lc_count - 1] && args.m_loftcurves[lc_count - 1]->m_trim)
      {
        args.m_end_condition = CArgsRhinoLoft::leTangent;
        args.m_bAllowEndTangent = TRUE;
      }

      // Do the loft calculation
      ON_SimpleArray<ON_Brep*> out_breps;
      CRhinoCommand::result rc = CreateOutput(doc, args, true, out_breps);
      if (
        rc == CRhinoCommand::success && 
        1 == out_breps.Count() &&
        nullptr != out_breps[0]
        )
      {
        if (bSplitCreasedSurfaces)
          out_breps[0]->SplitKinkyFaces();

        brep_obj = new CRhinoBrepObject();
        brep_obj->SetBrep(out_breps[0]);
        out_breps[0] = nullptr;
      }
      else
      {
        for (int i = 0; i < out_breps.Count(); i++)
        {
          delete out_breps[i];
          out_breps[i] = nullptr;
        }
      }
    }
  }

  return brep_obj;
}

//
// END SampleLoft command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
