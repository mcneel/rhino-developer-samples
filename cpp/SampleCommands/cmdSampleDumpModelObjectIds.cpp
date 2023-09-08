#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDumpModelObjectIds command
//

#pragma region SampleDumpModelObjectIds command

class CCommandSampleDumpModelObjectIds : public CRhinoCommand
{
public:
  CCommandSampleDumpModelObjectIds() = default;
  ~CCommandSampleDumpModelObjectIds() = default;
  UUID CommandUUID() override
  {
    // {1169AA23-84A7-43E2-B590-6181DF051CA9}
    static const GUID SampleDumpModelObjectIdsCommand_UUID =
    { 0x1169AA23, 0x84A7, 0x43E2, { 0xB5, 0x90, 0x61, 0x81, 0xDF, 0x05, 0x1C, 0xA9 } };
    return SampleDumpModelObjectIdsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDumpModelObjectIds"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleDumpModelObjectIds object
static class CCommandSampleDumpModelObjectIds theSampleDumpModelObjectIdsCommand;

CRhinoCommand::result CCommandSampleDumpModelObjectIds::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  //////////////////////
  // Method 1:

  RhinoApp().Print(L"ModelIDs by Layer:\n");
  {
    const CRhinoLayerTable& layer_table = doc->m_layer_table;
    int i, j, layer_count = layer_table.LayerCount();

    for (i = 0; i < layer_count; i++)
    {
      const CRhinoLayer& layer = layer_table[i];
      if (layer.IsVisible() && !layer.IsDeleted() && !layer.IsReference())
      {
        RhinoApp().Print(L"  %ls:\n", static_cast<const wchar_t*>(layer.Name()));

        ON_SimpleArray<CRhinoObject*> object_list;
        int object_count = doc->LookupObject(layer, object_list);
        if (object_count)
        {
          for (j = 0; j < object_count; j++)
          {
            const CRhinoObject* object = object_list[j];
            if (object)
            {
              ON_wString model_id;
              ON_UuidToString(object->ModelObjectId(), model_id);
              RhinoApp().Print(L"    %d: %ls\n", j, static_cast<const wchar_t*>(model_id));
            }
          }
        }
      }
    }
  }

  //////////////////////
  // Method 2:

  RhinoApp().Print(L"\nModelIDs by document:\n");
  {
    int count = 0;
    const CRhinoObject* object = 0;
    CRhinoObjectIterator it(CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_objects);
    for (object = it.First(); 0 != object; object = it.Next())
    {
      ON_wString model_id;
      ON_UuidToString(object->ModelObjectId(), model_id);
      RhinoApp().Print(L"    %d: %ls\n", count++, static_cast<const wchar_t*>(model_id));
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDumpModelObjectIds command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
