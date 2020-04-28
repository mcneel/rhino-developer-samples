#include "stdafx.h"

#define EPS_DIVIDE 0.000001

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDupEdge command
//

#pragma region SampleDupEdge command

class CCommandSampleDupEdge : public CRhinoCommand
{
public:
  CCommandSampleDupEdge();
  UUID CommandUUID() override
  {
    // {4E3DCF1D-B1EA-4E95-B6F6-7665E2847A1A}
    static const GUID SampleDupEdgeCommand_UUID =
    { 0x4E3DCF1D, 0xB1EA, 0x4E95, { 0xB6, 0xF6, 0x76, 0x65, 0xE2, 0x84, 0x7A, 0x1A } };
    return SampleDupEdgeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDupEdge"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
  CRhinoObject* ReplayHistory(const CRhinoHistoryRecord& history_record) override;

private:
  bool IsWeightConst(const ON_NurbsCurve& crv, double tolerance);

private:
  const int m_history_version;
};

// The one and only CCommandSampleDupEdge object
static class CCommandSampleDupEdge theSampleDupEdgeCommand;

CCommandSampleDupEdge::CCommandSampleDupEdge()
  :m_history_version(20200224)
{
}

CRhinoCommand::result CCommandSampleDupEdge::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select edges to duplicate");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::edge_curve);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    CRhinoObjRef obj_ref = go.Object(i);
    const ON_BrepEdge* edge = obj_ref.Edge();
    if (nullptr != edge)
    {
      ON_Curve* curve = edge->DuplicateCurve();
      if (nullptr != curve)
      {
        // Some extra fluff
        ON_NurbsCurve* nurb = ON_NurbsCurve::Cast(curve);
        if (
          nullptr != nurb &&
          nurb->IsRational() &&
          IsWeightConst(*nurb, EPS_DIVIDE) &&
          0.0 != nurb->Weight(0)
          )
        {
          nurb->MakeNonRational();
        }

        CRhinoCurveObject* curve_obj = new CRhinoCurveObject();
        curve_obj->SetCurve(curve);
        if (context.m_doc.AddObject(curve_obj))
        {
          // Select the new curve
          curve_obj->Select();

          // Unselect the object
          if (go.ObjectsWerePreSelected())
          {
            const CRhinoObject* obj = obj_ref.Object();
            if (nullptr != obj)
              obj->Select(false);
          }

          // Record history if needed
          if (context.m_doc.m_history_record_table.HistoryRecordingEnabled())
          {
            CRhinoHistory history(*this);
            history.SetHistoryVersion(m_history_version);
            history.SetObjRefValue(0, obj_ref);
            curve_obj->SetHistory(history);
          }
        }
        else
        {
          delete curve_obj;
          curve_obj = nullptr;
        }
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

CRhinoObject* CCommandSampleDupEdge::ReplayHistory(const CRhinoHistoryRecord& history_record)
{
  if (history_record.HistoryVersion() < m_history_version)
    return nullptr;

  CRhinoObjRef obj_ref;
  history_record.m_hr.GetObjRefValue(0, obj_ref);

  const ON_BrepEdge* edge = obj_ref.Edge();
  if (nullptr != edge)
  {
    ON_Curve* curve = edge->DuplicateCurve();
    if (nullptr != curve)
    {
      // Some extra fluff
      ON_NurbsCurve* nurb = ON_NurbsCurve::Cast(curve);
      if (
        nullptr != nurb &&
        nurb->IsRational() &&
        IsWeightConst(*nurb, EPS_DIVIDE) &&
        0.0 != nurb->Weight(0)
        )
      {
        nurb->MakeNonRational();
      }

      CRhinoCurveObject* curve_obj = new CRhinoCurveObject();
      curve_obj->SetCurve(curve);
      return curve_obj;
    }
  }

  return nullptr;
}

bool CCommandSampleDupEdge::IsWeightConst(const ON_NurbsCurve& crv, double tolerance)
{
  if (crv.IsRational())
  {
    double w0 = fabs(crv.Weight(0));
    for (int i = 1; i < crv.CVCount(); i++)
    {
      double w = crv.Weight(i);
      if (fabs(w - w0) > tolerance * w0)
        return false;
    }
  }
  return true;
}


#pragma endregion

  //
  // END SampleDupEdge command
  //
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
