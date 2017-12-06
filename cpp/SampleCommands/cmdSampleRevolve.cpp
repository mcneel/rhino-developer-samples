#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRevolve command
//

#pragma region SampleRevolve command

class CCommandSampleRevolve : public CRhinoCommand
{
public:
  CCommandSampleRevolve() = default;
  UUID CommandUUID() override
  {
    // {DB1996C0-F43B-4617-8E31-4438D7EBBDA6}
    static const GUID SampleRevolveCommand_UUID =
    { 0xDB1996C0, 0xF43B, 0x4617,{ 0x8E, 0x31, 0x44, 0x38, 0xD7, 0xEB, 0xBD, 0xA6 } };
    return SampleRevolveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleRevolve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleRevolve object
static class CCommandSampleRevolve theSampleRevolveCommand;

CRhinoCommand::result CCommandSampleRevolve::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to rotate around world x-y axis");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* curve = go.Object(0).Curve();
  if (nullptr == curve)
    return CRhinoCommand::failure;

  ON_3dPoint start(0.0, 0.0, 0.0);
  ON_3dPoint end(0.0, 0.0, 1.0);
  ON_Line axis(start, end);

  ON_Interval domain(0.0, ON_PI * 2.0);

  ON_Curve* curve_copy = curve->Duplicate();

  ON_PolyCurve* poly_curve = ON_PolyCurve::Cast(curve_copy);
  if (nullptr != poly_curve)
    poly_curve->RemoveNesting();

  curve_copy->RemoveShortSegments(ON_ZERO_TOLERANCE, true);

  // Do the revolve
  ON_Surface* surface = RhinoRevolve(curve_copy, axis, domain);

  delete curve_copy; // don't leak...
  curve_copy = nullptr;

  ON_RevSurface* rev_surface = ON_RevSurface::Cast(surface);
  if (nullptr != rev_surface)
  {
    // Convert results to NURBS
    ON_NurbsSurface* nurbs_surface = ON_NurbsSurface::New();
    if (rev_surface->GetNurbForm(*nurbs_surface))
    {
      rev_surface = nullptr;
      delete surface;
      surface = nurbs_surface;
    }
    else
    {
      delete nurbs_surface;
    }
  }

  if (nullptr != surface)
  {
    CRhinoSurfaceObject* surface_obj = new CRhinoSurfaceObject();
    surface_obj->SetSurface(surface);
    context.m_doc.AddObject(surface_obj);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRevolve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
