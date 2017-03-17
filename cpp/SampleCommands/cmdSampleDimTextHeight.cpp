#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimTextHeight command
//

class CRhinoGetDimObject : public CRhinoGetObject
{
public:
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const override;
};

bool CRhinoGetDimObject::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry*, ON_COMPONENT_INDEX) const
{
  const CRhinoDimension* dim_obj = CRhinoDimension::Cast(object);
  return (nullptr != dim_obj);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleDimTextHeight command

class CCommandSampleDimTextHeight : public CRhinoCommand
{
public:
  CCommandSampleDimTextHeight() = default;
  ~CCommandSampleDimTextHeight() = default;
  UUID CommandUUID() override
  {
    // {6B8CB155-D910-4EA3-BED3-7276DBABACAB}
    static const GUID SampleDimTextHeightCommand_UUID =
    { 0x6B8CB155, 0xD910, 0x4EA3, { 0xBE, 0xD3, 0x72, 0x76, 0xDB, 0xAB, 0xAC, 0xAB } };
    return SampleDimTextHeightCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDimTextHeight"; }
  const wchar_t* LocalCommandName() const override { return L"SampleDimTextHeight"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDimTextHeight object
static class CCommandSampleDimTextHeight theSampleDimTextHeightCommand;

CRhinoCommand::result CCommandSampleDimTextHeight::RunCommand(const CRhinoCommandContext& context)
{
  // Select a dimension
  CRhinoGetDimObject go;
  go.SetCommandPrompt(L"Select dimension to modify text height");
  go.SetGeometryFilter(CRhinoGetObject::annotation_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Validate selection
  const CRhinoObjRef& object_ref = go.Object(0);
  const CRhinoDimension* dim_obj = CRhinoDimension::Cast(object_ref.Object());
  if (nullptr == dim_obj)
    return CRhinoCommand::failure;

  const ON_DimStyle& style = dim_obj->GetEffectiveDimensionStyle(&context.m_doc);

  // Prompt for a new text height value
  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"New text height for dimension");
  gn.SetDefaultNumber(style.TextHeight());
  gn.SetLowerLimit(0.0, TRUE);
  gn.AcceptNothing();
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  // New text height value
  double height = gn.Number();

  // Validate new value
  if (height != style.TextHeight() && ON_IsValid(height) && height > ON_SQRT_EPSILON)
  {
    // Is this dimstyle a child (has it been overridden already)?
    if (style.IsChildDimstyle())
    {
      // Copy everything from the dimension's dimstyle
      ON_DimStyle modified_style(style);

      // Override with the text height field
      modified_style.SetFieldOverride(ON_DimStyle::field::TextHeight, true);
      modified_style.SetTextHeight(height);

      // Modify the dimension style
      context.m_doc.m_dimstyle_table.ModifyDimStyle(modified_style, style.Index());
    }
    else
    {
      // Copy everything from the dimension's dimstyle
      ON_DimStyle child_dimstyle(style);

      // Override with the text height field
      child_dimstyle.SetFieldOverride(ON_DimStyle::field::TextHeight, true);
      child_dimstyle.SetTextHeight(height);

      // Add the new child dimstyle
      int new_style_index = context.m_doc.m_dimstyle_table.OverrideDimStyle(child_dimstyle, style.Index());
      const ON_DimStyle* new_style = &context.m_doc.m_dimstyle_table[new_style_index];
      if (nullptr == new_style)
        return CRhinoCommand::result::failure;

      // Modify the dimension to reflect the new child dimstyle style_index
      CRhinoDimension* new_obj = dim_obj->Duplicate();
      const ON_Dimension* dim = ON_Dimension::Cast(new_obj->Geometry());
      const_cast<ON_Dimension*>(dim)->DimensionStyle(*new_style);

      context.m_doc.ReplaceObject(object_ref, new_obj);
    }

    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDimTextHeight command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
