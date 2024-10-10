#include "stdafx.h"

/// <summary>
/// CSampleBlendCurveConduit display conduit class
/// </summary>
class CSampleBlendCurveConduit : public CRhinoDisplayConduit
{
public:
  CSampleBlendCurveConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate) override;
  void SetCurve(const ON_Curve* curve);

private:
  const ON_Curve* m_curve = nullptr;
  ON_Color m_color;
};

CSampleBlendCurveConduit::CSampleBlendCurveConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_DRAWOVERLAY)
{
  m_color = RhinoApp().AppSettings().ActiveLayerColor();
}

void CSampleBlendCurveConduit::SetCurve(const ON_Curve* curve)
{
  m_curve = curve;
}

bool CSampleBlendCurveConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool&)
{
  if (nActiveChannel == CSupportChannels::SC_CALCBOUNDINGBOX)
  {
    if (nullptr != m_curve)
      m_pChannelAttrs->m_BoundingBox.Union(m_curve->BoundingBox());
  }
  else if (nActiveChannel == CSupportChannels::SC_DRAWOVERLAY)
  {
    if (nullptr != m_curve)
      dp.DrawCurve(*m_curve, m_color);
  }
  return true;
}


/// <summary>
/// Blend continuity types
/// </summary>
enum class BlendContinuity : unsigned char
{
  /// <summary>
  /// G0: The curves or surfaces touch at the join point (position).
  /// </summary>
  Position = 0,
  /// <summary>
  /// G1: The curves or surfaces also share a common tangent direction at the join point (tangent).
  /// </summary>
  Tangency = 1,
  /// <summary>
  /// G2: The curves or surfaces also share a common center of curvature at the join point (curvature).
  /// </summary>
  Curvature = 2
};

static BlendContinuity BlendContinuityFromUnsigned(unsigned int continuity_as_unsigned)
{
  switch (continuity_as_unsigned)
  {
    ON_ENUM_FROM_UNSIGNED_CASE(BlendContinuity::Position);
    ON_ENUM_FROM_UNSIGNED_CASE(BlendContinuity::Tangency);
    ON_ENUM_FROM_UNSIGNED_CASE(BlendContinuity::Curvature);
  }
  return BlendContinuity::Position;
}


/// <summary>
/// CSampleBlendCurveHelper blend curve helper class
/// </summary>
class CSampleBlendCurveHelper
{
public:
  CSampleBlendCurveHelper() = default;
  ~CSampleBlendCurveHelper();

  bool IsValid() const;
  bool ComputeBlendCurve();

  const ON_Curve* GetBlendCurve() const;
  ON_Curve* DetachBlendCurve();
  void DestroyBlendCurve();

public:
  CRhinoObjRef m_curveRefA;
  CRhinoObjRef m_curveRefB;
  BlendContinuity m_continuity = BlendContinuity::Position;
  double m_bulgeA = 1.0;
  double m_bulgeB = 1.0;

private:
  ON_Curve* m_blend_curve = nullptr;
};

CSampleBlendCurveHelper::~CSampleBlendCurveHelper()
{
  DestroyBlendCurve();
}

void CSampleBlendCurveHelper::DestroyBlendCurve()
{
  if (nullptr != m_blend_curve)
  {
    delete m_blend_curve;
    m_blend_curve = nullptr;
  }
}

bool CSampleBlendCurveHelper::IsValid() const
{
  const ON_Curve* curveA = m_curveRefA.Curve();
  const ON_Curve* curveB = m_curveRefB.Curve();
  if (
    nullptr != curveA
    && !curveA->IsClosed()
    && nullptr != curveB
    && !curveB->IsClosed()
    )
    return true;
  return false;
}

bool CSampleBlendCurveHelper::ComputeBlendCurve()
{
  DestroyBlendCurve();

  const ON_Curve* curveA = m_curveRefA.Curve();
  const ON_Curve* curveB = m_curveRefB.Curve();
  if (nullptr != curveA && nullptr != curveB)
  {
    // Simple curve blending routines, assume curves are in order.
    // You can also use RhinoSdkBlend::CurveBlend, but it will require
    // updating this class and the command history record.
    switch (m_continuity)
    {
    case BlendContinuity::Position:
      m_blend_curve = ON_BlendG0Curve(curveA, curveB);
      break;
    case BlendContinuity::Tangency:
      m_blend_curve = ON_BlendG1Curve(curveA, curveB, m_bulgeA, m_bulgeB);
      break;
    case BlendContinuity::Curvature:
      m_blend_curve = ON_BlendG2Curve(curveA, curveB, m_bulgeA, m_bulgeB);
      break;
    }
  }

  return (nullptr != m_blend_curve);
}

const ON_Curve* CSampleBlendCurveHelper::GetBlendCurve() const
{
  return m_blend_curve;
}

ON_Curve* CSampleBlendCurveHelper::DetachBlendCurve()
{
  if (nullptr != m_blend_curve)
  {
    // caller now own pointer
    ON_Curve* curve = m_blend_curve;
    m_blend_curve = nullptr;
    return curve;
  }
  return nullptr;
}


/// <summary>
/// CGetSampleBlendCurve object getting class
/// </summary>
class CGetSampleBlendCurve : public CRhinoGetObject
{
public:
  CGetSampleBlendCurve() = default;
  ~CGetSampleBlendCurve() = default;

  bool CustomGeometryFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
  ) const override;
};


/// <summary>
/// SampleBlendCurve command class
/// </summary>
class CCommandSampleBlendCurve : public CRhinoCommand
{
public:
  CCommandSampleBlendCurve() = default;
  UUID CommandUUID() override
  {
    // {4E602038-AEB3-42D4-A567-3A2625B91BBB}
    static const GUID SampleBlendCurveCommand_UUID =
    { 0x4E602038, 0xAEB3, 0x42D4, { 0xA5, 0x67, 0x3A, 0x26, 0x25, 0xB9, 0x1B, 0xBB } };
    return SampleBlendCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleBlendCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
  CRhinoObject* ReplayHistory(const CRhinoHistoryRecord& history_record) override;

  bool HasSampleBlendSurfaceHistory(const CRhinoObject* object);

private:
  CRhinoCommand::result EditBlendCurve(const CRhinoCommandContext& context, CSampleBlendCurveHelper& helper);
  CRhinoCommand::result EditCommandOption(const CRhinoCommandContext& context);

  bool WriteHistory(CRhinoHistory& history, const CSampleBlendCurveHelper& helper) const;
  bool ReadHistory(const CRhinoHistoryRecord& history_record, CSampleBlendCurveHelper& helper) const;

  void GetPersistentSettings(CSampleBlendCurveHelper& helper);
  void SetPersistentSettings(const CSampleBlendCurveHelper& helper);

private:
  const int m_history_version = 20240918;
};

// The one and only CCommandSampleBlendCurve object
static class CCommandSampleBlendCurve theSampleBlendCurveCommand;

CRhinoCommand::result CCommandSampleBlendCurve::RunCommand(const CRhinoCommandContext& context)
{
  CSampleBlendCurveHelper helper;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to blend");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::open_curve);
  go.EnableSubObjectSelect(false);
  go.AddCommandOption(RHCMDOPTNAME(L"Edit"));

  CRhinoGet::result res = go.GetObjects(1, 1);
  if (res == CRhinoGet::option)
    return EditCommandOption(context);
  else if (res != CRhinoGet::object)
    return CRhinoCommand::cancel;

  helper.m_curveRefA = go.Object(0);

  go.ClearCommandOptions();
  go.EnablePreSelect(false, true);
  go.EnableDeselectAllBeforePostSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  helper.m_curveRefB = go.Object(0);

  if (!helper.IsValid())
    return CRhinoCommand::failure;

  GetPersistentSettings(helper);

  CRhinoCommand::result rc = EditBlendCurve(context, helper);
  if (rc == CRhinoCommand::success)
  {
    // One final compute
    if (helper.ComputeBlendCurve())
    {
      ON_Curve* curve = helper.DetachBlendCurve();
      if (curve)
      {
        CRhinoCurveObject* curve_obj = new CRhinoCurveObject();
        curve_obj->SetCurve(curve);

        CRhinoHistory history(*this);
        WriteHistory(history, helper);

        context.m_doc.m_history_record_table.CreateObjectHistory(curve_obj, history);
        context.m_doc.AddObject(curve_obj);

        SetPersistentSettings(helper);
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleBlendCurve::EditBlendCurve(const CRhinoCommandContext& context, CSampleBlendCurveHelper& helper)
{
  if (!helper.IsValid())
    return CRhinoCommand::failure;

  CRhinoCommandOptionValue list[3] = { RHCMDOPTVALUE(L"Position"), RHCMDOPTVALUE(L"Tangency"), RHCMDOPTVALUE(L"Curvature") };

  CSampleBlendCurveConduit conduit;
  conduit.Enable(context.m_rhino_doc_sn);

  CRhinoGetOption go;
  go.SetCommandPrompt(L"Blend curve options");
  go.AcceptNothing();

  CRhinoCommand::result rc = CRhinoCommand::success;

  while (true)
  {
    helper.ComputeBlendCurve();
    conduit.SetCurve(helper.GetBlendCurve());
    context.m_doc.Redraw();

    go.ClearCommandOptions();
    int c_index = go.AddCommandOptionList(RHCMDOPTNAME(L"Continuity"), _countof(list), list, (int)helper.m_continuity);
    int a_index = -1;
    int b_index = -2;
    if (helper.m_continuity != BlendContinuity::Position)
    {
      a_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"BulgeA"), &helper.m_bulgeA, L"New bulge value", false, 0.0);
      b_index = go.AddCommandOptionNumber(RHCMDOPTNAME(L"BulgeB"), &helper.m_bulgeB, L"New bulge value", false, 0.0);
    }

    CRhinoGet::result res = go.GetOption();
    if (res == CRhinoGet::nothing)
      break;

    else if (res != CRhinoGet::option)
    {
      rc = CRhinoCommand::cancel;
      break;
    }

    const CRhinoCommandOption* command_option = go.Option();
    if (nullptr == command_option)
    {
      rc = CRhinoCommand::failure;
      break;
    }

    const int option_index = command_option->m_option_index;
    if (option_index == c_index)
      helper.m_continuity = BlendContinuityFromUnsigned(command_option->m_list_option_current);

    // other options modify helper values directly

    continue;
  }

  conduit.Disable();

  return rc;
}

bool CGetSampleBlendCurve::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry*, ON_COMPONENT_INDEX) const
{
  return theSampleBlendCurveCommand.HasSampleBlendSurfaceHistory(object);
}

CRhinoCommand::result CCommandSampleBlendCurve::EditCommandOption(const CRhinoCommandContext& context)
{
  CGetSampleBlendCurve go;
  go.SetCommandPrompt(L"Select blend curve to edit");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef objref = go.Object(0);
  const CRhinoObject* object = objref.Object();
  if (!HasSampleBlendSurfaceHistory(object))
    return CRhinoCommand::failure;

  const CRhinoHistoryRecord* history_record = object->HistoryRecord();
  if (nullptr == history_record)
  {
    RhinoApp().Print(L"Error retrieving %ls history record.\n", EnglishCommandName());
    return CRhinoCommand::failure;
  }

  CSampleBlendCurveHelper helper;
  if (!ReadHistory(*history_record, helper))
  {
    RhinoApp().Print(L"Error reading %ls history record.\n", EnglishCommandName());
    return CRhinoCommand::failure;
  }

  CRhinoHistoryManager* history_manager = RhinoHistoryManager();
  bool bLocking = history_manager->HistoryObjectLockingEnabled();
  if (bLocking)
    history_manager->EnableHistoryObjectLocking(false);

  CRhinoCommand::result rc = EditBlendCurve(context, helper);
  if (rc == CRhinoCommand::success)
  {
    // One final compute
    if (helper.ComputeBlendCurve())
    {
      ON_Curve* curve = helper.DetachBlendCurve();
      if (curve)
      {
        CRhinoCurveObject* curve_obj = new CRhinoCurveObject();
        curve_obj->SetCurve(curve);

        // Always record history for editing
        bool bRecording = history_manager->HistoryRecordingEnabled();
        history_manager->EnableHistoryRecording(true);

        CRhinoHistory history(*this);
        WriteHistory(history, helper);

        context.m_doc.m_history_record_table.CreateObjectHistory(curve_obj, history);
        context.m_doc.ReplaceObject(object, curve_obj);

        history_manager->EnableHistoryRecording(bRecording);
      }
    }
  }

  history_manager->EnableHistoryObjectLocking(bLocking);

  context.m_doc.Redraw();

  return rc;
}

CRhinoObject* CCommandSampleBlendCurve::ReplayHistory(const CRhinoHistoryRecord& history_record)
{
  if (m_history_version != history_record.HistoryVersion())
    return nullptr;

  CRhinoCurveObject* curve_obj = nullptr;

  CSampleBlendCurveHelper helper;
  if (ReadHistory(history_record, helper))
  {
    if (helper.ComputeBlendCurve())
    {
      ON_Curve* curve = helper.DetachBlendCurve();
      if (curve)
      {
        curve_obj = new CRhinoCurveObject();
        curve_obj->SetCurve(curve);
      }
    }
  }

  return curve_obj;
}

bool CCommandSampleBlendCurve::WriteHistory(CRhinoHistory& history, const CSampleBlendCurveHelper& helper) const
{
  bool rc = history.SetHistoryVersion(m_history_version);
  if (rc)
    rc = history.SetObjRefValue(1, helper.m_curveRefA);
  if (rc)
    rc = history.SetObjRefValue(2, helper.m_curveRefB);
  if (rc)
    rc = history.SetIntValue(3, (int)helper.m_continuity);
  if (rc)
    rc = history.SetDoubleValue(4, helper.m_bulgeA);
  if (rc)
    rc = history.SetDoubleValue(5, helper.m_bulgeB);
  return rc;
}

bool CCommandSampleBlendCurve::ReadHistory(const CRhinoHistoryRecord& history_record, CSampleBlendCurveHelper& helper) const
{
  bool rc = m_history_version == history_record.HistoryVersion();
  if (rc)
    rc = history_record.m_hr.GetObjRefValue(1, helper.m_curveRefA);
  if (rc)
    rc = history_record.m_hr.GetObjRefValue(2, helper.m_curveRefB);
  if (rc)
  {
    int continuity = (int)BlendContinuity::Position;
    rc = history_record.m_hr.GetIntValue(3, &continuity);
    if (rc)
      helper.m_continuity = BlendContinuityFromUnsigned(continuity);
  }
  if (rc)
    rc = history_record.m_hr.GetDoubleValue(4, &helper.m_bulgeA);
  if (rc)
    rc = history_record.m_hr.GetDoubleValue(5, &helper.m_bulgeB);
  return rc;
}

void CCommandSampleBlendCurve::GetPersistentSettings(CSampleBlendCurveHelper& helper)
{
  int continuity = (int)BlendContinuity::Position;
  Settings().GetInteger(L"continuity", continuity, (int)BlendContinuity::Position);
  helper.m_continuity = BlendContinuityFromUnsigned(continuity);

  Settings().GetDouble(L"bulgeA", helper.m_bulgeA, 1.0);
  Settings().GetDouble(L"bulgeB", helper.m_bulgeB, 1.0);
}

void CCommandSampleBlendCurve::SetPersistentSettings(const CSampleBlendCurveHelper& helper)
{
  Settings().SetInteger(L"continuity", (int)helper.m_continuity);
  Settings().SetDouble(L"bulgeA", helper.m_bulgeA);
  Settings().SetDouble(L"bulgeB", helper.m_bulgeB);
}

bool CCommandSampleBlendCurve::HasSampleBlendSurfaceHistory(const CRhinoObject* object)
{
  const CRhinoCurveObject* curve_obj = CRhinoCurveObject::Cast(object);
  if (nullptr != curve_obj)
  {
    const CRhinoHistoryRecord* history_record = curve_obj->HistoryRecord();
    if (nullptr != history_record)
    {
      if (m_history_version == history_record->HistoryVersion())
      {
        CRhinoCommand* command = history_record->Command();
        if (command == this)
          return true;
      }
    }
  }
  return false;
}
