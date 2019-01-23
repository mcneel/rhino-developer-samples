#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCreateNestedBlock command
//

#pragma region SampleCreateNestedBlock command

class CCommandSampleCreateNestedBlock : public CRhinoCommand
{
public:
  CCommandSampleCreateNestedBlock() = default;
  UUID CommandUUID() override
  {
    // {62EDBB73-ADA0-4F07-977A-DE5BE926F21A}
    static const GUID SampleCreateNestedBlockCommand_UUID =
    { 0x62EDBB73, 0xADA0, 0x4F07, { 0x97, 0x7A, 0xDE, 0x5B, 0xE9, 0x26, 0xF2, 0x1A } };
    return SampleCreateNestedBlockCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCreateNestedBlock"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleCreateNestedBlock object
static class CCommandSampleCreateNestedBlock theSampleCreateNestedBlockCommand;

CRhinoCommand::result CCommandSampleCreateNestedBlock::RunCommand(const CRhinoCommandContext& context)
{
  // Add a circle to the document as instance definition geometry
  ON_Circle circle(ON_Plane::World_xy, 5.0);
  ON_ArcCurve* crv0 = new ON_ArcCurve(circle);
  CRhinoCurveObject* crv0_obj = new CRhinoCurveObject();
  crv0_obj->SetCurve(crv0);
  context.m_doc.AddObject(crv0_obj, false, true);

  // Create a new instance definition
  ON_InstanceDefinition idef0;
  idef0.SetName(L"Circle");

  // Add the new instance definition to the document
  ON_SimpleArray<const CRhinoObject*> idef0_objs;
  idef0_objs.Append(crv0_obj);
  int idef0_idx = context.m_doc.m_instance_definition_table.AddInstanceDefinition(idef0, idef0_objs);

  // Add a rectangle to the document as instance definition geometry
  ON_Polyline rect;
  rect.Append(ON_3dPoint(-5.0, -5.0, 0.0));
  rect.Append(ON_3dPoint(5.0, -5.0, 0.0));
  rect.Append(ON_3dPoint(5.0, 5.0, 0.0));
  rect.Append(ON_3dPoint(-5.0, 5.0, 0.0));
  rect.Append(rect[0]);
  ON_PolylineCurve* crv1 = new ON_PolylineCurve(rect);
  CRhinoCurveObject* crv1_obj = new CRhinoCurveObject();
  crv1_obj->SetCurve(crv1);
  context.m_doc.AddObject(crv1_obj, false, true);

  // Add a instance of "Circle" to the document as instance definition geometry
  CRhinoInstanceObject* iref0_obj = new CRhinoInstanceObject();
  iref0_obj->SetInstanceDefinition(context.m_doc.m_instance_definition_table[idef0_idx]);
  iref0_obj->SetInstanceTransform(ON_Xform(1));
  context.m_doc.AddObject(iref0_obj, false, true);

  // Create another new instance definition
  ON_InstanceDefinition idef1;
  idef1.SetName(L"Rectangle and Circle");

  // Add the new instance definition to the document
  ON_SimpleArray<const CRhinoObject*> idef1_objs;
  idef1_objs.Append(crv1_obj);
  idef1_objs.Append(iref0_obj);
  int idef1_idx = context.m_doc.m_instance_definition_table.AddInstanceDefinition(idef1, idef1_objs);

  // Add an instance of "Rectangle and Circle" to the document
  context.m_doc.m_instance_definition_table.AddInstanceObject(idef1_idx, ON_Xform(1));
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCreateNestedBlock command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
