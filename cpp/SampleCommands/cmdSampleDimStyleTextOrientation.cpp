#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimStyleTextOrientation command
//

#pragma region SampleDimStyleTextOrientation command

class CCommandSampleDimStyleTextOrientation : public CRhinoCommand
{
public:
  CCommandSampleDimStyleTextOrientation() = default;
  UUID CommandUUID() override
  {
    // {3F28B073-2055-4A4A-B243-8DBD0A8D4CE2}
    static const GUID SampleDimStyleTextOrientationCommand_UUID =
    { 0x3F28B073, 0x2055, 0x4A4A, { 0xB2, 0x43, 0x8D, 0xBD, 0x0A, 0x8D, 0x4C, 0xE2 } };
    return SampleDimStyleTextOrientationCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDimStyleTextOrientation"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDimStyleTextOrientation object
static class CCommandSampleDimStyleTextOrientation theSampleDimStyleTextOrientationCommand;

CRhinoCommand::result CCommandSampleDimStyleTextOrientation::RunCommand(const CRhinoCommandContext& context)
{
  // Get the current dimstyle
  const CRhinoDimStyle& dimstyle = context.m_doc.m_dimstyle_table.CurrentDimStyle();
  const int dim_style_index = dimstyle.Index();

  // Toggle text orientation
  ON::TextOrientation text_orientation = dimstyle.TextOrientation() == ON::TextOrientation::InPlane 
    ? ON::TextOrientation::InView 
    : ON::TextOrientation::InPlane;

  // Copy the dimstyle
  ON_DimStyle modified_dimstyle(context.m_doc.m_dimstyle_table[dim_style_index]);

  // Modify the text orientation
  modified_dimstyle.SetTextOrientation(text_orientation);

  // Modify the dimension style
  if (context.m_doc.m_dimstyle_table.ModifyDimStyle(modified_dimstyle, dim_style_index))
    context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDimStyleTextOrientation command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
