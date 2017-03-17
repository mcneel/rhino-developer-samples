#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimTextHeight command
//

class CRhinoGetDimObject : public CRhinoGetObject
{
public:
  bool CustomGeometryFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
    ) const;
};

bool CRhinoGetDimObject::CustomGeometryFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
    ) const
{
  bool rc = false;

  const CRhinoAnnotationObject* annotation_object = CRhinoAnnotationObject::Cast( object );
  if( 0 != annotation_object )
  {
    switch( annotation_object->Type() )
    {
    case ON::dtDimLinear:
    case ON::dtDimAligned:
    case ON::dtDimAngular:
    case ON::dtDimDiameter:
    case ON::dtDimRadius:
    case ON::dtLeader:
    case ON::dtDimOrdinate:
      rc = true;
      break;
  
    default:
      break;
    }
  }

  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleDimTextHeight command

class CCommandSampleDimTextHeight : public CRhinoCommand
{
public:
  CCommandSampleDimTextHeight() {}
  ~CCommandSampleDimTextHeight() {}
  UUID CommandUUID()
  {
    // {6B8CB155-D910-4EA3-BED3-7276DBABACAB}
    static const GUID SampleDimTextHeightCommand_UUID =
    { 0x6B8CB155, 0xD910, 0x4EA3, { 0xBE, 0xD3, 0x72, 0x76, 0xDB, 0xAB, 0xAC, 0xAB } };
    return SampleDimTextHeightCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDimTextHeight"; }
  const wchar_t* LocalCommandName() const { return L"SampleDimTextHeight"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDimTextHeight object
static class CCommandSampleDimTextHeight theSampleDimTextHeightCommand;

CRhinoCommand::result CCommandSampleDimTextHeight::RunCommand( const CRhinoCommandContext& context )
{
  // Select a dimension
  CRhinoGetDimObject go;
  go.SetCommandPrompt( L"Select dimension to modify text height" );
  go.SetGeometryFilter( CRhinoGetObject::annotation_object );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  // Validate selection
  const CRhinoObjRef& object_ref = go.Object(0);
  const CRhinoAnnotationObject* annotation_object = CRhinoAnnotationObject::Cast( object_ref.Object() );
  if( 0 == annotation_object )
    return CRhinoCommand::failure;

  // Make a copy of of the dimension's dimstyle
  ON_DimStyle dimstyle = annotation_object->Style();

  // Prompt for a new text height value
  CRhinoGetNumber gn;
  gn.SetCommandPrompt( L"New text height for dimension" );
  gn.SetDefaultNumber( dimstyle.TextHeight() );
  gn.SetLowerLimit( 0.0, TRUE );
  gn.AcceptNothing();
  gn.GetNumber();
  if( gn.CommandResult() != CRhinoCommand::success )
    return gn.CommandResult();

  // New text height value
  double height = gn.Number();

  // Validate new value
  if( height != dimstyle.TextHeight() && ON_IsValid(height) && height > ON_SQRT_EPSILON )
  {
    // Reference to dimstyle table
    CRhinoDimStyleTable& dimstyle_table = context.m_doc.m_dimstyle_table;

    // Is this dimstyle a child (has it been overridden already)?
    if( dimstyle.IsChildDimstyle() )
    {
      // This dimension already references a child dimstyle
      int style_index = annotation_object->StyleIndex();
      if( style_index >= 0 )
      {
        // Copy everything from the dimension's dimstyle
        ON_DimStyle modified_dimstyle( dimstyle );

        // Override with the text height field
        modified_dimstyle.SetFieldOverride( ON_DimStyle::fn_textheight, true );
        modified_dimstyle.SetTextHeight( height );
          
        // Modify the dimension style
        dimstyle_table.ModifyDimStyle( modified_dimstyle, style_index );
      }
    }
    else
    {
      // Copy everything from the dimension's dimstyle
      ON_DimStyle child_dimstyle( dimstyle );

      // Override with the text height field
      child_dimstyle.SetFieldOverride( ON_DimStyle::fn_textheight, true );
      child_dimstyle.SetTextHeight( height );

      // Make this dimstyle a child
      child_dimstyle.SetParentId( dimstyle.m_dimstyle_id );

      // Change the name of the child dimstyle
      dimstyle_table.GetUnusedDimStyleName( child_dimstyle.m_dimstyle_name );

      // Add the new child dimstyle
      int style_index = dimstyle_table.AddDimStyle( child_dimstyle, false );
      if( style_index >= 0 )
      {
        // Modify the dimension to reflect the new child dimstyle style_index
        CRhinoAnnotationObject* new_object = annotation_object->Duplicate();
        if( 0 != new_object )
        {
          new_object->SetStyleIndex( style_index );
          new_object->UpdateText();
          context.m_doc.ReplaceObject( object_ref, new_object );
        }
      }
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
