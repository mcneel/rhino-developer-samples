#pragma once

class CSampleRhinoEventWatcher : public CRhinoEventWatcher
{
  //////////////////////////////////////////////////////////////
  // Document events

  void OnCloseDocument(CRhinoDoc& doc) override;
  void OnNewDocument(CRhinoDoc& doc) override;
  void OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference) override;
  void OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference) override;
  void OnBeginSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected) override;
  void OnEndSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected) override;
  void OnDocumentPropertiesChanged(CRhinoDoc& doc) override;
  void OnAppSettingsChanged(const CRhinoAppSettings& new_app_settings) override;

  //////////////////////////////////////////////////////////////
  // View events

  void OnCreateView(CRhinoView* rhino_view) override;
  void OnDestroyView(CRhinoView* rhino_view) override;
  void OnSetActiveView(CRhinoView* rhino_view) override;
  void OnRenameView(CRhinoView* rhino_view) override;

  //////////////////////////////////////////////////////////////
  // Command events

  void OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context) override;
  void OnEndCommand(const CRhinoCommand& command, const CRhinoCommandContext& context, CRhinoCommand::result rc) override;

  //////////////////////////////////////////////////////////////
  // Object events

  void OnAddObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object) override;
  void OnUnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnPurgeObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnSelectObject(CRhinoDoc& doc, const CRhinoObject& object) override;
  void OnSelectObjects(CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects) override;
  void OnDeselectObject(CRhinoDoc& doc, const CRhinoObject& object) override;
  void OnDeselectObjects(CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects) override;
  void OnDeselectAllObjects(CRhinoDoc& doc, int count) override;
  void OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes) override;

  //////////////////////////////////////////////////////////////
  // Table events

  void LayerTableEvent(CRhinoEventWatcher::layer_event type, const CRhinoLayerTable& layer_table, int layer_index, const ON_Layer* old_settings) override;
  void LinetypeTableEvent(CRhinoEventWatcher::linetype_event type, const CRhinoLinetypeTable& linetype_table, int linetype_index, const ON_Linetype* old_settings) override;
  void InstanceDefinitionTableEvent(CRhinoEventWatcher::idef_event type, const CRhinoInstanceDefinitionTable& idef_table, int idef_index, const ON_InstanceDefinition* old_settings) override;
  void LightTableEvent(CRhinoEventWatcher::light_event type, const CRhinoLightTable& light_table, int light_index, const ON_Light* old_settings) override;
  void MaterialTableEvent(CRhinoEventWatcher::material_event type, const CRhinoMaterialTable& material_table, int material_index, const ON_Material* old_settings) override;
  void TextureMappingTableEvent(CRhinoEventWatcher::texture_mapping_event type, const CRhinoTextureMappingTable& texture_mapping_table, int texture_mapping_index, const ON_TextureMapping* old_settings) override;
  void GroupTableEvent(CRhinoEventWatcher::group_event type, const CRhinoGroupTable& group_table, int group_index, const ON_Group* old_settings) override;
  //void TextStyleTableEvent(CRhinoEventWatcher::text_style_event event, const CRhinoTextStyleTable& text_style_table, int text_style_index, const ON_TextStyle* old_settings) override;
  void DimStyleTableEvent(CRhinoEventWatcher::dimstyle_event type, const CRhinoDimStyleTable& dimstyle_table, int dimstyle_index, const ON_DimStyle* old_settings) override;
  void HatchPatternTableEvent(CRhinoEventWatcher::hatchpattern_event type, const CRhinoHatchPatternTable& hatchpattern_table, int hatchpattern_index, const ON_HatchPattern* old_settings) override;

  //////////////////////////////////////////////////////////////
  // Undo event

  void UndoEvent(CRhinoEventWatcher::undo_event type, unsigned int undo_record_serialnumber, const CRhinoCommand* cmd) override;
};