#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSurfaceCurvature command
//

#pragma region SampleSurfaceCurvature command

class CCommandSampleSurfaceCurvature : public CRhinoCommand
{
public:
  CCommandSampleSurfaceCurvature() = default;
  UUID CommandUUID() override
  {
    // {D40D37D7-76BD-4020-A451-548ED580B35C}
    static const GUID SampleSurfaceCurvatureCommand_UUID =
    { 0xD40D37D7, 0x76BD, 0x4020, { 0xA4, 0x51, 0x54, 0x8E, 0xD5, 0x80, 0xB3, 0x5C } };
    return SampleSurfaceCurvatureCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSurfaceCurvature"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSurfaceCurvature object
static class CCommandSampleSurfaceCurvature theSampleSurfaceCurvatureCommand;

CRhinoCommand::result CCommandSampleSurfaceCurvature::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);

  double s = ON_UNSET_VALUE;
  double t = ON_UNSET_VALUE;
  const ON_Surface* srf = obj_ref.SurfaceParameter(&s, &t);
  if (nullptr == srf || !ON_IsValid(s) || !ON_IsValid(t))
    return CRhinoCommand::failure;

  ON_3dPoint P;
  ON_3dVector Ds, Dt, Dss, Dst, Dtt;
  double gauss = ON_UNSET_VALUE;
  double mean = ON_UNSET_VALUE;
  double k[2] = { ON_UNSET_VALUE , ON_UNSET_VALUE };
  ON_3dVector K[2];
  if (!srf->Ev2Der(s, t, P, Ds, Dt, Dss, Dst, Dtt))
    return CRhinoCommand::failure; // failed to evaluate derivatives

  ON_3dVector N;
  if (!srf->EvNormal(s, t, N))
    return CRhinoCommand::failure; // failed to evaluate normal

  if (!ON_EvPrincipalCurvatures(Ds, Dt, Dss, Dst, Dtt, N, &gauss, &mean, &k[0], &k[1], K[0], K[1]))
    return CRhinoCommand::failure; // failed to evaluate principal curvatures

  ON_wString str;
  RhinoApp().Print(L"Surface curvature evaluation at parameter: (%g, %g)\n", s, t);

  str.Empty();
  RhinoFormatPoint(P, str);
  RhinoApp().Print(L"  3-D Point: (%s)\n", static_cast<const wchar_t*>(str));

  str.Empty();
  RhinoFormatPoint(N, str);
  RhinoApp().Print(L"  3-D Normal: (%s)\n", static_cast<const wchar_t*>(str));

  str.Empty();
  RhinoFormatPoint(K[0], str);
  RhinoApp().Print(L"  Maximum principal curvature: %g (%s)\n", k[0], static_cast<const wchar_t*>(str));

  str.Empty();
  RhinoFormatPoint(K[1], str);
  RhinoApp().Print(L"  Minimum principal curvature: %g (%s)\n", k[1], static_cast<const wchar_t*>(str));

  RhinoApp().Print(L"  Gaussian curvature: %g\n", gauss);
  RhinoApp().Print(L"  Mean curvature: %g\n", mean);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSurfaceCurvature command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
