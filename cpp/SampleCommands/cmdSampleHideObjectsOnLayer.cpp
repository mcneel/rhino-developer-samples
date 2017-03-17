#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleHideObjectsOnLayer command
//

#pragma region SampleHideObjectsOnLayer command

class CCommandSampleHideObjectsOnLayer : public CRhinoCommand
{
public:
  CCommandSampleHideObjectsOnLayer() {}
  ~CCommandSampleHideObjectsOnLayer() {}
  UUID CommandUUID()
  {
    // {4326C435-2835-4C0D-BDE9-2C80D632E113}
    static const GUID SampleHideObjectsOnLayerCommand_UUID =
    { 0x4326C435, 0x2835, 0x4C0D,{ 0xBD, 0xE9, 0x2C, 0x80, 0xD6, 0x32, 0xE1, 0x13 } };
    return SampleHideObjectsOnLayerCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleHideObjectsOnLayer"; }
  const wchar_t* LocalCommandName() const { return L"SampleHideObjectsOnLayer"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleHideObjectsOnLayer object
static class CCommandSampleHideObjectsOnLayer theSampleHideObjectsOnLayerCommand;

CRhinoCommand::result CCommandSampleHideObjectsOnLayer::RunCommand(const CRhinoCommandContext& context)
{
  const CRhinoLayerTable& layer_table = context.m_doc.m_layer_table;
  const ON_wString& current_layer_name = layer_table.CurrentLayer().Name();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Layer to hide objects");
  gs.SetDefaultString(current_layer_name);
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  ON_wString layer_name = gs.String();
  layer_name.TrimLeftAndRight();
  if (layer_name.IsEmpty())
    return CRhinoCommand::nothing;

  const int layer_index = layer_table.FindLayerFromFullPathName(layer_name, -1);
  if (layer_index < 0)
  {
    RhinoApp().Print(L"The \"%s\" layer was not found.\n", layer_name);
    return CRhinoCommand::nothing;
  }

  const CRhinoLayer& layer = layer_table[layer_index];
  ON_SimpleArray<CRhinoObject*> objects;
  const int object_count = context.m_doc.LookupObject(layer, objects);

  for (int i = 0; i < object_count; i++)
  {
    const CRhinoObject* obj = objects[i];
    if (obj && obj->IsVisible())
      context.m_doc.HideObject(CRhinoObjRef(obj));
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleHideObjectsOnLayer command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
