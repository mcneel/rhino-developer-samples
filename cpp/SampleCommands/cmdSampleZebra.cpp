#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleZebra command
//

#pragma region SampleZebra command

class CCommandSampleZebra : public CRhinoCommand
{
public:
  CCommandSampleZebra() = default;
  UUID CommandUUID() override
  {
    // {E5BFA563-DBFF-4B93-80F6-70D56EE9BDE5}
    static const GUID SampleZebraCommand_UUID =
    { 0xE5BFA563, 0xDBFF, 0x4B93, { 0x80, 0xF6, 0x70, 0xD5, 0x6E, 0xE9, 0xBD, 0xE5 } };
    return SampleZebraCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleZebra"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleZebra object
static class CCommandSampleZebra theSampleZebraCommand;

CRhinoCommand::result CCommandSampleZebra::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surfaces or polysurfaces for Zebra analysis");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObject* obj = go.Object(i).Object();
    if (nullptr != obj)
      obj->EnableAnalysisMode(RHINO_ZEBRA_STRIPE_ANALYSIS_MODE_ID, true);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleZebra command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
