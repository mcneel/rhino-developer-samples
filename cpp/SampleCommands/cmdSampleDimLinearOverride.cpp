#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimLinearOverride command
//

#pragma region SampleDimLinearOverride command

class CCommandSampleDimLinearOverride : public CRhinoCommand
{
public:
  CCommandSampleDimLinearOverride() {}
  ~CCommandSampleDimLinearOverride() {}
  UUID CommandUUID()
  {
    // {56236AC2-709A-4842-BC4B-41671E7179B8}
    static const GUID SampleDimLinearOverrideCommand_UUID =
    { 0x56236AC2, 0x709A, 0x4842, { 0xBC, 0x4B, 0x41, 0x67, 0x1E, 0x71, 0x79, 0xB8 } };
    return SampleDimLinearOverrideCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDimLinearOverride"; }
  const wchar_t* LocalCommandName() const { return L"SampleDimLinearOverride"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDimLinearOverride object
static class CCommandSampleDimLinearOverride theSampleDimLinearOverrideCommand;

CRhinoCommand::result CCommandSampleDimLinearOverride::RunCommand( const CRhinoCommandContext& context )
{
  // To override a property of a dimension, you must first create a child
  // dimension style. In this example, we will create a child dimension
  // style of the current dimension style, override the text alignment
  // property, and then assign it to a new linear dimension.

  // Get reference to dimension style table
  CRhinoDimStyleTable& dimstyle_table = context.m_doc.m_dimstyle_table;

  // Get the current dimension style
  const CRhinoDimStyle& dimstyle = dimstyle_table.CurrentDimStyle();

  // Create a new dimenstion style based on the current dimension style
  ON_DimStyle child_dimstyle(dimstyle);
  // Make this dimstyle a child of the current dimension style
  child_dimstyle.SetParentId(dimstyle.m_dimstyle_id);
  // Change the name of the child dimension style
  dimstyle_table.GetUnusedDimStyleName(child_dimstyle.m_dimstyle_name );

  // Override with the text alignment field
  child_dimstyle.SetFieldOverride(ON_DimStyle::fn_textalign, true);
  // Set text alignment field to horizontal
  child_dimstyle.SetTextAlignment(ON::dtHorizontal);

  // Add the new child dimension style
  int child_dimstyle_index = dimstyle_table.AddDimStyle(child_dimstyle, false);

  // Create a new linear dimension object
  CRhinoLinearDimension* dim_obj = new CRhinoLinearDimension();
  // Set the dimension style to the newly added style
  dim_obj->SetStyleIndex(child_dimstyle_index);

  // Set some linear dimension points (example)
  dim_obj->SetPlane(ON_Plane::World_xy);
  dim_obj->SetPoint(0, ON_2dPoint(0.0, 0.0));
  dim_obj->SetPoint(2, ON_2dPoint(10,0) );
  ON_2dPoint dimline_point(5.0, 2.0);
  dim_obj->UpdateDimPoints(dimline_point);
  dim_obj->UpdateText();

  // Add linear dimension to the document
  CRhinoCommand::result rc = CRhinoCommand::success;
  if (context.m_doc.AddObject(dim_obj))
  {
    context.m_doc.Redraw();
  }
  else
  {
    delete dim_obj; // Don't leak...
    rc = CRhinoCommand::failure;
  }

  return rc;


  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDimLinearOverride command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
