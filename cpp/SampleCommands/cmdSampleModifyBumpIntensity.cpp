#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleModifyBumpIntensity command
//

class CCommandSampleModifyBumpIntensity : public CRhinoCommand
{
public:
  CCommandSampleModifyBumpIntensity() = default;
  ~CCommandSampleModifyBumpIntensity() = default;
  UUID CommandUUID() override
  {
    // {AEB0DBEB-9904-4367-AB1A-53919D7EA37A}
    static const GUID SampleModifyBumpIntensityCommand_UUID =
    { 0xAEB0DBEB, 0x9904, 0x4367, { 0xAB, 0x1A, 0x53, 0x91, 0x9D, 0x7E, 0xA3, 0x7A } };
    return SampleModifyBumpIntensityCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleModifyBumpIntensity"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleModifyBumpIntensity object
static class CCommandSampleModifyBumpIntensity theSampleModifyBumpIntensityCommand;

CRhinoCommand::result CCommandSampleModifyBumpIntensity::RunCommand(const CRhinoCommandContext& context)
{
  auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object to modify bump intensity");
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const auto* obj = go.Object(0).Object();
  if (nullptr == obj)
    return CRhinoCommand::failure;

  const auto* pContent = pDoc->Contents().Find(obj->ObjectMaterial().RdkMaterialInstanceId());
  if (nullptr == pContent)
  {
    // I'm assuming the object already has a material. That is, it is not just using the default material.
    RhinoApp().Print(L"Object does not have a material.\n");
    return CRhinoCommand::failure;
  }

  const auto amount = pContent->ChildSlotAmount(CS_MAT_BUMP_TEXTURE).AsInteger();
  CRhinoGetInteger gi;
  gi.SetCommandPrompt(L"New bump intensity");
  gi.SetDefaultInteger(amount);
  gi.SetLowerLimit(0, false);
  gi.SetUpperLimit(100, false);
  gi.GetInteger();
  if (gi.CommandResult() != CRhinoCommand::success)
    return gi.CommandResult();

  auto& c = pContent->BeginChange(RhRdkChangeContext::UI);
  c.SetChildSlotAmount(CS_MAT_BUMP_TEXTURE, double(gi.Number()));
  c.EndChange();

  pDoc->Redraw();

  return success;
}

//
// END SampleModifyBumpIntensity command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
