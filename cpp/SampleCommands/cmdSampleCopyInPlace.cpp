#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCopyInPlace command
//

#pragma region SampleCopyInPlace command

class CCommandSampleCopyInPlace : public CRhinoCommand
{
public:
  CCommandSampleCopyInPlace() = default;
  UUID CommandUUID() override
  {
    // {3641D4B9-4808-48AC-8863-3B74C9FF07C3}
    static const GUID SampleCopyInPlaceCommand_UUID =
    { 0x3641D4B9, 0x4808, 0x48AC, { 0x88, 0x63, 0x3B, 0x74, 0xC9, 0xFF, 0x07, 0xC3 } };
    return SampleCopyInPlaceCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCopyInPlace"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleCopyInPlace object
static class CCommandSampleCopyInPlace theSampleCopyInPlaceCommand;

CRhinoCommand::result CCommandSampleCopyInPlace::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  unsigned int geometry_filter = (unsigned int)CRhinoGetObject::any_object;
  geometry_filter &= ~CRhinoGetObject::extrusion_object;
  geometry_filter &= ~CRhinoGetObject::light_object;
  geometry_filter &= ~CRhinoGetObject::loop_object;
  geometry_filter &= ~CRhinoGetObject::edge_object;
  geometry_filter &= ~CRhinoGetObject::polyedge_object;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to copy in-place");
  go.SetGeometryFilter(geometry_filter);
  go.EnableSubObjectSelect(FALSE);
  go.EnableGroupSelect();
  go.EnableIgnoreGrips();
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  ON_2dexMap group_map;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& objref = go.Object(i);
    const CRhinoObject* obj = objref.Object();
    if (nullptr != obj)
    {
      // Copy the object
      CRhinoObject* dupe = obj->Duplicate();
      if (nullptr != dupe)
      {
        // In some situations, a bunch of objects are copied, new
        // new groups need to be created.This function handles the
        // details.
        RhinoUpdateObjectGroups(dupe, group_map);
        if (!doc->AddObject(dupe))
        {
          delete dupe;
          dupe = nullptr;
        }
      }
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCopyInPlace command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
