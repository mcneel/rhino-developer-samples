#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCurveTorsion command
//

#pragma region SampleCurveTorsion command

// See O'Neill, Elem. Diff. Geom, pg 69
double CurveTorsion(const ON_Curve& crv, double t)
{
  ON_3dVector deriv[4];
  crv.Evaluate(t, 3, 3, deriv[0]);

  ON_3dVector b = ON_CrossProduct(deriv[1], deriv[2]);
  double len2 = b*b;

  double torsion = 0.0;
  if (len2 > 0)
    torsion = b * deriv[3] / len2;

  return torsion;
}

class CCommandSampleCurveTorsion : public CRhinoCommand
{
public:
  CCommandSampleCurveTorsion() {}
  ~CCommandSampleCurveTorsion() {}
  UUID CommandUUID()
  {
    // {70579BBE-CE4F-4500-B95A-7ECD2128CD53}
    static const GUID SampleCurveTorsionCommand_UUID =
    { 0x70579BBE, 0xCE4F, 0x4500, { 0xB9, 0x5A, 0x7E, 0xCD, 0x21, 0x28, 0xCD, 0x53 } };
    return SampleCurveTorsionCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCurveTorsion"; }
  const wchar_t* LocalCommandName() const { return L"SampleCurveTorsion"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCurveTorsion object
static class CCommandSampleCurveTorsion theSampleCurveTorsionCommand;

CRhinoCommand::result CCommandSampleCurveTorsion::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve for torsion evaluation");
  go.SetGeometryFilter(CRhinoObject::curve_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* curve = go.Object(0).Curve();
  if (0 == curve)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point on curve");
  gp.Constrain(*curve);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint point = gp.Point();
  double t = ON_UNSET_VALUE;
  if (curve->GetClosestPoint(point, &t))
  {
    if (ON_IsValid(t))
    {
      double torsion = CurveTorsion(*curve, t);
      RhinoApp().Print(L"Torsion at curve parameter(%g) = %g\n", t, torsion);
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCurveTorsion command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
