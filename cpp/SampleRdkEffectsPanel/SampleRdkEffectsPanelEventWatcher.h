// SampleRdkEffectsPanelEventWatcher.h
//

#pragma once

// CSampleRdkEffectsPanelEventWatcher
// See SampleRdkEffectsPanelEventWatcher.cpp for the implementation of this class.
//

class CSampleRdkEffectsPanelEventWatcher : public CRhinoEventWatcher
{
public:
  CSampleRdkEffectsPanelEventWatcher();

  void Defaults(BOOL b = FALSE);

  BOOL MaterialModified() const;
  BOOL MaterialAdded() const;
  BOOL MaterialDeleted() const;
  BOOL RenderMeshModified() const;
  BOOL RenderMeshAdded() const;
  BOOL RenderMeshDeleted() const;
  BOOL RenderMeshVisibilityChanged() const;
  BOOL LightModified() const;
  BOOL LightAdded() const;
  BOOL LightDeleted() const;

  void SetRenderMeshFlags(BOOL b = FALSE);
  void SetMaterialFlags(BOOL b = FALSE);
  void SetLightFlags(BOOL b = FALSE);

  virtual BOOL RenderSceneModified() const;
  virtual BOOL RenderLightingModified() const;

  // CRhinoEventWatcher overrides
  void OnEnableEventWatcher(BOOL b) override;
  void OnInitRhino(CRhinoApp& app) override;
  void OnCloseRhino(CRhinoApp& app) override;
  void OnCloseDocument(CRhinoDoc& doc) override;
  void OnNewDocument(CRhinoDoc& doc) override;
  void OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference) override;
  void OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference) override;
  void OnBeginSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected) override;
  void OnEndSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected) override;
  void OnDocumentPropertiesChanged(CRhinoDoc& doc) override;
  void OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context) override;
  void OnEndCommand(const CRhinoCommand& command, const CRhinoCommandContext& context, CRhinoCommand::result rc) override;
  void OnAddObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object) override;
  void OnUnDeleteObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnPurgeObject(CRhinoDoc& doc, CRhinoObject& object) override;
  void OnSelectObject(CRhinoDoc& doc, const CRhinoObject& object) override;
  void OnDeselectObject(CRhinoDoc& doc, const CRhinoObject& object) override;
  void OnDeselectAllObjects(CRhinoDoc& doc, int count) override;
  void OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes) override;
  void LayerTableEvent(CRhinoEventWatcher::layer_event event, const CRhinoLayerTable& layer_table, int layer_index, const ON_Layer* old_settings) override;
  void LightTableEvent(CRhinoEventWatcher::light_event event, const CRhinoLightTable& light_table, int light_index, const ON_Light* old_settings) override;
  void MaterialTableEvent(CRhinoEventWatcher::material_event event, const CRhinoMaterialTable& material_table, int material_index, const ON_Material* old_settings) override;
  void GroupTableEvent(CRhinoEventWatcher::group_event event, const CRhinoGroupTable& group_table, int group_index, const ON_Group* old_settings) override;
  void DimStyleTableEvent(CRhinoEventWatcher::dimstyle_event event, const CRhinoDimStyleTable& dimstyle_table, int dimstyle_index, const ON_DimStyle* old_settings) override;
  void OnUpdateObjectMesh(CRhinoDoc& doc, CRhinoObject& object, ON::mesh_type mesh_type) override;

private:
  BOOL m_material_modified;
  BOOL m_material_added;
  BOOL m_material_deleted;
  BOOL m_render_mesh_modified;
  BOOL m_render_mesh_added;
  BOOL m_render_mesh_deleted;
  BOOL m_render_mesh_visibility_changed;
  BOOL m_light_modified;
  BOOL m_light_added;
  BOOL m_light_deleted;
};

