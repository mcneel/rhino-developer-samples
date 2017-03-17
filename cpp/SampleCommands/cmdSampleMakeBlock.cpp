#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMakeBlock command
//

class CCommandSampleMakeBlock : public CRhinoCommand
{
public:
  CCommandSampleMakeBlock() {}
  ~CCommandSampleMakeBlock() {}
  UUID CommandUUID()
  {
    // {75BDE8D5-A2EE-4EB8-8439-005E0DA9E982}
    static const GUID SampleMakeBlockCommand_UUID =
    { 0x75BDE8D5, 0xA2EE, 0x4EB8, { 0x84, 0x39, 0x00, 0x5E, 0x0D, 0xA9, 0xE9, 0x82 } };
    return SampleMakeBlockCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMakeBlock"; }
  const wchar_t* LocalCommandName() { return L"SampleMakeBlock"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMakeBlock object
static class CCommandSampleMakeBlock theSampleMakeBlockCommand;

CRhinoCommand::result CCommandSampleMakeBlock::RunCommand(const CRhinoCommandContext& context)
{
  // Select objects to define block
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to define block");
  go.EnableReferenceObjectSelect(false);
  go.EnableSubObjectSelect(false);
  go.EnableGroupSelect(true);

  // Phantoms, grips, lights, etc., cannot be in blocks.
  const unsigned int forbidden_geometry_filter
    = CRhinoGetObject::light_object
    | CRhinoGetObject::grip_object
    | CRhinoGetObject::phantom_object;
  const unsigned int geometry_filter = forbidden_geometry_filter
    ^ CRhinoGetObject::any_object;
  go.SetGeometryFilter(geometry_filter);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Block base point
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Block base point");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint base_point = gp.Point();

  // Block definition name
  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Block definition name");
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  // Validate block name
  ON_wString idef_name = gs.String();
  idef_name.TrimLeftAndRight();
  if (idef_name.IsEmpty())
    return CRhinoCommand::nothing;

  // See if block name already exists
  CRhinoInstanceDefinitionTable& idef_table = context.m_doc.m_instance_definition_table;
  int idef_index = idef_table.FindInstanceDefinition(idef_name);
  if (idef_index >= 0)
  {
    RhinoApp().Print(L"Block definition \"%s\" already exists.\n", idef_name);
    return CRhinoCommand::nothing;
  }

  // Create new block definition
  ON_InstanceDefinition idef;
  idef.SetName(idef_name);

  // Gather all of the selected objects
  ON_SimpleArray<const CRhinoObject*> objects(go.ObjectCount());
  int i;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObject* obj = go.Object(i).Object();
    if (obj)
      objects.Append(obj);
  }

  ON_Xform xform;
  xform.Translation(ON_origin - base_point);

  // Duplicate all of the selected objects and add them
  // to the document as instance definition objects
  ON_SimpleArray<const CRhinoObject*> idef_objects(objects.Count());
  for (i = 0; i < objects.Count(); i++)
  {
    const CRhinoObject* obj = objects[i];
    if (obj)
    {
      CRhinoObject* dupe = obj->Duplicate();
      if (dupe)
      {
        // Transform the geometry to the world origin.
        ON_Geometry* geom = const_cast<ON_Geometry*>(dupe->Geometry());
        if (geom)
        {
          geom->Transform(xform);
          dupe->UpdateBoundingBox();
        }
        context.m_doc.AddObject(dupe, false, true);
        idef_objects.Append(dupe);
      }
    }
  }

  if (idef_objects.Count() < 1)
  {
    RhinoApp().Print(L"Unable to duplicate block definition geometry.\n");
    return CRhinoCommand::failure;
  }

  idef_index = idef_table.AddInstanceDefinition(idef, idef_objects);
  if (idef_index < 0)
  {
    RhinoApp().Print(L"Unable to create block definition \"%s\".\n", idef_name);
    return CRhinoCommand::failure;
  }

  return CRhinoCommand::success;
}

//
// END SampleMakeBlock command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
