#include "StdAfx.h"

//  This command shows how to use the History mechanism in Rhino.
//  The command makes a simple surface from a pair of input curves.
//  A history record is added to the document together with the object.
//  The record refers to the curves which are called the antecedent objects.
//  The ReplayHistory function reads the history record and constructs the 
//  updated surface from the updated curves. 

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleHistory command
//

#pragma region SampleHistory command


// Do NOT put the definition of class CCommandSampleHistory in a header
// file.  There is only ONE instance of a CCommandSampleHistory class
// and that instance is the static theSampleHistoryCommand that appears
// immediately below the class definition.

class CCommandSampleHistory : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleHistory is created below.
  // No copy constructor or operator= is required.  Values of
  // member variables persist for the duration of the application.

  // CCommandSampleHistory::CCommandSampleHistory()
  // is called exactly once when static theSampleHistoryCommand is created.
  CCommandSampleHistory() {}

  // CCommandSampleHistory::~CCommandSampleHistory()
  // is called exactly once when static theSampleHistoryCommand is
  // destroyed.  The destructor should not make any calls to
  // the Rhino SDK.  If your command has persistent settings,
  // then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleHistory() {}

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work.  Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID()
  {
    // {5D78F231-A88E-453F-AC09-0AD37135EB16}
    static const GUID SampleHistoryCommand_UUID =
    { 0x5D78F231, 0xA88E, 0x453F, { 0xAC, 0x9, 0xA, 0xD3, 0x71, 0x35, 0xEB, 0x16 } };
    return SampleHistoryCommand_UUID;
  }

  // Returns the English command name.
  const wchar_t* EnglishCommandName() { return L"SampleHistory"; }

  // Returns the localized command name.
  const wchar_t* LocalCommandName() const { return L"SampleHistory"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

  // Returns the resulting surface from the input curves
  ON_Surface* MakeBilinearSurface(const ON_Curve& C0, const ON_Curve& C1);

  // Rhino calls the virtual ReplayHistory functions to
  // to remake an objects when inputs have changed.  
  // If a command supports history, it must override exactly
  // one of the ReplayHistory functions. 
  CRhinoObject* ReplayHistory(const class CRhinoHistoryRecord& history_record);

  // Write a history record referencing the antecedent objects
  // The history should contain all the information required to reconstruct the input,
  // This may include parameters, options, or settings
  bool WriteHistory(CRhinoHistory& history, CRhinoObjRef Crv0, CRhinoObjRef Crv1);

  // Read a history record and extract the references for the antecedent objects
  bool ReadHistory(const CRhinoHistoryRecord& history_record, CRhinoObjRef& Crv0, CRhinoObjRef& Crv1);

  // History recording version
  // This field is used to ensure the version of the replay function matches
  // the version that originally created the geometry.
  static const int m_sample_history_version;
};

// The one and only CCommandSampleHistory object.  
// Do NOT create any other instance of a CCommandSampleHistory class.
static class CCommandSampleHistory theSampleHistoryCommand;

const int CCommandSampleHistory::m_sample_history_version(20120813);

bool CCommandSampleHistory::WriteHistory(CRhinoHistory& history, CRhinoObjRef Crv0, CRhinoObjRef Crv1)
{
  if (!history.SetObjRefValue(0, Crv0))
    return false;

  if (!history.SetObjRefValue(1, Crv1))
    return false;

  history.SetHistoryVersion(m_sample_history_version);

  return true;
}

bool CCommandSampleHistory::ReadHistory(const CRhinoHistoryRecord& history_record, CRhinoObjRef& Crv0, CRhinoObjRef& Crv1)
{
  // Check to make sure this history record was made with this version
  if (m_sample_history_version != history_record.HistoryVersion())
    return false;

  if (!history_record.GetRhinoObjRef(0, Crv0))
    return false;

  if (!history_record.GetRhinoObjRef(1, Crv1))
    return false;

  return true;
}

CRhinoObject* CCommandSampleHistory::ReplayHistory(const class CRhinoHistoryRecord& history_record)
{
  CRhinoSurfaceObject* result = 0;
  CRhinoObjRef Crv0, Crv1;
  if (ReadHistory(history_record, Crv0, Crv1))
  {
    if (Crv0.Curve() && Crv1.Curve())
    {
      ON_Surface* pSrf = MakeBilinearSurface(*Crv0.Curve(), *Crv1.Curve());
      if (pSrf)
      {
        result = new CRhinoSurfaceObject();
        result->SetSurface(pSrf);
      }
    }
  }
  return result;
}

ON_Surface* CCommandSampleHistory::MakeBilinearSurface(const ON_Curve& C0, const ON_Curve& C1)
{
  ON_BezierSurface Srf(3, false, 2, 2);
  Srf.SetCV(0, 0, C0.PointAtStart());
  Srf.SetCV(0, 1, C0.PointAtEnd());
  Srf.SetCV(1, 0, C1.PointAtStart());
  Srf.SetCV(1, 1, C1.PointAtEnd());

  ON_NurbsSurface* pSrf = new ON_NurbsSurface(Srf);
  return pSrf;
}

CRhinoCommand::result CCommandSampleHistory::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select two curves");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(2, 2);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  if (2 != go.ObjectCount())
    return CRhinoCommand::failure;

  CRhinoCommand::result rc = CRhinoCommand::failure;

  const CRhinoObjRef& CObj0 = go.Object(0);
  const CRhinoObjRef& CObj1 = go.Object(1);

  const ON_Curve* c0 = CObj0.Curve();
  const ON_Curve* c1 = CObj1.Curve();

  if (c0 && c1)
  {
    ON_Surface* pSrf = MakeBilinearSurface(*c0, *c1);
    ON_Brep brep;
    if (pSrf && brep.Create(pSrf))
    {
      CRhinoHistory history(*this);
      WriteHistory(history, CObj0, CObj1);
      context.m_doc.AddBrepObject(brep, 0, &history);
      rc = CRhinoCommand::success;
    }
  }

  return rc;
}

#pragma endregion

//
// END SampleHistory command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
