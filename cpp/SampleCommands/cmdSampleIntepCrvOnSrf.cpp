#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleIntepCrvOnSrf command
//

class CCommandSampleIntepCrvOnSrf : public CRhinoCommand
{
public:
  CCommandSampleIntepCrvOnSrf() {}
  ~CCommandSampleIntepCrvOnSrf() {}
  UUID CommandUUID()
  {
    // {2479F956-852B-4E7D-8437-684C2705777C}
    static const GUID SampleIntepCrvOnSrfCommand_UUID =
    { 0x2479F956, 0x852B, 0x4E7D, { 0x84, 0x37, 0x68, 0x4C, 0x27, 0x05, 0x77, 0x7C } };
    return SampleIntepCrvOnSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleIntepCrvOnSrf"; }
  const wchar_t* LocalCommandName() const { return L"SampleIntepCrvOnSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleIntepCrvOnSrf object
static class CCommandSampleIntepCrvOnSrf theSampleIntepCrvOnSrfCommand;

CRhinoCommand::result CCommandSampleIntepCrvOnSrf::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface to draw curve on");
  go.SetGeometryFilter(CRhinoObject::surface_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObject* obj = go.Object(0).Object();
  const ON_BrepFace* face = go.Object(0).Face();
  if (0 == obj || 0 == face)
    return CRhinoCommand::failure;

  ON_SimpleArray<ON_2dPoint> points2d(25);

  CRhinoGetPoint gp;
  gp.Constrain(*face, obj->Attributes().m_wire_density);

  for (;;)
  {
    gp.ClearCommandOptions();

    if (points2d.Count() < 1)
      gp.SetCommandPrompt(L"Start of curve");
    else if (points2d.Count() == 1)
      gp.SetCommandPrompt(L"Next point");
    else
      gp.SetCommandPrompt(L"Next point. Press Enter when done");

    gp.AcceptUndo(points2d.Count() > 0);

    gp.AcceptNothing();

    CRhinoGet::result res = gp.GetPoint();

    if (res == CRhinoGet::cancel)
      return CRhinoCommand::cancel;

    else if (res == CRhinoGet::undo)
    {
      points2d.Remove(); // remove last point
      continue;
    }
    else if (res == CRhinoGet::point)
    {
      ON_2dPoint pt;
      if (gp.PointOnSurface(&pt.x, &pt.y))
      {
        int count = points2d.Count();
        if (count > 0)
        {
          ON_2dVector v = *points2d.At(count - 1) - pt;
          if (v.IsTiny())
            continue;
        }

        points2d.Append(pt);
      }
      continue;
    }

    break;
  }

  const int count = points2d.Count();
  if (count > 1)
  {
    // Check for closed curve
    int is_closed = 0;
    if (count > 3)
    {
      ON_2dPoint pt = points2d[0];
      if (pt.DistanceTo(points2d[count - 1]) < ON_SQRT_EPSILON)
        is_closed = 1;
    }

    double tol = context.m_doc.AbsoluteTolerance();
    ON_Curve* crv = RhinoInterpolatePointsOnSurface(*face, points2d, is_closed, tol, 1);
    if (crv)
    {
      CRhinoCurveObject* crv_obj = new CRhinoCurveObject();
      crv_obj->SetCurve(crv);
      context.m_doc.AddObject(crv_obj);
      context.m_doc.Redraw();
    }

    return CRhinoCommand::success;
  }

  return CRhinoCommand::success;
}

//
// END SampleIntepCrvOnSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
