/////////////////////////////////////////////////////////////////////////////
// SampleRhinoEventWatcher.cpp
//

#include "StdAfx.h"
#include "SampleRhinoEventWatcher.h"

////////////////////////////////////////////////////////////////
// Document events

void CSampleRhinoEventWatcher::OnCloseDocument( CRhinoDoc& doc )
{
  RhinoApp().Print( L"** EVENT: Close Document **\n" );
}

void CSampleRhinoEventWatcher::OnNewDocument( CRhinoDoc& doc )
{
  RhinoApp().Print( L"** EVENT: New Document **\n" );
}

void CSampleRhinoEventWatcher::OnBeginOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference )
{
  RhinoApp().Print( L"** EVENT: Begin Open Document **\n" );
}

void CSampleRhinoEventWatcher::OnEndOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference )
{
  RhinoApp().Print( L"** EVENT: End Open Document **\n" );
}

void CSampleRhinoEventWatcher::OnBeginSaveDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected )
{
  RhinoApp().Print( L"** EVENT: Begin Save Document **\n" );
}

void CSampleRhinoEventWatcher::OnEndSaveDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bExportSelected )
{
  RhinoApp().Print( L"** EVENT: End Save Document **\n" );
}

void CSampleRhinoEventWatcher::OnDocumentPropertiesChanged( CRhinoDoc& doc )
{
  RhinoApp().Print( L"** EVENT: Document Properties Changed **\n" );
}

void CSampleRhinoEventWatcher::OnAppSettingsChanged( const CRhinoAppSettings& new_app_settings )
{
  RhinoApp().Print( L"** EVENT: Application Settings Changed **\n" );
}

////////////////////////////////////////////////////////////////
// View events

void CSampleRhinoEventWatcher::OnCreateView( CRhinoView* rhino_view )
{
  RhinoApp().Print( L"** EVENT: Create View **\n" );
}

void CSampleRhinoEventWatcher::OnDestroyView( CRhinoView* rhino_view )
{
  RhinoApp().Print( L"** EVENT: Destroy View **\n" );
}

void CSampleRhinoEventWatcher::OnSetActiveView( CRhinoView* rhino_view )
{
  RhinoApp().Print( L"** EVENT: Set Active View **\n" );
}

void CSampleRhinoEventWatcher::OnRenameView( CRhinoView* rhino_view )
{
  RhinoApp().Print( L"** EVENT: Rename View **\n" );
}

////////////////////////////////////////////////////////////////
// Command events

void CSampleRhinoEventWatcher::OnBeginCommand( const CRhinoCommand& command, const CRhinoCommandContext& context )
{
  RhinoApp().Print( L"** EVENT: Begin Command **\n" );
}

void CSampleRhinoEventWatcher::OnEndCommand( const CRhinoCommand& command, const CRhinoCommandContext& context, CRhinoCommand::result rc )
{
  RhinoApp().Print( L"** EVENT: End Command **\n" );
}

////////////////////////////////////////////////////////////////
// Object events

void CSampleRhinoEventWatcher::OnAddObject( CRhinoDoc& doc, CRhinoObject& object )
{
  RhinoApp().Print( L"** EVENT: Add Object **\n" );
}

void CSampleRhinoEventWatcher::OnDeleteObject( CRhinoDoc& doc, CRhinoObject& object )
{
  RhinoApp().Print( L"** EVENT: Delete Object **\n" );
}

void CSampleRhinoEventWatcher::OnReplaceObject( CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object )
{
  RhinoApp().Print( L"** EVENT: Replace Object **\n" );
  RhinoApp().Print( L"**      UndoActive() = %s **\n", doc.UndoActive() ? L"TRUE" : L"FALSE" );
  RhinoApp().Print( L"**      RedoActive() = %s **\n", doc.RedoActive() ? L"TRUE" : L"FALSE" );
}

void CSampleRhinoEventWatcher::OnUnDeleteObject( CRhinoDoc& doc, CRhinoObject& object )
{
  RhinoApp().Print( L"** EVENT: Undelete Object **\n" );
}

void CSampleRhinoEventWatcher::OnPurgeObject( CRhinoDoc& doc, CRhinoObject& object )
{
  RhinoApp().Print( L"** EVENT: Purge Object **\n" );
}

void CSampleRhinoEventWatcher::OnSelectObject( CRhinoDoc& doc, const CRhinoObject& object )
{
  RhinoApp().Print( L"** EVENT: Select Object **\n" );
}

void CSampleRhinoEventWatcher::OnSelectObjects( CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects )
{
  RhinoApp().Print( L"** EVENT: Select Objects **\n" );
}

void CSampleRhinoEventWatcher::OnDeselectObject( CRhinoDoc& doc, const CRhinoObject& object )
{
  RhinoApp().Print( L"** EVENT: Deselect Object **\n" );
}

void CSampleRhinoEventWatcher::OnDeselectObjects( CRhinoDoc& doc, const ON_SimpleArray<const CRhinoObject*>& objects )
{
  RhinoApp().Print( L"** EVENT: Deselect Objects **\n" );
}

void CSampleRhinoEventWatcher::OnDeselectAllObjects( CRhinoDoc& doc, int count )
{
  RhinoApp().Print( L"** EVENT: Deselect All Objects **\n" );
}

void CSampleRhinoEventWatcher::OnModifyObjectAttributes( CRhinoDoc& doc, CRhinoObject& object, const CRhinoObjectAttributes& old_attributes )
{
  RhinoApp().Print( L"** EVENT: Modify Object Attributes **\n" );
}

////////////////////////////////////////////////////////////////
// Table events

void CSampleRhinoEventWatcher::LayerTableEvent( CRhinoEventWatcher::layer_event type, const CRhinoLayerTable& layer_table, int layer_index, const ON_Layer* old_settings )
{
  RhinoApp().Print( L"** EVENT: Layer Table **\n" );
}

void CSampleRhinoEventWatcher::LinetypeTableEvent( CRhinoEventWatcher::linetype_event type, const CRhinoLinetypeTable& linetype_table, int linetype_index, const ON_Linetype* old_settings )
{
  RhinoApp().Print( L"** EVENT: Linetype Table **\n" );
}

void CSampleRhinoEventWatcher::InstanceDefinitionTableEvent( CRhinoEventWatcher::idef_event type, const CRhinoInstanceDefinitionTable& idef_table, int idef_index, const ON_InstanceDefinition* old_settings )
{
  RhinoApp().Print( L"** EVENT: Instance Definition Table **\n" );
}

void CSampleRhinoEventWatcher::LightTableEvent( CRhinoEventWatcher::light_event type, const CRhinoLightTable& light_table, int light_index, const ON_Light* old_settings )
{
  RhinoApp().Print( L"** EVENT: Light Table **\n" );
}

void CSampleRhinoEventWatcher::MaterialTableEvent( CRhinoEventWatcher::material_event type, const CRhinoMaterialTable& material_table, int material_index, const ON_Material* old_settings )
{
  RhinoApp().Print( L"** EVENT: Material Table **\n" );
}

void CSampleRhinoEventWatcher::TextureMappingTableEvent( CRhinoEventWatcher::texture_mapping_event type, const CRhinoTextureMappingTable& texture_mapping_table, int texture_mapping_index, const ON_TextureMapping* old_settings )
{
  RhinoApp().Print( L"** EVENT: Texture Mapping Table **\n" );
}

void CSampleRhinoEventWatcher::GroupTableEvent( CRhinoEventWatcher::group_event type, const CRhinoGroupTable& group_table, int group_index, const ON_Group* old_settings )
{
  RhinoApp().Print( L"** EVENT: Group Table **\n" );
}

void CSampleRhinoEventWatcher::FontTableEvent( CRhinoEventWatcher::font_event type, const CRhinoFontTable& font_table, int font_index, const ON_Font* old_settings )
{
  RhinoApp().Print( L"** EVENT: Font Table **\n" );
}

void CSampleRhinoEventWatcher::DimStyleTableEvent( CRhinoEventWatcher::dimstyle_event type, const CRhinoDimStyleTable& dimstyle_table, int dimstyle_index, const ON_DimStyle* old_settings )
{
  RhinoApp().Print( L"** EVENT: Dimension Style Table **\n" );
}

void CSampleRhinoEventWatcher::HatchPatternTableEvent( CRhinoEventWatcher::hatchpattern_event type, const CRhinoHatchPatternTable& hatchpattern_table, int hatchpattern_index, const ON_HatchPattern* old_settings )
{
  RhinoApp().Print( L"** EVENT: Hatch Pattern Table **\n" );
}

////////////////////////////////////////////////////////////////
// Undo event

void CSampleRhinoEventWatcher::UndoEvent(CRhinoEventWatcher::undo_event type, unsigned int undo_record_serialnumber, const CRhinoCommand* cmd)
{
  ON_wString str;
  switch (type)
  {
  case CRhinoEventWatcher::no_undo_event:
    str = L"No Undo Event";
    break;
  case CRhinoEventWatcher::begin_recording:
    str = L"Begin Recording";
    break;
  case CRhinoEventWatcher::end_recording:
    str = L"End Recording";
    break;
  case CRhinoEventWatcher::begin_undo:
    str = L"Begin Undo";
    break;
  case CRhinoEventWatcher::end_undo:
    str = L"End Undo";
    break;
  case CRhinoEventWatcher::begin_redo:
    str = L"Begin Undo";
    break;
  case CRhinoEventWatcher::end_redo:
    str = L"End Redo";
    break;
  case CRhinoEventWatcher::purge_record:
    str = L"Purge Record";
    break;
  }
  RhinoApp().Print(L"** EVENT: Undo, Type: %s **\n", (const wchar_t*)str);
}
