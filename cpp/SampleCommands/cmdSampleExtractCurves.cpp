#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtractCurves command
//

#pragma region SampleExtractCurves command

class CCommandSampleExtractCurves : public CRhinoCommand
{
public:
  CCommandSampleExtractCurves();
  ~CCommandSampleExtractCurves() {}
  UUID CommandUUID()
  {
    // {D1021FFA-FA7E-4E14-B50C-0F3B68AD2FEC}
    static const GUID SampleExtractCurvesCommand_UUID =
    { 0xD1021FFA, 0xFA7E, 0x4E14, { 0xB5, 0x0C, 0x0F, 0x3B, 0x68, 0xAD, 0x2F, 0xEC } };
    return SampleExtractCurvesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExtractCurves"; }
  const wchar_t* LocalCommandName() const { return L"SampleExtractCurves"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  double m_row_count;
  double m_col_count;
};

// The one and only CCommandSampleExtractCurves object
static class CCommandSampleExtractCurves theSampleExtractCurvesCommand;

CCommandSampleExtractCurves::CCommandSampleExtractCurves()
{
  m_row_count = 2;
  m_col_count = 2;
}

CRhinoCommand::result CCommandSampleExtractCurves::RunCommand(const CRhinoCommandContext& context)
{
  // Note, to extract isocurves from trimmed faces, use RhinoGetBrepFaceIsoCurves().

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select untrimmed surface for isocurve extraction");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::untrimmed_surface);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Surface* srf = go.Object(0).Surface();
  if (0 == srf)
    return CRhinoCommand::failure;

  CRhinoGetInteger gu;
  gu.SetCommandPrompt(L"Number of rows (V)");
  gu.SetDefaultNumber(m_row_count);
  gu.SetLowerLimit(2);
  gu.SetUpperLimit(100);
  gu.GetInteger();
  if (gu.CommandResult() != CRhinoCommand::success)
    return gu.CommandResult();

  int row_count = gu.Number();

  CRhinoGetInteger gv;
  gv.SetCommandPrompt(L"Number of columns (U)");
  gv.SetDefaultNumber(m_col_count);
  gv.SetLowerLimit(2);
  gv.SetUpperLimit(100);
  gv.GetInteger();
  if (gv.CommandResult() != CRhinoCommand::success)
    return gv.CommandResult();

  int col_count = gv.Number();

  ON_Interval dom_u = srf->Domain(0);
  ON_Interval dom_v = srf->Domain(1);

  int i = 0;

  for (i = 0; i < row_count; i++)
  {
    double nv = (double)i / ((double)row_count - 1);
    double v = dom_v.ParameterAt(nv);
    ON_Curve* crv = srf->IsoCurve(0, v);
    if (0 != crv)
    {
      CRhinoCurveObject* crv_obj = new CRhinoCurveObject();
      crv_obj->SetCurve(crv);
      if (context.m_doc.AddObject(crv_obj))
        crv_obj->Select();
      else
        delete crv_obj;
    }
  }

  for (i = 0; i < col_count; i++)
  {
    double nu = (double)i / ((double)col_count - 1);
    double u = dom_u.ParameterAt(nu);
    ON_Curve* crv = srf->IsoCurve(1, u);
    if (0 != crv)
    {
      CRhinoCurveObject* crv_obj = new CRhinoCurveObject();
      crv_obj->SetCurve(crv);
      if (context.m_doc.AddObject(crv_obj))
        crv_obj->Select();
      else
        delete crv_obj;
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleExtractCurves command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
