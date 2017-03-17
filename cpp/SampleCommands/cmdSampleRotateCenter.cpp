#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRotateCenter command
//

class CCommandSampleRotateCenter : public CRhinoCommand
{
public:
  CCommandSampleRotateCenter() { m_angle = 0.0; }
  ~CCommandSampleRotateCenter() {}
  UUID CommandUUID()
  {
    // {188044C6-7177-4956-A1A6-FAC9B7AAE436}
    static const GUID SampleRotateCenterCommand_UUID =
    { 0x188044C6, 0x7177, 0x4956, { 0xA1, 0xA6, 0xFA, 0xC9, 0xB7, 0xAA, 0xE4, 0x36 } };
    return SampleRotateCenterCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleRotateCenter"; }
  const wchar_t* LocalCommandName() { return L"SampleRotateCenter"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  double m_angle;
};

// The one and only CCommandSampleRotateCenter object
static class CCommandSampleRotateCenter theSampleRotateCenterCommand;

CRhinoCommand::result CCommandSampleRotateCenter::RunCommand(const CRhinoCommandContext& context)
{
  // Select objects to rotate
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to rotate");
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Rotation angle (in degrees)
  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Rotation angle");
  gn.SetDefaultNumber(m_angle);
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  // Validate input
  double angle = gn.Number();
  if (angle == 0)
    return CRhinoCommand::nothing;

  m_angle = angle;

  // Get the active view's construction plane
  ON_Plane plane = RhinoActiveCPlane();

  int i;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    // Do not split objects that get kinky 
    // when they are transformed.
    CRhinoKeepKinkySurfaces keep_kinky_srfs;

    // Get an object reference
    const CRhinoObjRef& ref = go.Object(i);

    // Get the real object
    const CRhinoObject* obj = ref.Object();
    if (!obj)
      continue;

    // Get the object's tight bounding box
    ON_BoundingBox bbox;
    if (!obj->GetTightBoundingBox(bbox, false, 0))
      continue;

    // Create transformation matrix
    ON_Xform xform;
    xform.Rotation(m_angle * ON_PI / 180.0, plane.zaxis, bbox.Center());

    // Transform the object
    context.m_doc.TransformObject(obj, xform, true, true, true);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleRotateCenter command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
