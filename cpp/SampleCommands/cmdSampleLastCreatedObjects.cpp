#include "stdafx.h"

/*
Description:
  Retrieves a range of objects based on their runtime serial numbers.
Parameters:
  doc     - [in]  - A reference to the active document.
  first   - [in]  - The first runtime serial number.
  last    - [in]  - The first runtime serial number.
  objects - [out] - The objects.
Returns:
  The number of objects added to the output array,
See Also:
  CRhinoDoc::LookupObject
  CRhinoDoc::LookupObjectByRuntimeSerialNumber
*/
static int RhinoObjectIdsFromRuntimeSerialNumbers(
  CRhinoDoc& doc,
  unsigned int first,
  unsigned int last,
  ON_SimpleArray<const CRhinoObject*>& objects
)
{
  if (0 >= first || 0 >= last || first >= last)
    return 0;

  const int objects_Count = objects.Count();

  // Look up objects by their runtime serial number
  unsigned int i = 0;
  for (i = first; i < last; i++)
  {
    const CRhinoObject* obj = doc.LookupObjectByRuntimeSerialNumber(i);
    if (0 == obj || obj->IsDeleted() || obj->IsInstanceDefinitionGeometry())
      continue;

    objects.Append(obj);
  }

  // Cull duplicates objects
  const int count = objects.Count();
  int j, k;
  if (count > 0)
  {
    ON_SimpleArray<const CRhinoObject*> temp = objects;
    objects.SetCount(0);

    int* culled = (int*)onmalloc(count * sizeof(int));
    memset(culled, 0, count * sizeof(int));

    for (j = 0; j < count; j++)
    {
      if (!culled[j])
      {
        objects.Append(temp[j]);
        for (k = j + 1; k < count; k++)
        {
          if (culled[k])
            continue;

          if (0 == ON_UuidCompare(temp[k]->ModelObjectId(), temp[j]->ModelObjectId()))
            culled[k] = TRUE;
        }
      }
    }

    onfree((void*)culled);
  }

  return objects.Count() - objects_Count;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLastCreatedObjects command
//

#pragma region SampleLastCreatedObjects command

// Commands that call CRhinoApp::RunScript MUST be derived from
// CRhinoScriptCommand, not CRhinoCommand.
class CCommandSampleLastCreatedObjects : public CRhinoScriptCommand
{
public:
  CCommandSampleLastCreatedObjects() {}
  ~CCommandSampleLastCreatedObjects() {}
  UUID CommandUUID()
  {
    // {EA6611BE-EADA-4C09-8123-234A783334F0}
    static const GUID SampleLastCreatedObjectsCommand_UUID =
    { 0xEA6611BE, 0xEADA, 0x4C09, { 0x81, 0x23, 0x23, 0x4A, 0x78, 0x33, 0x34, 0xF0 } };
    return SampleLastCreatedObjectsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleLastCreatedObjects"; }
  const wchar_t* LocalCommandName() const { return L"SampleLastCreatedObjects"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleLastCreatedObjects object
static class CCommandSampleLastCreatedObjects theSampleLastCreatedObjectsCommand;

CRhinoCommand::result CCommandSampleLastCreatedObjects::RunCommand(const CRhinoCommandContext& context)
{
  // Select polysurfaces to explode
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select polysurfaces to explode");
  go.SetGeometryFilter(CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Make sure the objects are selected
  int i = 0;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObject* obj = go.Object(i).Object();
    if (obj)
      obj->Select(true);
  }

  // Get the runtime serial number that will be assigned to
  // the next CRhinoObject that is created.
  unsigned int first = CRhinoObject::NextRuntimeObjectSerialNumber();

  // Run the Explode command
  RhinoApp().RunScript(L"_-Explode", 0);

  // Get the runtime serial number that will be assigned to
  // the next CRhinoObject that is created.
  unsigned int last = CRhinoObject::NextRuntimeObjectSerialNumber();

  ON_SimpleArray<const CRhinoObject*> objects;

  // Get all of the CRhinoObject that were added to the document
  // by the Explode command
  if (RhinoObjectIdsFromRuntimeSerialNumbers(context.m_doc, first, last, objects))
  {
    for (i = 0; i < objects.Count(); i++)
    {
      const CRhinoObject* obj = objects[i];
      if (obj)
      {
        // Make sure the objects are unselected
        obj->Select(false);

        // Print the object's id to the command line
        ON_wString str;
        ON_UuidToString(obj->ModelObjectId(), str);
        RhinoApp().Print(L"%d - %s\n", i, str);
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleLastCreatedObjects command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
