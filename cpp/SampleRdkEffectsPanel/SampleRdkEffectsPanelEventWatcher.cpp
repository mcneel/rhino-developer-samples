// SampleRdkEffectsPanelEventWatcher.cpp
//

#include "StdAfx.h"
#include "SampleRdkEffectsPanelEventWatcher.h"

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//

CSampleRdkEffectsPanelEventWatcher::CSampleRdkEffectsPanelEventWatcher()
{
  Defaults();
}

void CSampleRdkEffectsPanelEventWatcher::Defaults( BOOL b /*= FALSE*/)
{
  SetRenderMeshFlags( b);
  SetMaterialFlags( b);
  SetLightFlags( b);
}

/////////////////////////////////////////////////////////////////////////////
// CSampleRdkEffectsPanelEventWatcher methods
//

BOOL CSampleRdkEffectsPanelEventWatcher::RenderSceneModified() const
{
  return( MaterialModified() ||
          MaterialAdded() ||
          MaterialDeleted() ||
          RenderMeshModified() ||
          RenderMeshAdded() ||
          RenderMeshDeleted() ||
          RenderMeshVisibilityChanged()
        );
}

BOOL CSampleRdkEffectsPanelEventWatcher::RenderLightingModified() const
{
  return( LightModified() ||
          LightAdded() ||
          LightDeleted()
        );
}

BOOL CSampleRdkEffectsPanelEventWatcher::MaterialModified() const
{
  return m_material_modified;
}

BOOL CSampleRdkEffectsPanelEventWatcher::MaterialAdded() const
{
  return m_material_added;
}

BOOL CSampleRdkEffectsPanelEventWatcher::MaterialDeleted() const
{
  return m_material_deleted;
}

BOOL CSampleRdkEffectsPanelEventWatcher::RenderMeshModified() const
{
  return m_render_mesh_modified;
}

BOOL CSampleRdkEffectsPanelEventWatcher::RenderMeshAdded() const
{
  return m_render_mesh_added;
}

BOOL CSampleRdkEffectsPanelEventWatcher::RenderMeshDeleted() const
{
  return m_render_mesh_deleted;
}

BOOL CSampleRdkEffectsPanelEventWatcher::RenderMeshVisibilityChanged() const
{
  return m_render_mesh_visibility_changed;
}

BOOL CSampleRdkEffectsPanelEventWatcher::LightModified() const
{
  return m_light_modified;
}

BOOL CSampleRdkEffectsPanelEventWatcher::LightAdded() const
{
  return m_light_added;
}

BOOL CSampleRdkEffectsPanelEventWatcher::LightDeleted() const
{
  return m_light_deleted;
}

void CSampleRdkEffectsPanelEventWatcher::SetRenderMeshFlags( BOOL b /*= FALSE*/)
{
  m_render_mesh_modified = b;
  m_render_mesh_added = b;
  m_render_mesh_deleted = b;
  m_render_mesh_visibility_changed = b;
}

void CSampleRdkEffectsPanelEventWatcher::SetMaterialFlags( BOOL b /*= FALSE*/)
{
  m_material_modified = b;
  m_material_added = b;
  m_material_deleted = b;
}

void CSampleRdkEffectsPanelEventWatcher::SetLightFlags( BOOL b /*= FALSE*/)
{
  m_light_modified = b;
  m_light_added = b;
  m_light_deleted = b;
}

/////////////////////////////////////////////////////////////////////////////
// CRhinoEventWatcher overrides
//

void CSampleRdkEffectsPanelEventWatcher::OnEnableEventWatcher( BOOL b )
{
	UNREFERENCED_PARAMETER(b);
	Defaults();
}

void CSampleRdkEffectsPanelEventWatcher::OnInitRhino(CRhinoApp& app)
{
	UNREFERENCED_PARAMETER(app);
}

void CSampleRdkEffectsPanelEventWatcher::OnCloseRhino(CRhinoApp& app)
{
	UNREFERENCED_PARAMETER(app);
}

void CSampleRdkEffectsPanelEventWatcher::OnCloseDocument( CRhinoDoc& doc )
{
	UNREFERENCED_PARAMETER(doc);
}

void CSampleRdkEffectsPanelEventWatcher::OnNewDocument( CRhinoDoc& doc )
{
	UNREFERENCED_PARAMETER(doc);
	Defaults( true);
}

void CSampleRdkEffectsPanelEventWatcher::OnBeginOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(bMerge);
	UNREFERENCED_PARAMETER(bReference);
	Defaults( true);
}

void CSampleRdkEffectsPanelEventWatcher::OnEndOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(bMerge);
	UNREFERENCED_PARAMETER(bReference);
	Defaults( true);
}

void CSampleRdkEffectsPanelEventWatcher::OnBeginSaveDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(bExportSelected);
}

void CSampleRdkEffectsPanelEventWatcher::OnEndSaveDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(bExportSelected);
}

void CSampleRdkEffectsPanelEventWatcher::OnDocumentPropertiesChanged(CRhinoDoc& doc)
{
	UNREFERENCED_PARAMETER(doc);
}

void CSampleRdkEffectsPanelEventWatcher::OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context)
{
	UNREFERENCED_PARAMETER(command);
	UNREFERENCED_PARAMETER(context);
}

void CSampleRdkEffectsPanelEventWatcher::OnEndCommand(
        const CRhinoCommand& command,
        const CRhinoCommandContext& context,
        CRhinoCommand::result rc
        )
{
	UNREFERENCED_PARAMETER(command);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(rc);
}

void CSampleRdkEffectsPanelEventWatcher::OnAddObject( CRhinoDoc& doc, CRhinoObject& object )
{
	UNREFERENCED_PARAMETER(doc);

	// TODO: CHECK FOR LIGHTS
	if( object.IsMeshable(ON::render_mesh) )
		m_render_mesh_added = true;
}

void CSampleRdkEffectsPanelEventWatcher::OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object)
{
	UNREFERENCED_PARAMETER(doc);

	// TODO: CHECK FOR LIGHTS
	if (object.IsMeshable(ON::render_mesh))
		m_render_mesh_deleted = true;
}

void CSampleRdkEffectsPanelEventWatcher::OnReplaceObject( CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object )
{
	UNREFERENCED_PARAMETER(doc);

  // TODO: CHECK FOR LIGHTS
  ON_SimpleArray<const ON_Mesh*> old_meshes, new_meshes;

  BOOL bOldMeshes = (old_object.GetMeshes( ON::render_mesh, old_meshes) < 1);
  BOOL bNewMeshes = (new_object.GetMeshes( ON::render_mesh, new_meshes) < 1);

  if( bOldMeshes )
  {
    if( new_object.IsMeshable(ON::render_mesh) )
    {
      if( bNewMeshes )
        m_render_mesh_modified = true;
      else
        m_render_mesh_deleted = true;
    }
    else
    {
      m_render_mesh_deleted = true;
    }
  }
  else if( bNewMeshes )
  {
    m_render_mesh_added = true;
  }
  else if( new_object.IsMeshable(ON::render_mesh) )
  {
    m_render_mesh_added = true;
  }
}

void CSampleRdkEffectsPanelEventWatcher::OnUnDeleteObject( CRhinoDoc& doc, CRhinoObject& object )
{
	UNREFERENCED_PARAMETER(doc);

  // TODO: CHECK FOR LIGHTS
  if( object.IsMeshable(ON::render_mesh) )
    m_render_mesh_added = true;
}

void CSampleRdkEffectsPanelEventWatcher::OnPurgeObject( CRhinoDoc& doc, CRhinoObject& object)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(object);
}

void CSampleRdkEffectsPanelEventWatcher::OnSelectObject( CRhinoDoc& doc, const CRhinoObject& object)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(object);
}

void CSampleRdkEffectsPanelEventWatcher::OnDeselectObject( CRhinoDoc& doc, const CRhinoObject& object)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(object);
}

void CSampleRdkEffectsPanelEventWatcher::OnDeselectAllObjects( CRhinoDoc& doc, int count)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(count);
}

void CSampleRdkEffectsPanelEventWatcher::OnModifyObjectAttributes(
        CRhinoDoc& doc,
        CRhinoObject& object,
        const CRhinoObjectAttributes& old_attributes
        )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(object);

  if( !object.IsMeshable( ON::render_mesh))
    return;

  const CRhinoObjectAttributes& new_attributes = object.Attributes();

  if( old_attributes.MaterialSource() == new_attributes.MaterialSource())
  {
    if( new_attributes.MaterialSource() == ON::material_from_object
        && old_attributes.m_material_index != new_attributes.m_material_index)
    {
      m_render_mesh_modified = true;
    } else if( new_attributes.MaterialSource() == ON::material_from_layer
               && old_attributes.m_layer_index != new_attributes.m_layer_index)
    {
      m_render_mesh_modified = true;
    }
  } else
    m_render_mesh_modified = true;
}

void CSampleRdkEffectsPanelEventWatcher::OnUpdateObjectMesh(
        CRhinoDoc& doc,
        CRhinoObject& object,
        ON::mesh_type mesh_type
        )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(object);

  if( mesh_type == ON::render_mesh )
    m_render_mesh_modified = true;
}

void CSampleRdkEffectsPanelEventWatcher::LayerTableEvent(
        CRhinoEventWatcher::layer_event event,
        const CRhinoLayerTable& layer_table,
        int layer_index,
        const ON_Layer* old_settings
        )
{
  if( event != CRhinoEventWatcher::layer_modified )
    return;

  const CRhinoLayer& layer = layer_table[layer_index];

  if( old_settings )
  {
    if( layer.RenderMaterialIndex() != old_settings->RenderMaterialIndex() )
      m_render_mesh_modified = true;

    if( layer.IsVisible() != old_settings->IsVisible() )
      m_light_modified = m_render_mesh_modified = true;

    if( layer.IsLocked() != old_settings->IsLocked() )
      m_light_modified = m_render_mesh_modified = true;
  }
}

void CSampleRdkEffectsPanelEventWatcher::LightTableEvent(
        CRhinoEventWatcher::light_event event,
        const CRhinoLightTable& light_table,
        int light_index,
        const ON_Light* old_settings
        )
{
	UNREFERENCED_PARAMETER(old_settings);
	UNREFERENCED_PARAMETER(light_index);
	UNREFERENCED_PARAMETER(light_table);

  switch( event )
  {
    case light_added:
    case light_undeleted:
      m_light_added = true;
      break;
    case light_deleted:
      m_light_deleted = true;
      break;
    case light_modified:
      m_light_modified = true;
      break;
  }
}

void CSampleRdkEffectsPanelEventWatcher::MaterialTableEvent(
        CRhinoEventWatcher::material_event event,
        const CRhinoMaterialTable& material_table,
        int material_index,
        const ON_Material* old_settings
        )
{
	UNREFERENCED_PARAMETER(old_settings);
	UNREFERENCED_PARAMETER(material_index);
	UNREFERENCED_PARAMETER(material_table);

  switch( event)
  {
    case CRhinoEventWatcher::material_added:
    case CRhinoEventWatcher::material_undeleted:
      m_material_added = true;
      break;
    case CRhinoEventWatcher::material_deleted:
      m_material_deleted = true;
      break;
    case CRhinoEventWatcher::material_modified:
      m_material_modified = true;
      break;
  }
}

void CSampleRdkEffectsPanelEventWatcher::GroupTableEvent(
        CRhinoEventWatcher::group_event event,
        const CRhinoGroupTable& group_table,
        int group_index,
        const ON_Group* old_settings
        )
{
	UNREFERENCED_PARAMETER(event);
	UNREFERENCED_PARAMETER(group_table);
	UNREFERENCED_PARAMETER(group_index);
	UNREFERENCED_PARAMETER(old_settings);
}

void CSampleRdkEffectsPanelEventWatcher::DimStyleTableEvent(
        CRhinoEventWatcher::dimstyle_event event,
        const CRhinoDimStyleTable& dimstyle_table,
        int dimstyle_index,
        const ON_DimStyle* old_settings
        )
{
	UNREFERENCED_PARAMETER(event);
	UNREFERENCED_PARAMETER(dimstyle_table);
	UNREFERENCED_PARAMETER(dimstyle_index);
	UNREFERENCED_PARAMETER(old_settings);
}
