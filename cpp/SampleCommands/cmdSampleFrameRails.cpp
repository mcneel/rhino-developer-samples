#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleFrameRails command
//

#pragma region SampleFrameRails command

class CCommandSampleFrameRails : public CRhinoCommand
{
public:
  CCommandSampleFrameRails() = default;
  UUID CommandUUID() override
  {
    // {FA97EEDF-63-451C-BD3A-C43F48BC617F}
    static const GUID SampleFrameRailsCommand_UUID =
    { 0xFA97EEDF, 0x63, 0x451C, { 0xBD, 0x3A, 0xC4, 0x3F, 0x48, 0xBC, 0x61, 0x7F } };
    return SampleFrameRailsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleFrameRails"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  bool GetCurveFrames(const ON_Curve& curve, int count, ON_SimpleArray<ON_Plane>& frames);
  ON_3dVector GetArbitraryAxis(const ON_3dVector& axis);
};

// The one and only CCommandSampleFrameRails object
static class CCommandSampleFrameRails theSampleFrameRailsCommand;

CRhinoCommand::result CCommandSampleFrameRails::RunCommand(const CRhinoCommandContext& context)
{
  int count = 10;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.AddCommandOptionInteger(RHCMDOPTNAME(L"Frames"), &count, L"Number of curve frames", 1, 100);
  for (;;)
  {
    CRhinoGet::result res = go.GetObjects(1, 1);
    if (res == CRhinoGet::option)
      continue;
    if (res != CRhinoGet::object)
      return CRhinoCommand::cancel;
    break;
  }

  const ON_Curve* curve = go.Object(0).Curve();
  if (nullptr == curve)
    return CRhinoCommand::failure;

  ON_SimpleArray<ON_Plane> frames(count);
  if (!GetCurveFrames(*curve, count, frames))
    return CRhinoCommand::failure;

  for (int i = 0; i < frames.Count(); i++)
  {
    ON_Line line(frames[i].origin, frames[i].origin + frames[i].xaxis);
    context.m_doc.AddCurveObject(line);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

/*
Description:
  Get relatively parallel frames at selected points along a curve.
*/
bool CCommandSampleFrameRails::GetCurveFrames(const ON_Curve& curve, int count, ON_SimpleArray<ON_Plane>& frames)
{
  if (!curve.IsValid())
    return false;

  ON_3dVector zaxis = curve.TangentAt(curve.Domain().Min());
  ON_3dVector xaxis = GetArbitraryAxis(zaxis);

  ON_SimpleArray<double> curve_t(count);
  for (int i = 0; i < count; i++)
  {
    double s = (double)i / (count - 1);
    double t = ON_UNSET_VALUE;
    if (curve.GetNormalizedArcLengthPoint(s, &t))
      curve_t.Append(t);
  }

  return RhinoGet1RailFrames(curve, curve_t.Count(), curve_t.Array(), frames, xaxis);
}

/*
Description:
  Given a unit vector to be used as one of the axes of a coordinate system, choose a second axis.
  To get a right-handed system, treat the axis returned by this function as the cyclically next 
  one after the given axis (e.g., if you give it a Z-axis, it returns a suitable X-axis).
*/
ON_3dVector CCommandSampleFrameRails::GetArbitraryAxis(const ON_3dVector& axis)
{
  static double bounds = 0.015625; // 1/64
  ON_3dVector rc;
  if (fabs(axis.x) < bounds && fabs(axis.y) < bounds)
    rc = ON_CrossProduct(ON_3dVector::YAxis, axis);
  else
    rc = ON_CrossProduct(ON_3dVector::ZAxis, axis);
  rc.Unitize();
  return rc;
}

#pragma endregion

//
// END SampleFrameRails command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
