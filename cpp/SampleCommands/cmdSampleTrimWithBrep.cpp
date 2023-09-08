#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTrimWithBrep command
//

#pragma region SampleTrimWithBrep command

class CCommandSampleTrimWithBrep : public CRhinoCommand
{
public:
  CCommandSampleTrimWithBrep() = default;
  UUID CommandUUID() override
  {
    // {649B81D3-3040-4036-A838-8309CAD6EA6F}
    static const GUID SampleTrimWithBrepCommand_UUID =
    { 0x649B81D3, 0x3040, 0x4036, { 0xA8, 0x38, 0x83, 0x09, 0xCA, 0xD6, 0xEA, 0x6F } };
    return SampleTrimWithBrepCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleTrimWithBrep"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleTrimWithBrep object
static class CCommandSampleTrimWithBrep theSampleTrimWithBrepCommand;

CRhinoCommand::result CCommandSampleTrimWithBrep::RunCommand(const CRhinoCommandContext& context)
{
  // Works similar to Grasshopper's "Trim with Brep (Trim)" component

  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  // Select curves to split
  CRhinoGetObject gc;
  gc.SetCommandPrompt(L"Select curves to split");
  gc.SetGeometryFilter(CRhinoGetObject::curve_object);
  gc.EnableSubObjectSelect(FALSE);
  gc.GetObjects(1, 0);
  if (gc.CommandResult() != CRhinoCommand::success)
    return gc.CommandResult();

  // Select cutting Brep
  CRhinoGetObject gb;
  gb.SetCommandPrompt(L"Select cutting Brep");
  gb.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  gb.SetGeometryAttributeFilter(CRhinoGetObject::closed_surface | CRhinoGetObject::closed_polysrf);
  gb.EnableSubObjectSelect(FALSE);
  gb.EnablePreSelect(FALSE);
  gb.EnableDeselectAllBeforePostSelect(false);
  gb.GetObjects(1, 1);
  if (gb.CommandResult() != CRhinoCommand::success)
    return gb.CommandResult();

  const ON_Brep* brep = gb.Object(0).Brep();
  if (nullptr == brep)
    return CRhinoCommand::failure;

  // Make some output layers
  int inside_index = doc->m_layer_table.AddLayer(L"Inside", ON_Color(255, 0, 0), true);
  int outside_index = doc->m_layer_table.AddLayer(L"Outside", ON_Color(0, 0, 255), true);

  double tol = doc->AbsoluteTolerance();
  double angtol = doc->AngleToleranceRadians();

  // Process each curve
  for (int i = 0; i < gc.ObjectCount(); i++)
  {
    const CRhinoObjRef& object_ref = gc.Object(i);
    const ON_Curve* curve = object_ref.Curve();
    if (nullptr == curve)
      continue;

    bool ok = false;

    ON_SimpleArray<ON_Curve*> pieces;
    if (RhinoCurveSplit(*curve, *brep, tol, angtol, pieces))
    {
      for (int p = 0; p < pieces.Count(); p++)
      {
        ON_Curve* piece = pieces[p];
        if (nullptr == piece)
          continue;

        // Get midpoint
        double t = ON_UNSET_VALUE;
        if (!piece->GetNormalizedArcLengthPoint(0.5, &t))
          continue;

        // Detemine if midpoint is inside cutter
        ON_3dPoint point = piece->PointAt(t);
        bool inside = RhinoIsPointInBrep(brep, point, tol, false);

        CRhinoObjectAttributes attributes;
        doc->GetDefaultObjectAttributes(attributes);
        attributes.m_layer_index = (inside) ? inside_index : outside_index;

        CRhinoCurveObject* curve_object = new CRhinoCurveObject(attributes);
        curve_object->SetCurve(piece);
        doc->AddObject(curve_object);
        ok = true;
      }
    }

    if (ok)
      doc->DeleteObject(object_ref);
  }

  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleTrimWithBrep command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
