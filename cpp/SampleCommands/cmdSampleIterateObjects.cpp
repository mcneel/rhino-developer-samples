#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleIterateObjects command
//

#pragma region SampleIterateObjects command

class CCommandSampleIterateObjects : public CRhinoCommand
{
public:
  CCommandSampleIterateObjects() {}
  ~CCommandSampleIterateObjects() {}
  UUID CommandUUID()
  {
    // {E7FE5940-F1F4-4C49-ABC0-744651415013}
    static const GUID SampleIterateObjectsCommand_UUID =
    { 0xE7FE5940, 0xF1F4, 0x4C49,{ 0xAB, 0xC0, 0x74, 0x46, 0x51, 0x41, 0x50, 0x13 } };
    return SampleIterateObjectsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleIterateObjects"; }
  const wchar_t* LocalCommandName() const { return L"SampleIterateObjects"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context);

private:
  void IterateInstanceDefinition(const CRhinoInstanceDefinition* idef, ON_TextLog& dump);
};

// The one and only CCommandSampleIterateObjects object
static class CCommandSampleIterateObjects theSampleIterateObjectsCommand;

CRhinoCommand::result CCommandSampleIterateObjects::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString writer;
  ON_TextLog dump(writer);
  dump.SetIndentSize(4);

  const CRhinoObject* object = 0;
  int object_count = 0;

  // Create an object iterator to process all norrmal (e.g. not locked and not hidden)
  // objects that are stored in the active document.
  CRhinoObjectIterator it(CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_objects);
  for (object = it.First(); 0 != object; object = it.Next())
  {
    // Try casting as an instance object
    const CRhinoInstanceObject* iref_object = CRhinoInstanceObject::Cast(object);
    if (0 != iref_object)
    {
      // Try getting the instance object's definition
      const CRhinoInstanceDefinition* idef = iref_object->InstanceDefinition();
      if (0 != idef)
      {
        dump.Print(L"\x2514 Model object %d: %s (%s)\n", object_count++, iref_object->ShortDescription(false), idef->Name());
        IterateInstanceDefinition(iref_object->InstanceDefinition(), dump);
      }
    }
    else
    {
      // Just a regular model object
      dump.Print(L"\x2500 Model object %d: %s\n", object_count++, object->ShortDescription(false));
    }
  }

  RhinoApp().Print(L"%s\n", writer);

  return CRhinoCommand::success;
}

void CCommandSampleIterateObjects::IterateInstanceDefinition(const CRhinoInstanceDefinition* idef, ON_TextLog& dump)
{
  if (0 == idef)
    return;

  dump.PushIndent();
  ON_wString idef_name = idef->Name();

  const int idef_object_count = idef->ObjectCount();
  if (idef_object_count)
  {
    // Iterate through the instance definition's objects
    for (int i = 0; i < idef->ObjectCount(); i++)
    {
      const CRhinoObject* object = idef->Object(i);
      if (object)
      {
        // Try casting as an instance object
        const CRhinoInstanceObject* iref_object = CRhinoInstanceObject::Cast(object);
        if (0 != iref_object)
        {
          // Try getting the instance object's definition
          const CRhinoInstanceDefinition* idef2 = iref_object->InstanceDefinition();
          if (0 != idef2)
          {
            dump.Print(L"\x2514 %s object %d: %s (%s)\n", idef_name, i, iref_object->ShortDescription(false), idef2->Name());
            // Recursive...
            IterateInstanceDefinition(idef2, dump);
          }
        }
        else
        {
          // Just a regular instance definition object
          dump.Print(L"\x2500 %s object %d: %s\n", idef_name, i, object->ShortDescription(false));
        }
      }
    }
  }

  dump.PopIndent();
}

#pragma endregion

//
// END SampleIterateObjects command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
