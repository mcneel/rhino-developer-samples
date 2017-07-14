#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCPlaneObject command
//

#pragma region SampleCPlaneObject command

class CCommandSampleCPlaneObject : public CRhinoCommand
{
public:
  CCommandSampleCPlaneObject() = default;
  ~CCommandSampleCPlaneObject() = default;
  UUID CommandUUID() override
  {
    // {ADC35DF0-D159-4C3D-AE04-8A1A6698D1BA}
    static const GUID SampleCPlaneObjectCommand_UUID =
    { 0xADC35DF0, 0xD159, 0x4C3D, { 0xAE, 0x04, 0x8A, 0x1A, 0x66, 0x98, 0xD1, 0xBA } };
    return SampleCPlaneObjectCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCPlaneObject"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context);
};

// The one and only CCommandSampleCPlaneObject object
static class CCommandSampleCPlaneObject theSampleCPlaneObjectCommand;

CRhinoCommand::result CCommandSampleCPlaneObject::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoView* view = RhinoApp().ActiveView();
  if (0 == view)
    return CRhinoCommand::failure;

  ON_3dmConstructionPlane cplane = view->Viewport().ConstructionPlane();

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object to orient CPlane to");
  go.SetGeometryFilter(CRhinoGetObject::curve_object | CRhinoGetObject::surface_object | CRhinoGetObject::point_object | ON::clipplane_object);
  go.EnablePreSelect();
  go.EnableSubObjectSelect();
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& objref = go.Object(0);

  ON_Plane plane;
  bool rc = false;
  const ON_Curve* curve = nullptr;
  const ON_Surface* surface = nullptr;
  const ON_Point* point = nullptr;
  const CRhinoClippingPlaneObject* clipper = nullptr;

  if (nullptr != (curve = objref.Curve()))
  {
    rc = curve->IsPlanar(&plane, context.m_doc.AbsoluteTolerance());
    if (rc)
      plane.SetOrigin(curve->PointAtStart());
    else
      rc = curve->FrameAt(curve->Domain().Min(), plane);
  }
  else if (!rc && nullptr != (surface = objref.Surface()))
  {
    rc = surface->FrameAt(surface->Domain(0).Mid(), surface->Domain(1).Mid(), plane);
  }
  else if (!rc && nullptr != (point = objref.Point()))
  {
    plane = cplane.m_plane;
    plane.SetOrigin(point->point);
    rc = TRUE;
  }
  else if (!rc && nullptr != (clipper = CRhinoClippingPlaneObject::Cast(objref.Object())))
  {
    plane = clipper->ClippingPlane().m_plane;
    rc = TRUE;
  }

  if (!rc)
  {
    // One last try...
    const CRhinoObject* object = objref.Object();
    if (object)
      rc = object->GetFrame(plane);
  }

  if (rc)
  {
    cplane.m_plane = plane;
    view->Viewport().SetConstructionPlane(cplane);
    view->Redraw();
  }

  return (rc) ? CRhinoCommand::success : CRhinoCommand::nothing;
}

#pragma endregion

//
// END SampleCPlaneObject command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
