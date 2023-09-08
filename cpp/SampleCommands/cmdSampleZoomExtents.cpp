#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleZoomExtents command
//

#pragma region SampleZoomExtents command

class CCommandSampleZoomExtents : public CRhinoCommand
{
public:
  CCommandSampleZoomExtents() = default;
  UUID CommandUUID() override
  {
    // {516C170F-58E1-4C10-B70B-9539622C14AD}
    static const GUID SampleZoomExtentsCommand_UUID =
    { 0x516C170F, 0x58E1, 0x4C10, { 0xB7, 0x0B, 0x95, 0x39, 0x62, 0x2C, 0x14, 0xAD } };
    return SampleZoomExtentsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleZoomExtents"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleZoomExtents object
static class CCommandSampleZoomExtents theSampleZoomExtentsCommand;

CRhinoCommand::result CCommandSampleZoomExtents::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_SimpleArray<CRhinoView*> views(16);

  const int view_count = doc->GetViewList(views, true, false);

  for (int i = 0; i < view_count; i++)
    RhinoDollyExtents(views[i]);  
  doc->Redraw();
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleZoomExtents command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
