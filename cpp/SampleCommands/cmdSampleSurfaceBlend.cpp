#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSurfaceBlend command
//

#pragma region SampleSurfaceBlend command

class CCommandSampleSurfaceBlend : public CRhinoCommand
{
public:
  CCommandSampleSurfaceBlend() = default;
  UUID CommandUUID() override
  {
    // {1FB45CE3-18BF-4B11-9F38-6E1F122A6DAC}
    static const GUID SampleSurfaceBlendCommand_UUID =
    { 0x1FB45CE3, 0x18BF, 0x4B11,{ 0x9F, 0x38, 0x6E, 0x1F, 0x12, 0x2A, 0x6D, 0xAC } };
    return SampleSurfaceBlendCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSurfaceBlend"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSurfaceBlend object
static class CCommandSampleSurfaceBlend theSampleSurfaceBlendCommand;

CRhinoCommand::result CCommandSampleSurfaceBlend::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface edges to blend");
  go.SetGeometryFilter(CRhinoGetObject::edge_object);
  go.GetObjects(2, 2);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_BrepFace* faces[2] = { 0 };
  const ON_BrepEdge* edges[2] = { 0 };
  ON_Interval domains[2];
  bool revs[2] = { 0 };

  for (int i = 0; i < 2; i++)
  {
    const CRhinoObjRef& obj_ref = go.Object(i);

    const ON_BrepTrim* trim = obj_ref.Trim();
    if (nullptr == trim)
      return CRhinoCommand::failure;

    const ON_BrepFace* face = trim->Face();
    const ON_BrepEdge* edge = trim->Edge();
    if (nullptr == face || nullptr == edge)
      return CRhinoCommand::failure;

    ON_Interval domain = edge->Domain();
    bool rev = false;

    if (1 == obj_ref.SelectionMethod())
    {
      ON_3dPoint pt;
      if (obj_ref.SelectionPoint(pt))
      {
        double t = ON_UNSET_VALUE;
        if (edge->GetClosestPoint(pt, &t))
          rev = (t > domain.Mid()) ? true : false;
      }
    }

    faces[i] = face;
    edges[i] = edge;
    domains[i] = domain;
    revs[i] = rev;
  }

  int continuity[] = { 2, 2 };

  ON_SimpleArray<ON_Brep*> output;
  bool rc = RhinoSdkBlend::SurfaceBlend(*doc,
    faces[0], edges[0], domains[0], revs[0], continuity[0],
    faces[1], edges[1], domains[1], revs[1], continuity[1],
    output
  );

  if (rc)
  {
    for (int i = 0; i < output.Count(); i++)
    {
      if (nullptr != output[0])
      {
        CRhinoBrepObject* brep_obj = new CRhinoBrepObject();
        brep_obj->SetBrep(output[0]);
        output[0] = nullptr;
        doc->AddObject(brep_obj);
      }
    }
  }

  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSurfaceBlend command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
