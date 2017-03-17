#pragma once

class CSampleRhinoEventWatcher : public CRhinoEventWatcher
{
  //////////////////////////////////////////////////////////////
  // Document events

  void OnCloseDocument(CRhinoDoc& doc);
  void OnNewDocument(CRhinoDoc& doc);
  void OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference);
  void OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference);
  void OnBeginSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected);
  void OnEndSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected);
  void OnDocumentPropertiesChanged(CRhinoDoc& doc);
  void OnAppSettingsChanged(const CRhinoAppSettings& new_app_settings);

  //////////////////////////////////////////////////////////////
  // View events

  void OnCreateView(CRhinoView* rhino_view);
  void OnDestroyView(CRhinoView* rhino_view);
  void OnSetActiveView(CRhinoView* rhino_view);
  void OnRenameView(CRhinoView* rhino_view);

  //////////////////////////////////////////////////////////////
  // Command events

  void OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context);
  void OnEndCommand(const CRhinoCommand& command, const CRhinoCommandContext& context, CRhinoCommand::result rc);

  //////////////////////////////////////////////////////////////
  // Object events

  void OnAddObject(CRhinoDoc& doc, CRhinoObject& object);
  void OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object);
  void OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object);
  void OnUnDeleteObject(CRhinoDoc& doc, CRhinoObject& object);
  void OnPurgeObject(CRhinoDoc& doc, CRhinoObject& object);
  void OnSelectObject(CRhinoDoc& doc, const CRhinoObject& object);
  void OnSelectObjects(CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects);
  void OnDeselectObject(CRhinoDoc& doc, const CRhinoObject& object);
  void OnDeselectObjects(CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects);
  void OnDeselectAllObjects(CRhinoDoc& doc, int count);
  void OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes);

  //////////////////////////////////////////////////////////////
  // Table events

  void LayerTableEvent(CRhinoEventWatcher::layer_event type, const CRhinoLayerTable& layer_table, int layer_index, const ON_Layer* old_settings);
  void LinetypeTableEvent(CRhinoEventWatcher::linetype_event type, const CRhinoLinetypeTable& linetype_table, int linetype_index, const ON_Linetype* old_settings);
  void InstanceDefinitionTableEvent(CRhinoEventWatcher::idef_event type, const CRhinoInstanceDefinitionTable& idef_table, int idef_index, const ON_InstanceDefinition* old_settings);
  void LightTableEvent(CRhinoEventWatcher::light_event type, const CRhinoLightTable& light_table, int light_index, const ON_Light* old_settings);
  void MaterialTableEvent(CRhinoEventWatcher::material_event type, const CRhinoMaterialTable& material_table, int material_index, const ON_Material* old_settings);
  void TextureMappingTableEvent(CRhinoEventWatcher::texture_mapping_event type, const CRhinoTextureMappingTable& texture_mapping_table, int texture_mapping_index, const ON_TextureMapping* old_settings);
  void GroupTableEvent(CRhinoEventWatcher::group_event type, const CRhinoGroupTable& group_table, int group_index, const ON_Group* old_settings);
  //void TextStyleTableEvent(CRhinoEventWatcher::text_style_event event, const CRhinoTextStyleTable& text_style_table, int text_style_index, const ON_TextStyle* old_settings);
  void DimStyleTableEvent(CRhinoEventWatcher::dimstyle_event type, const CRhinoDimStyleTable& dimstyle_table, int dimstyle_index, const ON_DimStyle* old_settings);
  void HatchPatternTableEvent(CRhinoEventWatcher::hatchpattern_event type, const CRhinoHatchPatternTable& hatchpattern_table, int hatchpattern_index, const ON_HatchPattern* old_settings);

  //////////////////////////////////////////////////////////////
  // Undo event

  void UndoEvent(CRhinoEventWatcher::undo_event type, unsigned int undo_record_serialnumber, const CRhinoCommand* cmd);
};