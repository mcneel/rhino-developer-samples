#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtractEdgeCurves command
//

#pragma region SampleExtractEdgeCurves command

class CCommandSampleExtractEdgeCurves : public CRhinoCommand
{
public:
  CCommandSampleExtractEdgeCurves() {}
  ~CCommandSampleExtractEdgeCurves() {}
  UUID CommandUUID()
  {
    // {9DB6743D-FF4B-4D87-9915-4E41346A1681}
    static const GUID SampleExtractEdgeCurvesCommand_UUID =
    { 0x9DB6743D, 0xFF4B, 0x4D87, { 0x99, 0x15, 0x4E, 0x41, 0x34, 0x6A, 0x16, 0x81 } };
    return SampleExtractEdgeCurvesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExtractEdgeCurves"; }
  const wchar_t* LocalCommandName() const { return L"SampleExtractEdgeCurves"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExtractEdgeCurves object
static class CCommandSampleExtractEdgeCurves theSampleExtractEdgeCurvesCommand;

static int RhCopyBrepFaceLoopCurves(const ON_Brep* brep, ON_SimpleArray<ON_Curve*>& out_curves)
{
  if (NULL == brep)
    return 0;

  const int out_curves_Count = out_curves.Count();

  for (int fi = 0; fi < brep->m_F.Count(); fi++)
  {
    const ON_BrepFace& face = brep->m_F[fi];
    for (int fli = 0; fli < face.LoopCount(); fli++)
    {
      ON_BrepLoop* loop = face.Loop(fli);
      if (NULL == loop)
        continue;

      for (int lti = 0; lti < loop->TrimCount(); lti++)
      {
        ON_BrepTrim* trim = loop->Trim(lti);
        if (NULL == trim)
          continue;

        // This will be NULL for singular trims.
        ON_BrepEdge* edge = trim->Edge();
        if (NULL == edge)
          continue;

        ON_Curve* crv = edge->DuplicateCurve();
        if (NULL == crv)
          continue;

        // true if the 2d trim and 3d edge have opposite orientations.
        if (trim->m_bRev3d)
          crv->Reverse();

        // true if face orientation is opposite of natural surface orientation
        if (face.m_bRev)
          crv->Reverse();

        out_curves.Append(crv);
      }
    }
  }

  return out_curves.Count() - out_curves_Count;
}

CRhinoCommand::result CCommandSampleExtractEdgeCurves::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surfaces and polysurfaces for edge extraction");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Brep* brep = go.Object(i).Brep();
    if (0 != brep)
    {
      ON_SimpleArray<ON_Curve*> curves;
      if (0 != RhCopyBrepFaceLoopCurves(brep, curves))
      {
        for (int j = 0; j < curves.Count(); j++)
        {
          CRhinoCurveObject* curve_obj = new CRhinoCurveObject();
          curve_obj->SetCurve(curves[j]);
          curves[i] = 0;
          context.m_doc.AddObject(curve_obj);
        }
      }
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleExtractEdgeCurves command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
