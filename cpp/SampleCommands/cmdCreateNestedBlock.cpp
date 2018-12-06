#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN CreateNestedBlock command
//

#pragma region CreateNestedBlock command

class CCommandCreateNestedBlock : public CRhinoCommand
{
public:
  CCommandCreateNestedBlock() = default;
  UUID CommandUUID() override
  {
    // {F93A8F1D-4C6C-4F81-B806-E10D1018F369}
    static const GUID CreateNestedBlockCommand_UUID =
    { 0xF93A8F1D, 0x4C6C, 0x4F81, { 0xB8, 0x06, 0xE1, 0x0D, 0x10, 0x18, 0xF3, 0x69 } };
    return CreateNestedBlockCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"CreateNestedBlock"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandCreateNestedBlock object
static class CCommandCreateNestedBlock theCreateNestedBlockCommand;

CRhinoCommand::result CCommandCreateNestedBlock::RunCommand(const CRhinoCommandContext& context)
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
// END CreateNestedBlock command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
