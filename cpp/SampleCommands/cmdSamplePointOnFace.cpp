#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePointOnFace command
//

#pragma region SamplePointOnFace command

static bool RhFindPointOnFace(const ON_BrepFace* face, ON_3dPoint& point)
{
  if (0 == face)
    return false;

  // Get the face's outer parameter space trimming loo
  const ON_BrepLoop* loop = face->OuterLoop();
  if (0 == loop)
    return false;

  // Get the center point of parameter space trimming loop's bounding box
  ON_2dPoint center = loop->m_pbox.Center();

  // Get the iso curves, in each direction, at the center point 
  ON_SimpleArray<ON_Curve*> iso_curves;
  face->GetIsoCurves(0, center.y, iso_curves);
  face->GetIsoCurves(1, center.x, iso_curves);

  // Find the iso curve with the largest domain
  int iso_index = -1;
  double max_delta = 0.0;
  for (int i = 0; i < iso_curves.Count(); i++)
  {
    ON_Curve* crv = iso_curves[i];
    if (crv)
    {
      double delta = crv->Domain().Length();
      if (delta > max_delta)
      {
        iso_index = i;
        max_delta = delta;
      }
    }
  }

  // Get the center point of iso curve
  bool rc = false;
  if (iso_index >= 0 && iso_index < iso_curves.Count())
  {
    double t = 0.0;
    ON_Curve* crv = iso_curves[iso_index];
    if (crv && crv->GetNormalizedArcLengthPoint(0.5, &t))
    {
      point = crv->PointAt(t);
      rc = true;
    }
  }

  // ON_BrepFace::GetIsoCurves allocates memory for each iso curve.
  // We are responsible for recovering this memory so we don't leak.
  for (int i = 0; i < iso_curves.Count(); i++)
  {
    ON_Curve* crv = iso_curves[i];
    if (crv)
    {
      delete crv;
      iso_curves[i] = 0;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSamplePointOnFace : public CRhinoCommand
{
public:
  CCommandSamplePointOnFace() {}
  ~CCommandSamplePointOnFace() {}
  UUID CommandUUID()
  {
    // {E09AA069-DD6-4FF5-A708-8ACA7EA1482F}
    static const GUID SamplePointOnFaceCommand_UUID =
    { 0xE09AA069, 0xDD6, 0x4FF5, { 0xA7, 0x08, 0x8A, 0xCA, 0x7E, 0xA1, 0x48, 0x2F } };
    return SamplePointOnFaceCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePointOnFace"; }
  const wchar_t* LocalCommandName() const { return L"SamplePointOnFace"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePointOnFace object
static class CCommandSamplePointOnFace theSamplePointOnFaceCommand;

CRhinoCommand::result CCommandSamplePointOnFace::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(TRUE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_BrepFace* face = go.Object(0).Face();
  if (0 == face)
    return CRhinoCommand::failure;

  ON_3dPoint point = ON_UNSET_POINT;
  bool rc = RhFindPointOnFace(face, point);
  if (rc && point.IsValid())
  {
    ON_wString point_str;
    RhinoFormatPoint(point, point_str);
    RhinoApp().Print(L"Point on face = %s\n", point_str);

    context.m_doc.AddPointObject(point);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePointOnFace command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
