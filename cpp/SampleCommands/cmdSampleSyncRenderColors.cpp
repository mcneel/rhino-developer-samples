#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSyncRenderColors command
//

class CCommandSampleSyncRenderColors : public CRhinoCommand
{
public:
  CCommandSampleSyncRenderColors() {}
  ~CCommandSampleSyncRenderColors() {}
  UUID CommandUUID()
  {
    // {6CB51310-C634-4DE0-85E0-02C44A8E54B2}
    static const GUID SampleSyncRenderColorsCommand_UUID =
    { 0x6CB51310, 0xC634, 0x4DE0, { 0x85, 0xE0, 0x02, 0xC4, 0x4A, 0x8E, 0x54, 0xB2 } };
    return SampleSyncRenderColorsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSyncRenderColors"; }
  const wchar_t* LocalCommandName() { return L"SampleSyncRenderColors"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  bool SychronizeDiffuseColorWithDisplayColor(CRhinoDoc& doc, const CRhinoObject* obj);
};

// The one and only CCommandSampleSyncRenderColors object
static class CCommandSampleSyncRenderColors theSampleSyncRenderColorsCommand;

CRhinoCommand::result CCommandSampleSyncRenderColors::RunCommand(const CRhinoCommandContext& context)
{
  int num_modified = 0;

  if (context.IsInteractive())
  {
    CRhinoObjectIterator it(CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_objects);
    it.IncludeLights(FALSE);

    CRhinoObject* obj = 0;
    for (obj = it.First(); obj; obj = it.Next())
    {
      if (obj && SychronizeDiffuseColorWithDisplayColor(context.m_doc, obj))
        num_modified++;
    }
  }
  else
  {
    CRhinoGetObject go;
    go.SetCommandPrompt(L"Select objects");
    go.EnablePreSelect(TRUE);
    go.EnableGroupSelect(TRUE);
    go.EnableSubObjectSelect(FALSE);
    go.GetObjects(1, 0);
    if (go.CommandResult() != CRhinoCommand::success)
      return go.CommandResult();

    int i, count = go.ObjectCount();
    for (i = 0; i < count; i++)
    {
      const CRhinoObject* obj = go.Object(i).Object();
      if (obj && SychronizeDiffuseColorWithDisplayColor(context.m_doc, obj))
        num_modified++;
    }
  }

  if (num_modified > 0)
    context.m_doc.Regen();

  return CRhinoCommand::success;
}

bool CCommandSampleSyncRenderColors::SychronizeDiffuseColorWithDisplayColor(CRhinoDoc& doc, const CRhinoObject* obj)
{
  if (0 == obj)
    return false;

  int material_index = -1;

  ON_Color color = obj->Attributes().DrawColor();
  ON::object_color_source color_source = obj->Attributes().ColorSource();

  if (color_source == ON::color_from_layer)
  {
    const CRhinoLayer& layer = obj->ObjectLayer();
    material_index = layer.RenderMaterialIndex();
    if (material_index < 0)
    {
      material_index = doc.m_material_table.AddCopyOfDefaultMaterial();
      if (material_index >= 0)
      {
        ON_Layer new_layer(layer);
        new_layer.SetRenderMaterialIndex(material_index);
        if (!doc.m_layer_table.ModifyLayer(new_layer, layer.Index()))
          return false;
      }
    }
  }

  else if (color_source == ON::color_from_object)
  {
    material_index = obj->Attributes().m_material_index;

    if (material_index < 0)
    {
      material_index = doc.m_material_table.AddCopyOfDefaultMaterial();
      if (material_index >= 0)
      {
        CRhinoObjectAttributes new_attributes(obj->Attributes());
        new_attributes.m_material_index = material_index;
        new_attributes.SetMaterialSource(ON::material_from_object);
        if (!doc.ModifyObjectAttributes(CRhinoObjRef(obj), new_attributes))
          return false;
      }
    }
  }

  if (material_index < 0)
    return false;

  const CRhinoMaterial& material = doc.m_material_table[material_index];
  if (color == material.Diffuse())
    return false;

  ON_Material new_material(material);
  new_material.SetDiffuse(color);
  if (!doc.m_material_table.ModifyMaterial(new_material, material.Index(), FALSE))
    return false;

  return true;
}

//
// END SampleSyncRenderColors command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
