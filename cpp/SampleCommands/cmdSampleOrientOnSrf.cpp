#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOrientOnSrf command
//

class CCommandSampleOrientOnSrf : public CRhinoCommand
{
public:
  CCommandSampleOrientOnSrf() {}
  ~CCommandSampleOrientOnSrf() {}
  UUID CommandUUID()
  {
    // {855DDA39-6B1-4D53-9A92-2B1ADDC21C98}
    static const GUID SampleOrientOnSrfCommand_UUID =
    { 0x855DDA39, 0x6B1, 0x4D53, { 0x9A, 0x92, 0x2B, 0x1A, 0xDD, 0xC2, 0x1C, 0x98 } };
    return SampleOrientOnSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleOrientOnSrf"; }
  const wchar_t* LocalCommandName() { return L"SampleOrientOnSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleOrientOnSrf object
static class CCommandSampleOrientOnSrf theSampleOrientOnSrfCommand;

CRhinoCommand::result CCommandSampleOrientOnSrf::RunCommand(const CRhinoCommandContext& context)
{
  // Select objects to orient
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to orient");
  go.EnableSubObjectSelect(FALSE);
  go.EnableGroupSelect(TRUE);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Point to orient from
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point to orient from");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  // Define source plane
  CRhinoView* view = gp.View();
  if (0 == view)
  {
    view = RhinoApp().ActiveView();
    if (0 == view)
      return CRhinoCommand::failure;
  }
  ON_Plane source_plane(view->Viewport().ConstructionPlane().m_plane);
  source_plane.SetOrigin(gp.Point());

  // Surface to orient on
  CRhinoGetObject gs;
  gs.SetCommandPrompt(L"Surface to orient on");
  gs.SetGeometryFilter(CRhinoGetObject::surface_object);
  gs.EnableSubObjectSelect(TRUE);
  gs.EnableDeselectAllBeforePostSelect(false);
  gs.EnableOneByOnePostSelect();
  gs.GetObjects(1, 1);
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  const CRhinoObjRef& ref = gs.Object(0);
  // Get selected surface object
  const CRhinoObject* obj = ref.Object();
  if (0 == obj)
    return CRhinoCommand::failure;
  // Get selected surface (face)
  const ON_BrepFace* face = ref.Face();
  if (0 == face)
    return CRhinoCommand::failure;
  // Unselect surface
  obj->Select(false);

  // Point on surface to orient to
  gp.SetCommandPrompt(L"Point on surface to orient to");
  gp.Constrain(*face);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  // Do transformation
  CRhinoCommand::result rc = CRhinoCommand::failure;
  double u = 0.0, v = 0.0;
  if (face->GetClosestPoint(gp.Point(), &u, &v))
  {
    ON_Plane target_plane;
    if (face->FrameAt(u, v, target_plane))
    {
      // If the face orientation is opposite
      // of natural surface orientation, then
      // flip the plane's zaxis.
      if (face->m_bRev)
        target_plane.CreateFromFrame(
          target_plane.origin,
          target_plane.xaxis,
          -target_plane.zaxis
        );

      // Build transformation
      ON_Xform xform;
      xform.Rotation(source_plane, target_plane);

      // Do the transformation. In this example,
      // we will copy the original objects
      bool bDeleteOriginal = false;
      int i;
      for (i = 0; i < go.ObjectCount(); i++)
      {
        CRhinoObjRef obj_ref = go.Object(i);
        context.m_doc.TransformObject(obj_ref, xform, bDeleteOriginal);
      }
      context.m_doc.Redraw();
      rc = CRhinoCommand::success;
    }
  }

  return rc;
}

//
// END SampleOrientOnSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
