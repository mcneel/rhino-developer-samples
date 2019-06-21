#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOffsetCrvOnSrf command
//

#pragma region SampleOffsetCrvOnSrf command

class CCommandSampleOffsetCrvOnSrf : public CRhinoCommand
{
public:
  CCommandSampleOffsetCrvOnSrf() = default;
  UUID CommandUUID() override
  {
    // {BD3ADA9C-C629-4366-BFDD-A64D56A13697}
    static const GUID SampleOffsetCrvOnSrfCommand_UUID =
    { 0xBD3ADA9C, 0xC629, 0x4366, { 0xBF, 0xDD, 0xA6, 0x4D, 0x56, 0xA1, 0x36, 0x97 } };
    return SampleOffsetCrvOnSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleOffsetCrvOnSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  double m_distance = 1.0;
};

// The one and only CCommandSampleOffsetCrvOnSrf object
static class CCommandSampleOffsetCrvOnSrf theSampleOffsetCrvOnSrfCommand;

CRhinoCommand::result CCommandSampleOffsetCrvOnSrf::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface edge to offset");
  go.SetGeometryFilter(CRhinoGetObject::edge_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  double crv_t = ON_UNSET_VALUE;
  const ON_Curve* crv = obj_ref.CurveParameter(&crv_t);
  if (nullptr == crv)
    return CRhinoCommand::failure;

  const ON_BrepTrim* trim = obj_ref.Trim();
  if (nullptr == trim)
    return CRhinoCommand::failure;

  const ON_BrepFace* face = trim->Face();
  if (nullptr == face)
    return CRhinoCommand::failure;

  const ON_Brep* brep = face->Brep();
  if (nullptr == brep)
    return CRhinoCommand::failure;

  bool bFlip = trim->m_bRev3d;
  double distance = m_distance;

  CRhinoGetNumber gd;
  gd.SetCommandPrompt(L"Offset distance");
  gd.SetDefaultNumber(distance);
  gd.AddCommandOptionToggle(RHCMDOPTNAME(L"Flip"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bFlip, &bFlip);
  gd.AcceptNothing();
  for (;;)
  {
    CRhinoGet::result res = gd.GetNumber();
    if (res == CRhinoGet::number)
    {
      distance = gd.Number();
      break;
    }
    if (res == CRhinoGet::nothing)
      break;
    if (res == CRhinoGet::option)
      continue;
    return CRhinoCommand::cancel;
  }

  double dist = distance;
  if (face->m_bRev)
    dist = -dist;
  if (bFlip)
    dist = -dist;

  double tol = context.m_doc.AbsoluteTolerance();
  ON_SimpleArray<ON_Curve*> result_curves;
  CRhinoCommand::result offset_rc = RhinoOffsetCurveOnSrf(crv, brep, face->m_face_index, dist, tol, result_curves);

  if (offset_rc == CRhinoCommand::success)
  {
    // RhinoMergeCurves requries an array of const curve*
    ON_SimpleArray<const ON_Curve*> curves_to_join(result_curves.Count());
    ON_SimpleArray<ON_Curve*> joined_curves;
    for (int i = 0; i < result_curves.Count(); i++)
      curves_to_join.Append(result_curves[i]);

    BOOL join_rc = RhinoMergeCurves(curves_to_join, joined_curves, 2.0*tol, TRUE);
    for (int i = 0; i < curves_to_join.Count(); i++)
      curves_to_join[i] = nullptr;

    if (join_rc)
    {
      for (int i = 0; i < joined_curves.Count(); i++)
      {
        // Extend both ends to edge of the surface
        if (!joined_curves[i]->IsClosed())
          RhinoExtendCrvOnSrf(*face, joined_curves[i]);

        CRhinoCurveObject* crv_obj = new CRhinoCurveObject();
        crv_obj->SetCurve(joined_curves[i]);
        context.m_doc.AddObject(crv_obj);
        joined_curves[i] = nullptr;
      }
    }
  }

  for (int i = 0; i < result_curves.Count(); i++)
  {
    delete result_curves[i];
    result_curves[i] = nullptr; // Don't leak...
  }

  m_distance = distance;

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleOffsetCrvOnSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
