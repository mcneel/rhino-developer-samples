#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtractIsoCurve command
//

class CCommandSampleExtractIsoCurve : public CRhinoCommand
{
public:
  CCommandSampleExtractIsoCurve() {}
  ~CCommandSampleExtractIsoCurve() {}
  UUID CommandUUID()
  {
    // {58D7F9B0-DAA7-4631-BCF7-15DCD109803E}
    static const GUID SampleExtractIsoCurveCommand_UUID =
    { 0x58D7F9B0, 0xDAA7, 0x4631, { 0xBC, 0xF7, 0x15, 0xDC, 0xD1, 0x09, 0x80, 0x3E } };
    return SampleExtractIsoCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExtractIsoCurve"; }
  const wchar_t* LocalCommandName() { return L"SampleExtractIsoCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExtractIsoCurve object
static class CCommandSampleExtractIsoCurve theSampleExtractIsoCurveCommand;

CRhinoCommand::result CCommandSampleExtractIsoCurve::RunCommand(const CRhinoCommandContext& context)
{
  // Select the surface to extract isocurve
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Validate selection
  const CRhinoObjRef& ref = go.Object(0);
  const ON_Surface* srf = ref.Surface();
  if (!srf)
    return CRhinoCommand::failure;

  ON_3dPoint pt(ON_UNSET_POINT);
  BOOL dir = FALSE;

  // Pick a point on the surface
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point on surface");
  gp.AddCommandOptionToggle(RHCMDOPTNAME(L"Direction"), RHCMDOPTVALUE(L"U"), RHCMDOPTVALUE(L"V"), dir, &dir);
  gp.Constrain(*srf);
  for (;;)
  {
    CRhinoGet::result res = gp.GetPoint();
    if (res == CRhinoGet::point)
    {
      pt = gp.Point();
      break;
    }
    else if (res == CRhinoGet::option)
      continue;
    else
      return CRhinoCommand::cancel;
  }

  // Get the parameters of the point on
  // the surface that is closest to pt.
  double u, v;
  if (srf->GetClosestPoint(pt, &u, &v))
  {
    // Get the isoparametric curve. ON_Surface::IsoCurve
    // allocates memory for the resulting curve that we
    // will be responsible for.
    ON_Curve* crv = srf->IsoCurve(dir, dir ? u : v);
    if (crv)
    {
      context.m_doc.AddCurveObject(*crv);

      // CRhinoDoc::AddCurveObject make a copy of the input curve.
      // So, we need to delete crv otherwise we will leak memory.
      delete crv;
      crv = 0;

      context.m_doc.Redraw();
    }
  }

  return CRhinoCommand::success;
}

//
// END SampleExtractIsoCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
