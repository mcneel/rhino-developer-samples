#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSection command
//

class CCommandSampleSection : public CRhinoCommand
{
public:
  CCommandSampleSection() {}
  ~CCommandSampleSection() {}
  UUID CommandUUID()
  {
    // {72CF64A5-CDF5-47C2-ABBA-8135F6FEFA63}
    static const GUID SampleSectionCommand_UUID =
    { 0x72CF64A5, 0xCDF5, 0x47C2, { 0xAB, 0xBA, 0x81, 0x35, 0xF6, 0xFE, 0xFA, 0x63 } };
    return SampleSectionCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSection"; }
  const wchar_t* LocalCommandName() { return L"SampleSection"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSection object
static class CCommandSampleSection theSampleSectionCommand;

CRhinoCommand::result CCommandSampleSection::RunCommand(const CRhinoCommandContext& context)
{
  // Select surfaces and polysurfaces to section
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surfaces and polysurfaces to section");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.EnableGroupSelect();
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Create contour input object
  CRhinoContourInput input;
  input.m_bAddOutputToDocument = false; // we will do this

  // Add geometry to contour input
  int i;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& ref = go.Object(0);
    const CRhinoObject* obj = ref.Object();
    if (0 == obj)
      return CRhinoCommand::failure;

    input.m_geom.Append(obj->Geometry());
    input.m_attribs.Append(&obj->Attributes());
  }

  // Start of section
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Start of section");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint basept = gp.Point();

  // End of section
  gp.SetCommandPrompt(L"End of section");
  gp.SetBasePoint(basept);
  gp.DrawLineFromPoint(basept, TRUE);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint endpt = gp.Point();

  // Define a plane such that the input points define
  // the plane's xaxis and the zaxis of the active view's 
  // construction plane will define the plane's yaxis.
  ON_Plane view_plane = RhinoActiveCPlane();
  basept = view_plane.ClosestPointTo(basept);
  endpt = view_plane.ClosestPointTo(endpt);
  ON_Plane plane(basept, endpt - basept, view_plane.zaxis);

  // Offset the endpt from basept in the plane's normal direction
  ON_3dVector zaxis = plane.zaxis;
  zaxis.Unitize();
  endpt = basept + zaxis;

  // Finish filling out the contour input
  input.m_basept = basept;
  input.m_endpt = endpt;
  input.m_limit_range = true;

  // To limit contouring to just one section, specify an 
  // interval larger than the distance between basept and endpt.
  input.m_interval = 100.0;

  // Make the contours (sections)
  ON_SimpleArray<ON_Polyline*> pline_array;
  ON_SimpleArray<ON_Curve*> curve_array;
  ON_SimpleArray<ON_3dPoint> point_array;
  if (!MakeRhinoContours(input, pline_array, curve_array, point_array))
    return CRhinoCommand::nothing;

  // Add the section curves to the document
  for (i = 0; i < curve_array.Count(); i++)
  {
    ON_Curve* crv = curve_array[i];
    if (crv)
      context.m_doc.AddCurveObject(*crv);
  }

  // We are responsible for cleanign up the section curves
  for (i = 0; i < curve_array.Count(); i++)
  {
    delete curve_array[i];
    curve_array[i] = 0;
  }

  // We are responsible for cleanign up the section polylines
  // (created from sectioning a mesh)
  for (i = 0; i < pline_array.Count(); i++)
  {
    delete pline_array[i];
    pline_array[i] = 0;
  }

  if (curve_array.Count())
    context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleSection command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
