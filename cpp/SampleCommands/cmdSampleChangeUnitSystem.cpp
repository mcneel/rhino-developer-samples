#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleChangeUnitSystem command
//

#pragma region SampleChangeUnitSystem command

class CCommandSampleChangeUnitSystem : public CRhinoCommand
{
public:
  CCommandSampleChangeUnitSystem() = default;
  UUID CommandUUID() override
  {
    // {31ADEBF5-AD99-448F-951D-72D01DFAA201}
    static const GUID SampleChangeUnitSystemCommand_UUID =
    { 0x31ADEBF5, 0xAD99, 0x448F, { 0x95, 0x1D, 0x72, 0xD0, 0x1D, 0xFA, 0xA2, 0x01 } };
    return SampleChangeUnitSystemCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleChangeUnitSystem"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleChangeUnitSystem object
static class CCommandSampleChangeUnitSystem theSampleChangeUnitSystemCommand;

CRhinoCommand::result CCommandSampleChangeUnitSystem::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  // Get the model units and tolerance settings
  ON_3dmUnitsAndTolerances units_tolerances = doc->Properties().ModelUnitsAndTolerances();
  if (units_tolerances.m_unit_system.UnitSystem() != ON::LengthUnitSystem::Inches)
  {
    units_tolerances.m_unit_system.SetUnitSystem(ON::LengthUnitSystem::Inches);
    bool bScaleModelObjects = true;
    // Set the model units and tolerance settings
    doc->Properties().SetModelUnitsAndTolerances(units_tolerances, bScaleModelObjects);
    doc->Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleChangeUnitSystem command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
