#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMove command
//

class CRhinoGetTranslationPoint : public CRhinoGetXform
{
public:
  CRhinoGetTranslationPoint() {}
  ~CRhinoGetTranslationPoint() {}
  BOOL CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform);
};

BOOL CRhinoGetTranslationPoint::CalculateTransform(CRhinoViewport& vp, const ON_3dPoint& pt, ON_Xform& xform)
{
  UNREFERENCED_PARAMETER(vp);
  ON_3dVector dir = pt - m_basepoint;
  if (dir.IsTiny())
    xform.Identity();
  else
    xform.Translation(dir);
  return (xform.IsValid()) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleMove : public CRhinoCommand
{
public:
  CCommandSampleMove() {}
  ~CCommandSampleMove() {}
  UUID CommandUUID()
  {
    // {B36D0E5E-5A57-4440-99D0-A96F63022064}
    static const GUID SampleMoveCommand_UUID =
    { 0xB36D0E5E, 0x5A57, 0x4440, { 0x99, 0xD0, 0xA9, 0x6F, 0x63, 0x02, 0x20, 0x64 } };
    return SampleMoveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMove"; }
  const wchar_t* LocalCommandName() { return L"SampleMove"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMove object
static class CCommandSampleMove theSampleMoveCommand;

CRhinoCommand::result CCommandSampleMove::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to move");
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point to move from");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  CRhinoGetTranslationPoint gt;
  gt.SetCommandPrompt(L"Point to move to");
  gt.SetBasePoint(gp.Point());
  gt.DrawLineFromPoint(gp.Point(), TRUE);
  gt.AppendObjects(go);
  gt.GetXform();
  if (gt.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  CRhinoView* view = gt.View();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_Xform xform;
  if (gt.CalculateTransform(view->ActiveViewport(), gt.Point(), xform))
  {
    for (int i = 0; i < go.ObjectCount(); i++)
    {
      CRhinoObjRef obj_ref = go.Object(i);
      context.m_doc.TransformObject(obj_ref, xform);
    }
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleMove command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
