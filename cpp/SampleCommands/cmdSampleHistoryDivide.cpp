#include "StdAfx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleHistoryDivide command
//

class CCommandSampleHistoryDivide : public CRhinoCommand
{
public:
  CCommandSampleHistoryDivide() {}
  ~CCommandSampleHistoryDivide() {}
  UUID CommandUUID()
  {
    // {C4EDB0D9-EF45-4B08-B0EE-5A5479938272}
    static const GUID SampleHistoryDivideCommand_UUID =
    { 0xC4EDB0D9, 0xEF45, 0x4B08, { 0xB0, 0xEE, 0x5A, 0x54, 0x79, 0x93, 0x82, 0x72 } };
    return SampleHistoryDivideCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleHistoryDivide"; }
  const wchar_t* LocalCommandName() { return L"SampleHistoryDivide"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

  //Rhino calls the virtual ReplayHistory functions to
  //to remake an objects when inputs have changed.  
  //If a command supports history, it must override exactly
  //one of the ReplayHistory functions. 
  bool ReplayHistory(const class CRhinoHistoryRecord& history_record, ON_SimpleArray<CRhinoObjectPair>& results);

  // Write a history record referencing the antecedent objects
  // The history should contain all the information required to reconstruct the input,
  // This may include parameters, options, or settings
  bool WriteHistory(CRhinoHistory& history, CRhinoObjRef Crv, int num);

  // Read a history record and extract the references for the antecedent objects
  bool ReadHistory(const CRhinoHistoryRecord& history_record, CRhinoObjRef& Crv, int& num);

  // History recording version
  // This field is used to ensure the version of the replay function matches
  // the version that originally created the geometry.
  static const int m_sample_history_divide_version;
};

// The one and only CCommandSampleHistoryDivide object
static class CCommandSampleHistoryDivide theSampleHistoryDivideCommand;

const int CCommandSampleHistoryDivide::m_sample_history_divide_version(20120813);

bool CCommandSampleHistoryDivide::WriteHistory(CRhinoHistory& history, CRhinoObjRef Crv, int num)
{
  if (!history.SetObjRefValue(0, Crv))
    return false;

  if (!history.SetIntValue(2, num))
    return false;

  history.SetHistoryVersion(m_sample_history_divide_version);

  return true;
}

bool CCommandSampleHistoryDivide::ReadHistory(const CRhinoHistoryRecord& history_record, CRhinoObjRef& Crv, int& num)
{
  // Check to make sure this history record was made with this version
  if (m_sample_history_divide_version != history_record.HistoryVersion())
    return false;

  if (!history_record.GetRhinoObjRef(0, Crv))
    return false;

  if (!history_record.m_hr.GetIntValue(2, &num))
    return false;

  return true;
}

bool CCommandSampleHistoryDivide::ReplayHistory(const class CRhinoHistoryRecord& history_record, ON_SimpleArray<CRhinoObjectPair>& results)
{
  bool result = false;
  CRhinoObjRef Crv;
  int num = 0;

  if (ReadHistory(history_record, Crv, num))
  {
    if (Crv.Curve())
    {
      ON_SimpleArray<ON_3dPoint> pts;
      pts.SetCount(0);
      result = RhinoDivideCurve(*Crv.Curve(), num, 0, false, true, &pts, 0);
      if (!result)
        return false;

      for (int i = 0; i < pts.Count(); i++)
      {
        CRhinoPointObject* PObj = new CRhinoPointObject();
        PObj->SetPoint(pts[i]);

        if (results.Count() > i)
        {
          results[i].m_object[1] = PObj;
        }
        else
        {
          CRhinoObjectPair& op = results.AppendNew();
          op.m_object[0] = NULL;
          op.m_object[1] = PObj;
        }
      }
    }
  }
  return true;
}

CRhinoCommand::result CCommandSampleHistoryDivide::RunCommand(const CRhinoCommandContext& context)
{
  static int num = 10;

  CRhinoGetObject go;

  for (;;)
  {
    go.SetCommandPrompt(L"Select curves to divide");
    go.SetGeometryFilter(ON::curve_object);
    go.AddCommandOptionInteger(RHCMDOPTNAME(L"Num"), &num);
    go.GetObjects(1, 1);

    if (go.CommandResult() != CRhinoCommand::success)
      return go.CommandResult();

    if (go.Result() == CRhinoGet::option)
      continue;

    break;
  }

  CRhinoObjRef CObj = go.Object(0);
  const ON_Curve* crv = CObj.Curve();
  if (!crv)
    return failure;

  ON_SimpleArray<ON_3dPoint> crv_pts;
  crv_pts.SetCount(0);

  if (!RhinoDivideCurve(*crv, num, 0, false, true, &crv_pts, 0))
    return failure;

  // ADD POINTS TO CONTEXT
  CRhinoHistory history(*this);
  WriteHistory(history, CObj, num);

  for (int j = 0; j < crv_pts.Count(); j++)
  {
    context.m_doc.AddPointObject(crv_pts[j], 0, &history);
  }

  return CRhinoCommand::success;
}

//
// END SampleHistoryDivide command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
