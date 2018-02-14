#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePrintGripLocations command
//

#pragma region SamplePrintGripLocations command

class CCommandSamplePrintGripLocations : public CRhinoCommand
{
public:
  CCommandSamplePrintGripLocations() = default;
  ~CCommandSamplePrintGripLocations() = default;
  UUID CommandUUID() override
  {
    // {CE52DCB-9140-4F05-9747-287416F2D992}
    static const GUID SamplePrintGripLocationsCommand_UUID =
    { 0xCE52DCB, 0x9140, 0x4F05, { 0x97, 0x47, 0x28, 0x74, 0x16, 0xF2, 0xD9, 0x92 } };
    return SamplePrintGripLocationsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePrintGripLocations"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSamplePrintGripLocations object
static class CCommandSamplePrintGripLocations theSamplePrintGripLocationsCommand;

CRhinoCommand::result CCommandSamplePrintGripLocations::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select grips");
  go.SetGeometryFilter(CRhinoGetObject::grip_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const int object_count = go.ObjectCount();
  for (int i = 0; i < object_count; i++)
  {
    const CRhinoGripObject* grip = CRhinoGripObject::Cast(go.Object(i).Object());
    if (grip)
    {
      ON_3dPoint point = grip->GripLocation();

      ON_wString point_str;
      RhinoFormatPoint(point, point_str);

      RhinoApp().Print(L"Grip %d = %s\n", i, static_cast<const wchar_t*>(point_str));
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePrintGripLocations command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
