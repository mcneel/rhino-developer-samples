#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleJoinSrf command
//

class CCommandSampleJoinSrf : public CRhinoCommand
{
public:
  CCommandSampleJoinSrf() {}
  ~CCommandSampleJoinSrf() {}
  UUID CommandUUID()
  {
    // {AB907EC4-6070-40F6-A76B-623F2902158C}
    static const GUID SampleJoinSrfCommand_UUID =
    { 0xAB907EC4, 0x6070, 0x40F6, { 0xA7, 0x6B, 0x62, 0x3F, 0x29, 0x02, 0x15, 0x8C } };
    return SampleJoinSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleJoinSrf"; }
  const wchar_t* LocalCommandName() { return L"SampleJoinSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleJoinSrf object
static class CCommandSampleJoinSrf theSampleJoinSrfCommand;

CRhinoCommand::result CCommandSampleJoinSrf::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoCommand::result rc = CRhinoCommand::success;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surfaces to join");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 0);
  rc = go.CommandResult();
  if (rc != CRhinoCommand::success)
    return go.CommandResult();

  ON_SimpleArray<ON_Brep*> InBreps(go.ObjectCount());
  int i;

  for (i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Brep* pBrep = go.Object(i).Brep();
    if (0 == pBrep)
    {
      rc = CRhinoCommand::failure;
      break;
    }

    ON_Brep* pDupe = pBrep->Duplicate();
    if (0 == pDupe)
    {
      rc = CRhinoCommand::failure;
      break;
    }

    InBreps.Append(pDupe);
  }

  if (rc != CRhinoCommand::success)
  {
    for (i = 0; i < InBreps.Count(); i++)
      delete InBreps[i]; // Don't leak...
    return rc;
  }

  ON_SimpleArray<ON_Brep*> OutBreps;
  bool bSuccess = RhinoJoinBreps(InBreps, OutBreps, context.m_doc.AbsoluteTolerance());

  if (bSuccess)
  {
    for (i = 0; i < OutBreps.Count(); i++)
    {
      CRhinoBrepObject* pBrepObj = new CRhinoBrepObject();
      pBrepObj->SetBrep(OutBreps[i]);
      OutBreps[i] = 0;
      if (!context.m_doc.AddObject(pBrepObj))
        delete pBrepObj; // Don't leak...
    }

    context.m_doc.Redraw();
  }

  for (i = 0; i < InBreps.Count(); i++)
    delete InBreps[i]; // Don't leak...

  return CRhinoCommand::success;
}

//
// END SampleJoinSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
