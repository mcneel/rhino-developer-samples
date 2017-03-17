#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSplitBrep command
//

class CCommandSampleSplitBrep : public CRhinoCommand
{
public:
  CCommandSampleSplitBrep() {}
  ~CCommandSampleSplitBrep() {}
  UUID CommandUUID()
  {
    // {581D3D84-631A-4283-ACA3-6FEE82C6BB49}
    static const GUID SampleSplitBrepCommand_UUID =
    { 0x581D3D84, 0x631A, 0x4283, { 0xAC, 0xA3, 0x6F, 0xEE, 0x82, 0xC6, 0xBB, 0x49 } };
    return SampleSplitBrepCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSplitBrep"; }
  const wchar_t* LocalCommandName() { return L"SampleSplitBrep"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSplitBrep object
static class CCommandSampleSplitBrep theSampleSplitBrepCommand;

CRhinoCommand::result CCommandSampleSplitBrep::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface or polysurface to split");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObject* brep_obj = go.Object(0).Object();
  const ON_Brep* brep = go.Object(0).Brep();
  if (0 == brep_obj || 0 == brep)
    return CRhinoCommand::failure;

  go.SetCommandPrompt(L"Select cutting surface or polysurface");
  go.EnablePreSelect(false);
  go.EnableDeselectAllBeforePostSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Brep* cutter = go.Object(0).Brep();
  if (0 == cutter)
    return CRhinoCommand::failure;

  double tol = context.m_doc.AbsoluteTolerance();

  ON_SimpleArray<ON_Brep*> pieces;
  if (RhinoBrepSplit(*brep, *cutter, tol, pieces))
  {
    const CRhinoObjectAttributes& attribs = brep_obj->Attributes();

    int i;
    for (i = 0; i < pieces.Count(); i++)
    {
      CRhinoBrepObject* piece_obj = new CRhinoBrepObject(attribs);
      piece_obj->SetBrep(pieces[i]);
      context.m_doc.AddObject(piece_obj);
    }

    context.m_doc.DeleteObject(CRhinoObjRef(brep_obj));
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleSplitBrep command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
