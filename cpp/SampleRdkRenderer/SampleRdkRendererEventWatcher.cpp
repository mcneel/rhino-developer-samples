// SampleRdkRendererEventWatcher.cpp
//

#include "StdAfx.h"
#include "SampleRdkRendererEventWatcher.h"

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//

CSampleRdkRendererEventWatcher::CSampleRdkRendererEventWatcher()
{
  Defaults();
}

void CSampleRdkRendererEventWatcher::Defaults( BOOL b /*= FALSE*/)
{
  SetRenderMeshFlags( b);
  SetMaterialFlags( b);
  SetLightFlags( b);
}

/////////////////////////////////////////////////////////////////////////////
// CSampleRdkRendererEventWatcher methods
//

BOOL CSampleRdkRendererEventWatcher::RenderSceneModified() const
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

BOOL CSampleRdkRendererEventWatcher::RenderLightingModified() const
{
  return( LightModified() ||
          LightAdded() ||
          LightDeleted()
        );
}

BOOL CSampleRdkRendererEventWatcher::MaterialModified() const
{
  return m_material_modified;
}

BOOL CSampleRdkRendererEventWatcher::MaterialAdded() const
{
  return m_material_added;
}

BOOL CSampleRdkRendererEventWatcher::MaterialDeleted() const
{
  return m_material_deleted;
}

BOOL CSampleRdkRendererEventWatcher::RenderMeshModified() const
{
  return m_render_mesh_modified;
}

BOOL CSampleRdkRendererEventWatcher::RenderMeshAdded() const
{
  return m_render_mesh_added;
}

BOOL CSampleRdkRendererEventWatcher::RenderMeshDeleted() const
{
  return m_render_mesh_deleted;
}

BOOL CSampleRdkRendererEventWatcher::RenderMeshVisibilityChanged() const
{
  return m_render_mesh_visibility_changed;
}

BOOL CSampleRdkRendererEventWatcher::LightModified() const
{
  return m_light_modified;
}

BOOL CSampleRdkRendererEventWatcher::LightAdded() const
{
  return m_light_added;
}

BOOL CSampleRdkRendererEventWatcher::LightDeleted() const
{
  return m_light_deleted;
}

void CSampleRdkRendererEventWatcher::SetRenderMeshFlags( BOOL b /*= FALSE*/)
{
  m_render_mesh_modified = b;
  m_render_mesh_added = b;
  m_render_mesh_deleted = b;
  m_render_mesh_visibility_changed = b;
}

void CSampleRdkRendererEventWatcher::SetMaterialFlags( BOOL b /*= FALSE*/)
{
  m_material_modified = b;
  m_material_added = b;
  m_material_deleted = b;
}

void CSampleRdkRendererEventWatcher::SetLightFlags( BOOL b /*= FALSE*/)
{
  m_light_modified = b;
  m_light_added = b;
  m_light_deleted = b;
}

/////////////////////////////////////////////////////////////////////////////
// CRhinoEventWatcher overrides
//

void CSampleRdkRendererEventWatcher::OnEnableEventWatcher( BOOL b )
{
	UNREFERENCED_PARAMETER(b);
	Defaults();
}

void CSampleRdkRendererEventWatcher::OnInitRhino(CRhinoApp& app)
{
	UNREFERENCED_PARAMETER(app);
}

void CSampleRdkRendererEventWatcher::OnCloseRhino(CRhinoApp& app)
{
	UNREFERENCED_PARAMETER(app);
}

void CSampleRdkRendererEventWatcher::OnCloseDocument( CRhinoDoc& doc )
{
	UNREFERENCED_PARAMETER(doc);
}

void CSampleRdkRendererEventWatcher::OnNewDocument( CRhinoDoc& doc )
{
	UNREFERENCED_PARAMETER(doc);
	Defaults( true);
}

void CSampleRdkRendererEventWatcher::OnBeginOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(bMerge);
	UNREFERENCED_PARAMETER(bReference);
	Defaults( true);
}

void CSampleRdkRendererEventWatcher::OnEndOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(bMerge);
	UNREFERENCED_PARAMETER(bReference);
	Defaults( true);
}

void CSampleRdkRendererEventWatcher::OnBeginSaveDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(bExportSelected);
}

void CSampleRdkRendererEventWatcher::OnEndSaveDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected )
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(bExportSelected);
}

void CSampleRdkRendererEventWatcher::OnDocumentPropertiesChanged(CRhinoDoc& doc)
{
	UNREFERENCED_PARAMETER(doc);
}

void CSampleRdkRendererEventWatcher::OnBeginCommand(const CRhinoCommand& command, const CRhinoCommandContext& context)
{
	UNREFERENCED_PARAMETER(command);
	UNREFERENCED_PARAMETER(context);
}

void CSampleRdkRendererEventWatcher::OnEndCommand(
        const CRhinoCommand& command,
        const CRhinoCommandContext& context,
        CRhinoCommand::result rc
        )
{
	UNREFERENCED_PARAMETER(command);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(rc);
}

void CSampleRdkRendererEventWatcher::OnAddObject( CRhinoDoc& doc, CRhinoObject& object )
{
	UNREFERENCED_PARAMETER(doc);

	// TODO: CHECK FOR LIGHTS
	if( object.IsMeshable(ON::render_mesh) )
		m_render_mesh_added = true;
}

void CSampleRdkRendererEventWatcher::OnDeleteObject(CRhinoDoc& doc, CRhinoObject& object)
{
	UNREFERENCED_PARAMETER(doc);

	// TODO: CHECK FOR LIGHTS
	if (object.IsMeshable(ON::render_mesh))
		m_render_mesh_deleted = true;
}

void CSampleRdkRendererEventWatcher::OnReplaceObject( CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object )
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

void CSampleRdkRendererEventWatcher::OnUnDeleteObject( CRhinoDoc& doc, CRhinoObject& object )
{
	UNREFERENCED_PARAMETER(doc);

  // TODO: CHECK FOR LIGHTS
  if( object.IsMeshable(ON::render_mesh) )
    m_render_mesh_added = true;
}

void CSampleRdkRendererEventWatcher::OnPurgeObject( CRhinoDoc& doc, CRhinoObject& object)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(object);
}

void CSampleRdkRendererEventWatcher::OnSelectObject( CRhinoDoc& doc, const CRhinoObject& object)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(object);
}

void CSampleRdkRendererEventWatcher::OnDeselectObject( CRhinoDoc& doc, const CRhinoObject& object)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(object);
}

void CSampleRdkRendererEventWatcher::OnDeselectAllObjects( CRhinoDoc& doc, int count)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(count);
}

void CSampleRdkRendererEventWatcher::OnModifyObjectAttributes(
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

void CSampleRdkRendererEventWatcher::OnUpdateObjectMesh(
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

void CSampleRdkRendererEventWatcher::LayerTableEvent(
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

void CSampleRdkRendererEventWatcher::LightTableEvent(
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

void CSampleRdkRendererEventWatcher::MaterialTableEvent(
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

void CSampleRdkRendererEventWatcher::GroupTableEvent(
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

void CSampleRdkRendererEventWatcher::DimStyleTableEvent(
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
