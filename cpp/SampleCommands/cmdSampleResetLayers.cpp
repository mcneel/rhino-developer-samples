#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleResetLayers command
//

#pragma region SampleResetLayers command

class CCommandSampleResetLayers : public CRhinoCommand
{
public:
  CCommandSampleResetLayers() {}
  ~CCommandSampleResetLayers() {}
  UUID CommandUUID()
  {
    // {1FBB2AD5-14A6-423B-8F2D-C975DB99C9B5}
    static const GUID SampleResetLayersCommand_UUID =
    { 0x1FBB2AD5, 0x14A6, 0x423B,{ 0x8F, 0x2D, 0xC9, 0x75, 0xDB, 0x99, 0xC9, 0xB5 } };
    return SampleResetLayersCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleResetLayers"; }
  const wchar_t* LocalCommandName() const { return L"SampleResetLayers"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleResetLayers object
static class CCommandSampleResetLayers theSampleResetLayersCommand;

CRhinoCommand::result CCommandSampleResetLayers::RunCommand(const CRhinoCommandContext& context)
{
  // Resets all layers by removing their per-viewport settings

  CRhinoLayerTable& layer_table = context.m_doc.m_layer_table;
  for (int i = 0; i < layer_table.LayerCount(); i++)
  {
    const CRhinoLayer& layer = layer_table[i];

    // If a layer is visible, the sublayer flag should be true
    if (layer.IsVisible() && !layer.PersistentVisibility())
      RhinoApp().Print(L"%s visible but not persistent visible\n", layer.Name());

    if (!layer.IsLocked() && layer.PersistentLocking())
      RhinoApp().Print(L"%s not locked but persistent locked\n", layer.Name());

    ON_Layer on_layer(layer);
    on_layer.SetVisible(true);
    on_layer.UnsetPersistentVisibility();
    on_layer.SetLocked(false);
    on_layer.UnsetPersistentLocking();
    on_layer.DeletePerViewportSettings(ON_nil_uuid);

    layer_table.ModifyLayer(on_layer, layer.Index());
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleResetLayers command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
