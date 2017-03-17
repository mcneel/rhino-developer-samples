
#pragma once

class CMarmaladeEventWatcher : public CRhinoEventWatcher
{
public:
	CMarmaladeEventWatcher();
	virtual ~CMarmaladeEventWatcher();

	void SetDefaults(bool b);

	bool MaterialModified(void) const;
	bool MaterialAdded(void) const;
	bool MaterialDeleted(void) const;
	bool RenderMeshModified(void) const;
	bool RenderMeshAdded(void) const;
	bool RenderMeshDeleted(void) const;
	bool RenderMeshVisibilityChanged(void) const;
	bool LightModified(void) const;
	bool LightAdded(void) const;
	bool LightDeleted(void) const;

	void SetRenderMeshFlags(bool b);
	void SetMaterialFlags(bool b);
	void SetLightFlags(bool b);

	bool RenderSceneModified(void) const;
	bool RenderLightingModified(void) const;

public: // CRhinoEventWatcher overrides.
	virtual void OnEnableEventWatcher(BOOL b);
	virtual void OnInitRhino(CRhinoApp& app);
	virtual void OnCloseRhino(CRhinoApp& app);
	virtual void OnCloseDocument(CRhinoDoc& doc);
	virtual void OnNewDocument(CRhinoDoc& doc);
	virtual void OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference);
	virtual void OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference);
	virtual void OnBeginSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected);
	virtual void OnEndSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected);
	virtual void OnDocumentPropertiesChanged(CRhinoDoc& doc, const wchar_t* old_model_name, const ON_3dmProperties& old_properties, const ON_3dmSettings& old_settings);
	virtual void OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context);
	virtual void OnEndCommand(const CRhinoCommand& command, const CRhinoCommandContext& context, CRhinoCommand::result rc);
	virtual void OnAddObject(CRhinoDoc& doc, CRhinoObject& object);
	virtual void OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object);
	virtual void OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object);
	virtual void OnUnDeleteObject(CRhinoDoc& doc, CRhinoObject& object);
	virtual void OnPurgeObject(CRhinoDoc& doc, CRhinoObject& object);
	virtual void OnSelectObject(CRhinoDoc& doc, const CRhinoObject& object);
	virtual void OnDeselectObject(CRhinoDoc& doc, const CRhinoObject& object);
	virtual void OnDeselectAllObjects(CRhinoDoc& doc, int count);
	virtual void OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes);
	virtual void LayerTableEvent(CRhinoEventWatcher::layer_event event, const CRhinoLayerTable& layer_table, int layer_index, const ON_Layer* old_settings);
	virtual void LightTableEvent(CRhinoEventWatcher::light_event event, const CRhinoLightTable& light_table, int light_index, const ON_Light* old_settings);
	virtual void MaterialTableEvent(CRhinoEventWatcher::material_event event, const CRhinoMaterialTable& material_table, int material_index, const ON_Material* old_settings);
	virtual void GroupTableEvent(CRhinoEventWatcher::group_event event, const CRhinoGroupTable& group_table, int group_index, const ON_Group* old_settings);
	virtual void FontTableEvent(CRhinoEventWatcher::font_event event, const CRhinoFontTable& font_table, int font_index, const ON_Font* old_settings);
	virtual void DimStyleTableEvent(CRhinoEventWatcher::dimstyle_event event, const CRhinoDimStyleTable& dimstyle_table, int dimstyle_index, const ON_DimStyle* old_settings);
	virtual void OnUpdateObjectMesh(CRhinoDoc& doc, CRhinoObject& object, ON::mesh_type mesh_type);

private:
	UUID m_uuidOldRenderEngine;
	bool m_bMaterialModified;
	bool m_bMaterialAdded;
	bool m_bMaterialDeleted;
	bool m_bRenderMeshModified;
	bool m_bRenderMeshAdded;
	bool m_bRenderMeshDeleted;
	bool m_bRenderMeshVisibilityChanged;
	bool m_bLightModified;
	bool m_bLightAdded;
	bool m_bLightDeleted;
};
