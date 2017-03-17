#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleQueryDiameterDimension command
//

class CRhGetDiameterDimensionObject : public CRhinoGetObject
{
  bool CustomGeometryFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
    ) const;
};

bool CRhGetDiameterDimensionObject::CustomGeometryFilter(
    const CRhinoObject* object,
    const ON_Geometry* geometry,
    ON_COMPONENT_INDEX component_index
    ) const
{
  bool rc = false;
  if( object )
  {
    const CRhinoAnnotationObject* annotation_object = CRhinoAnnotationObject::Cast( object );
    if( annotation_object && annotation_object->Type() == ON::dtDimDiameter )
      rc = true;
  }
  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleQueryDiameterDimension : public CRhinoCommand
{
public:
	CCommandSampleQueryDiameterDimension() {}
	~CCommandSampleQueryDiameterDimension() {}
	UUID CommandUUID()
	{
		// {B762C061-2A2-4D51-AB2C-4228EC4B691F}
		static const GUID SampleQueryDiameterDimensionCommand_UUID =
		{ 0xB762C061, 0x2A2, 0x4D51, { 0xAB, 0x2C, 0x42, 0x28, 0xEC, 0x4B, 0x69, 0x1F } };
		return SampleQueryDiameterDimensionCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"SampleQueryDiameterDimension"; }
	const wchar_t* LocalCommandName() { return L"SampleQueryDiameterDimension"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleQueryDiameterDimension object
static class CCommandSampleQueryDiameterDimension theSampleQueryDiameterDimensionCommand;

CRhinoCommand::result CCommandSampleQueryDiameterDimension::RunCommand( const CRhinoCommandContext& context )
{
  CRhGetDiameterDimensionObject go;
  go.SetCommandPrompt( L"Select diameter dimension" );
  go.SetGeometryFilter( CRhinoGetObject::annotation_object );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  const CRhinoObjRef object_ref = go.Object(0);

  // Get the Rhino object
  const CRhinoObject* object = object_ref.Object();
  if( 0 == object )
    return CRhinoCommand::failure;

  // The Rhino object class for a diameter dimension is a CRhinoAnnotationObject
  const CRhinoAnnotationObject* annotation_object = CRhinoAnnotationObject::Cast( object );
  if( 0 == annotation_object || annotation_object->Type() != ON::dtDimDiameter )
    return CRhinoCommand::failure;

  // Get the Rhino object's geometry. For CRhinoAnnotationObject, this is a ON_Annotation2 object
  const ON_Annotation2* annotation = annotation_object->Annotation(); 
  if( 0 == annotation )
    return CRhinoCommand::failure;

  // The geometry for a diameter dimension is a ON_RadialDimension2 object,
  // which is derived from ON_Annotation2.
  const ON_RadialDimension2* radial_dim = ON_RadialDimension2::Cast( annotation );
  if( 0 == radial_dim )
    return CRhinoCommand::failure;

  // Report some parameters
  ON_wString point0_str, point1_str;
  RhinoFormatPoint( radial_dim->Dim3dPoint(0), point0_str );
  RhinoFormatPoint( radial_dim->Dim3dPoint(1), point1_str );

  RhinoApp().Print( L"Center point: %s\n", point0_str );
  RhinoApp().Print( L"Arrowhead point: %s\n", point1_str );
  RhinoApp().Print( L"Dimension text: %s\n", annotation_object->TextString() );

  return CRhinoCommand::success;
}

//
// END SampleQueryDiameterDimension command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
