#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePointsOnSurface command
//

#pragma region SamplePointsOnSurface command

class CCommandSamplePointsOnSurface : public CRhinoCommand
{
public:
  CCommandSamplePointsOnSurface();
  UUID CommandUUID() override
  {
    // {CFF946B7-1394-476E-8203-F85BAAD5BF2E}
    static const GUID SamplePointsOnSurfaceCommand_UUID =
    { 0xCFF946B7, 0x1394, 0x476E, { 0x82, 0x03, 0xF8, 0x5B, 0xAA, 0xD5, 0xBF, 0x2E } };
    return SamplePointsOnSurfaceCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePointsOnSurface"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  int m_u_count;
  int m_v_count;
};

// The one and only CCommandSamplePointsOnSurface object
static class CCommandSamplePointsOnSurface theSamplePointsOnSurfaceCommand;

CCommandSamplePointsOnSurface::CCommandSamplePointsOnSurface()
  : m_u_count(10)
  , m_v_count(10)
{
}

CRhinoCommand::result CCommandSamplePointsOnSurface::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface to place points on");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(TRUE);
  go.AddCommandOptionInteger(RHCMDOPTNAME(L"UPoints"), &m_u_count, L"Number of points in U direction", 2, 100);
  go.AddCommandOptionInteger(RHCMDOPTNAME(L"VPoints"), &m_v_count, L"Number of points in V direction", 2, 100);
  for (;;)
  {
    CRhinoGet::result res = go.GetObjects(1, 1);

    if (res == CRhinoGet::option)
      continue;

    if (res != CRhinoGet::object)
      return CRhinoCommand::cancel;

    break;
  }

  const CRhinoObjRef& obj_ref = go.Object(0);
  const ON_BrepFace* face = obj_ref.Face();
  if (nullptr == face)
    return CRhinoCommand::failure;

  // Create array of normalized u parameters
  ON_SimpleArray<double> u_array(m_u_count);
  u_array.SetCount(m_u_count);
  for (int i = 0; i < m_u_count; i++)
  {
    double u = (double)i / ((double)m_u_count - 1);
    u_array[i] = u;
  }

  // Create array of normalized v parameters
  ON_SimpleArray<double> v_array(m_v_count);
  v_array.SetCount(m_v_count);
  for (int i = 0; i < m_v_count; i++)
  {
    double v = (double)i / ((double)m_v_count - 1);
    v_array[i] = v;
  }

  ON_Interval u_domain = face->Domain(0);
  ON_Interval v_domain = face->Domain(1);

  double tol = context.m_doc.AbsoluteTolerance();

  for (int i = 0; i < u_array.Count(); i++)
  {
    // Convert normalized parameter to interval value
    double u = u_domain.ParameterAt(u_array[i]);

    for (int j = 0; j < v_array.Count(); j++)
    {
      // Convert normalized parameter to interval value
      double v = v_domain.ParameterAt(v_array[j]);

      // Verify point is in the active region of a face
      if (RhinoIsPointOnFace(*face, tol, false, u, v))
      {
        ON_3dPoint pt = face->PointAt(u, v);
        context.m_doc.AddPointObject(pt);
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePointsOnSurface command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
