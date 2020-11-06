#include "stdafx.h"

/// <summary>
/// If sub-object selection is enabled and a piece of an instance reference
/// was selected, this returns the selected instance definition object.
/// </summary>
/// <param name="obj_ref">The object reference to query.</param>
/// <returns>
/// A pointer to the instance definition geometry if success, nullptr otherwise
/// </returns>
const CRhinoObject* RhGetInstanceDefinitionObject(const CRhinoObjRef& obj_ref)
{
  const int chain_length = obj_ref.m__iref.Count();
  if (chain_length > 0)
  {
    const ON_ObjRef_IRefID& iref_id = *obj_ref.m__iref.Last();
    const CRhinoObject* top_obj = obj_ref.Object();
    CRhinoDoc* doc = top_obj ? top_obj->Document() : nullptr;
    if (nullptr != doc)
    {
      int idef_index = doc->m_instance_definition_table.FindInstanceDefinition(iref_id.m_idef_uuid);
      if (idef_index >= 0)
      {
        const CRhinoInstanceDefinition* idef = doc->m_instance_definition_table[idef_index];
        if (nullptr != idef)
          return idef->Object(iref_id.m_idef_geometry_index);
      }
    }
  }
  return nullptr;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePickEdges command
//

#pragma region SamplePickEdges command

class CCommandSamplePickEdges : public CRhinoCommand
{
public:
	CCommandSamplePickEdges() = default;
	UUID CommandUUID() override
	{
		// {41BACCFB-BBAF-4404-BA91-F15F1027BB1D}
		static const GUID SamplePickEdgesCommand_UUID =
		{ 0x41BACCFB, 0xBBAF, 0x4404, { 0xBA, 0x91, 0xF1, 0x5F, 0x10, 0x27, 0xBB, 0x1D } };
		return SamplePickEdgesCommand_UUID;
	}
	const wchar_t* EnglishCommandName() override { return L"SamplePickEdges"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSamplePickEdges object
static class CCommandSamplePickEdges theSamplePickEdgesCommand;

CRhinoCommand::result CCommandSamplePickEdges::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select edges");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::edge_curve | CRhinoGetObject::mated_edge | CRhinoGetObject::boundary_edge);
  go.GetObjects(1, 0);
  if (CRhinoCommand::success != go.CommandResult())
    return go.CommandResult();

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& obj_ref = go.Object(i);

    const ON_BrepEdge* edge = obj_ref.Edge();
    if (nullptr == edge)
      return CRhinoCommand::failure;

    const CRhinoObject* rh_obj = obj_ref.Object();
    if (nullptr == rh_obj)
      return CRhinoCommand::failure;

    const CRhinoInstanceObject* iref_obj = CRhinoInstanceObject::Cast(obj_ref.Object());
    if (nullptr != iref_obj)
      rh_obj = RhGetInstanceDefinitionObject(obj_ref);

    if (nullptr != rh_obj)
    {
      const wchar_t* rh_obj_type = (iref_obj) ? L"Instance definition object" : L"Object";
      ON_wString uuid_str;
      ON_UuidToString(rh_obj->ModelObjectId(), uuid_str);
      RhinoApp().Print(L"%s: %s, edge index: %d\n", rh_obj_type, uuid_str.Array(), edge->m_edge_index);
    }
  }
  
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePickEdges command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
