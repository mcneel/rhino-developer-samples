
#include "StdAfx.h"
#include "MarmaladeEventWatcher.h"
#include "MarmaladePlugIn.h"

CMarmaladeEventWatcher::CMarmaladeEventWatcher()
{
	SetDefaults(false);
}

CMarmaladeEventWatcher::~CMarmaladeEventWatcher()
{
}

void CMarmaladeEventWatcher::SetDefaults(bool b)
{
	SetRenderMeshFlags(b);
	SetMaterialFlags(b);
	SetLightFlags(b);
}

bool CMarmaladeEventWatcher::RenderSceneModified(void) const
{
	return MaterialModified()   || MaterialAdded()   || MaterialDeleted()   ||
	       RenderMeshModified() || RenderMeshAdded() || RenderMeshDeleted() || RenderMeshVisibilityChanged();
}

bool CMarmaladeEventWatcher::RenderLightingModified(void) const
{
	return LightModified() || LightAdded() || LightDeleted();
}

bool CMarmaladeEventWatcher::MaterialModified(void) const
{
	return m_bMaterialModified;
}

bool CMarmaladeEventWatcher::MaterialAdded(void) const
{
	return m_bMaterialAdded;
}

bool CMarmaladeEventWatcher::MaterialDeleted(void) const
{
	return m_bMaterialDeleted;
}

bool CMarmaladeEventWatcher::RenderMeshModified(void) const
{
	return m_bRenderMeshModified;
}

bool CMarmaladeEventWatcher::RenderMeshAdded(void) const
{
	return m_bRenderMeshAdded;
}

bool CMarmaladeEventWatcher::RenderMeshDeleted(void) const
{
	return m_bRenderMeshDeleted;
}

bool CMarmaladeEventWatcher::RenderMeshVisibilityChanged(void) const
{
	return m_bRenderMeshVisibilityChanged;
}

bool CMarmaladeEventWatcher::LightModified(void) const
{
	return m_bLightModified;
}

bool CMarmaladeEventWatcher::LightAdded(void) const
{
	return m_bLightAdded;
}

bool CMarmaladeEventWatcher::LightDeleted(void) const
{
	return m_bLightDeleted;
}

void CMarmaladeEventWatcher::SetRenderMeshFlags(bool b)
{
	m_bRenderMeshModified = b;
	m_bRenderMeshAdded = b;
	m_bRenderMeshDeleted = b;
	m_bRenderMeshVisibilityChanged = b;
}

void CMarmaladeEventWatcher::SetMaterialFlags(bool b)
{
	m_bMaterialModified = b;
	m_bMaterialAdded = b;
	m_bMaterialDeleted = b;
}

void CMarmaladeEventWatcher::SetLightFlags(bool b)
{
	m_bLightModified = b;
	m_bLightAdded = b;
	m_bLightDeleted = b;
}

void CMarmaladeEventWatcher::OnEnableEventWatcher(BOOL b)
{
	SetDefaults(false);
}

void CMarmaladeEventWatcher::OnInitRhino(CRhinoApp& app)
{
}

void CMarmaladeEventWatcher::OnCloseRhino(CRhinoApp& app)
{
}

void CMarmaladeEventWatcher::OnCloseDocument(CRhinoDoc& doc)
{
}

void CMarmaladeEventWatcher::OnNewDocument(CRhinoDoc& doc)
{
	SetDefaults(true);
}

void CMarmaladeEventWatcher::OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference)
{
	SetDefaults(true);
}

void CMarmaladeEventWatcher::OnEndOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference)
{
	SetDefaults(true);
}

void CMarmaladeEventWatcher::OnBeginSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected)
{
}

void CMarmaladeEventWatcher::OnEndSaveDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected)
{
}

static bool ON_3dmRenderSettingsEqual(const ON_3dmRenderSettings& s1, const ON_3dmRenderSettings& s2)
{
	return (s1.m_bCustomImageSize  == s2.m_bCustomImageSize ) &&
	       (s1.m_image_width       == s2.m_image_width      ) &&
	       (s1.m_image_height      == s2.m_image_height     ) &&
	       (s1.m_image_dpi         == s2.m_image_dpi        ) &&
	       (s1.m_image_us          == s2.m_image_us         ) &&
	       (s1.m_ambient_light     == s2.m_ambient_light    ) &&
	       (s1.m_background_style  == s2.m_background_style ) &&
	       (s1.m_background_color  == s2.m_background_color ) &&
	       (s1.m_bUseHiddenLights  == s2.m_bUseHiddenLights ) &&
	       (s1.m_bDepthCue         == s2.m_bDepthCue        ) &&
	       (s1.m_bFlatShade        == s2.m_bFlatShade       ) &&
	       (s1.m_bRenderBackfaces  == s2.m_bRenderBackfaces ) &&
	       (s1.m_bRenderPoints     == s2.m_bRenderPoints    ) &&
	       (s1.m_bRenderCurves     == s2.m_bRenderCurves    ) &&
	       (s1.m_bRenderIsoparams  == s2.m_bRenderIsoparams ) &&
	       (s1.m_bRenderMeshEdges  == s2.m_bRenderMeshEdges ) &&
	       (s1.m_bRenderAnnotation == s2.m_bRenderAnnotation) &&
	       (s1.m_antialias_style   == s2.m_antialias_style  ) &&
	       (s1.m_shadowmap_style   == s2.m_shadowmap_style  ) &&
	       (s1.m_shadowmap_width   == s2.m_shadowmap_width  ) &&
	       (s1.m_shadowmap_height  == s2.m_shadowmap_height ) &&
	       (s1.m_shadowmap_offset  == s2.m_shadowmap_offset ) &&
	       (s1.m_background_bitmap_filename == s2.m_background_bitmap_filename);
}

void CMarmaladeEventWatcher::OnDocumentPropertiesChanged(CRhinoDoc& doc, const wchar_t* old_model_name, const ON_3dmProperties& old_properties, const ON_3dmSettings& old_settings)
{
}

void CMarmaladeEventWatcher::OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context)
{
	const ON_wString s = L"SetCurrentRenderPlugin";

	if (s.CompareNoCase(const_cast<CRhinoCommand&>(command).EnglishCommandName()) == 0)
	{
		m_uuidOldRenderEngine = RhinoApp().GetDefaultRenderApp();
	}
}

void CMarmaladeEventWatcher::OnEndCommand(const CRhinoCommand& command, const CRhinoCommandContext& context, CRhinoCommand::result rc)
{
	const ON_wString s = L"SetCurrentRenderPlugin";

	if ((rc == CRhinoCommand::success) && (s.CompareNoCase(const_cast<CRhinoCommand&>(command).EnglishCommandName()) == 0))
	{
		const UUID uuidRenderEngine = RhinoApp().GetDefaultRenderApp();
		if (m_uuidOldRenderEngine != uuidRenderEngine)
		{
			if (uuidRenderEngine == MarmaladePlugIn().PlugInID())
			{
				MarmaladePlugIn().AddMarmaladeMenu();
			}
			else
			{
				MarmaladePlugIn().RemoveMarmaladeMenu();
			}
		}
	}
}

void CMarmaladeEventWatcher::OnAddObject(CRhinoDoc& doc, CRhinoObject& object)
{
	// TODO: CHECK FOR LIGHTS
	if (object.IsMeshable(ON::render_mesh))
		m_bRenderMeshAdded = true;
}

void CMarmaladeEventWatcher::OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object)
{
	// TODO: CHECK FOR LIGHTS
	if (object.IsMeshable(ON::render_mesh))
		m_bRenderMeshDeleted = true;
}

void CMarmaladeEventWatcher::OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object)
{
	// TODO: CHECK FOR LIGHTS
	ON_SimpleArray<const ON_Mesh*> old_meshes, new_meshes;

	bool bOldMeshes = (old_object.GetMeshes(ON::render_mesh, old_meshes) < 1);
	bool bNewMeshes = (new_object.GetMeshes(ON::render_mesh, new_meshes) < 1);

	if (bOldMeshes)
	{
		if (new_object.IsMeshable(ON::render_mesh))
		{
			if (bNewMeshes)
			{
				m_bRenderMeshModified = true;
			}
			else
			{
				m_bRenderMeshDeleted = true;
			}
		} 
		else
		{
			m_bRenderMeshDeleted = true;
		}
	} 
	else
	if (bNewMeshes)
	{
		m_bRenderMeshAdded = true;
	} 
	else
	if (new_object.IsMeshable(ON::render_mesh))
	{
		m_bRenderMeshAdded = true;
	}
}

void CMarmaladeEventWatcher::OnUnDeleteObject(CRhinoDoc& doc, CRhinoObject& object)
{
	// TODO: CHECK FOR LIGHTS
	if (object.IsMeshable(ON::render_mesh))
		m_bRenderMeshAdded = true;
}

void CMarmaladeEventWatcher::OnPurgeObject(CRhinoDoc& doc, CRhinoObject& object)
{
}

void CMarmaladeEventWatcher::OnSelectObject(CRhinoDoc& doc, const CRhinoObject& object)
{
}

void CMarmaladeEventWatcher::OnDeselectObject(CRhinoDoc& doc, const CRhinoObject& object)
{
}

void CMarmaladeEventWatcher::OnDeselectAllObjects(CRhinoDoc& doc, int count)
{
}

void CMarmaladeEventWatcher::OnModifyObjectAttributes(CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes)
{
	if (!object.IsMeshable(ON::render_mesh))
		return;

	const CRhinoObjectAttributes& new_attributes = object.Attributes();

	if (old_attributes.MaterialSource() == new_attributes.MaterialSource())
	{
		if ((new_attributes.MaterialSource() == ON::material_from_object) && (old_attributes.m_material_index != new_attributes.m_material_index))
		{
			m_bRenderMeshModified = true;
		}
		else
		if ((new_attributes.MaterialSource() == ON::material_from_layer) && (old_attributes.m_layer_index != new_attributes.m_layer_index))
		{
			m_bRenderMeshModified = true;
		}
	}
	else
	{
		m_bRenderMeshModified = true;
	}
}

void CMarmaladeEventWatcher::OnUpdateObjectMesh(CRhinoDoc& doc, CRhinoObject& object, ON::mesh_type mesh_type)
{
	if (mesh_type == ON::render_mesh)
		m_bRenderMeshModified = true;
}

void CMarmaladeEventWatcher::LayerTableEvent(CRhinoEventWatcher::layer_event event, 
                                             const CRhinoLayerTable& layer_table,
                                             int layer_index, 
                                             const ON_Layer* old_settings)
{
	if (event != CRhinoEventWatcher::layer_modified)
		return;

	const CRhinoLayer& layer = layer_table[layer_index];

	if (old_settings)
	{
		if (layer.RenderMaterialIndex() != old_settings->RenderMaterialIndex())
			m_bRenderMeshModified = true;

		if (layer.IsVisible() != old_settings->IsVisible())
			m_bLightModified = m_bRenderMeshModified = true;

		if (layer.IsLocked() != old_settings->IsLocked())
			m_bLightModified = m_bRenderMeshModified = true;
	}
}

void CMarmaladeEventWatcher::LightTableEvent(CRhinoEventWatcher::light_event event, 
                                             const CRhinoLightTable& light_table, 
                                             int light_index, 
                                             const ON_Light* old_settings)
{
	switch(event)
	{
	case light_added:
	case light_undeleted:
		m_bLightAdded = true;
		break;

	case light_deleted:
		m_bLightDeleted = true;
		break;

	case light_modified:
		m_bLightModified = true;
		break;
	}
}

void CMarmaladeEventWatcher::MaterialTableEvent(CRhinoEventWatcher::material_event event,
                                                const CRhinoMaterialTable& material_table,
                                                int material_index,
                                                const ON_Material* old_settings)
{
	switch (event)
	{
	case CRhinoEventWatcher::material_added:
	case CRhinoEventWatcher::material_undeleted:
		m_bMaterialAdded = true;
		break;

	case CRhinoEventWatcher::material_deleted:
		m_bMaterialDeleted = true;
		break;

	case CRhinoEventWatcher::material_modified:
		m_bMaterialModified = true;
		break;
	}
}

void CMarmaladeEventWatcher::GroupTableEvent(CRhinoEventWatcher::group_event event, 
                                            const CRhinoGroupTable& group_table,
                                            int group_index, 
                                            const ON_Group* old_settings)
{
}

void CMarmaladeEventWatcher::FontTableEvent(CRhinoEventWatcher::font_event event, 
                                            const CRhinoFontTable& font_table,
                                            int font_index, 
                                            const ON_Font* old_settings)
{
}

void CMarmaladeEventWatcher::DimStyleTableEvent(CRhinoEventWatcher::dimstyle_event event, 
                                                const CRhinoDimStyleTable& dimstyle_table,
                                                int dimstyle_index, 
                                                const ON_DimStyle* old_settings)
{
}
