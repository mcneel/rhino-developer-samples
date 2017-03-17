#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleQueryDiameterDimension command
//

class CRhGetDiameterDimensionObject : public CRhinoGetObject
{
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const;
};

bool CRhGetDiameterDimensionObject::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry*, ON_COMPONENT_INDEX) const
{
  bool rc = false;
  if (nullptr != object)
  {
    const CRhinoDimRadial* dim_object = CRhinoDimRadial::Cast(object);
    if (nullptr != dim_object)
    {
      const ON_DimRadial* dim = dim_object->GetDimension();
      if (nullptr != dim)
        rc = dim->Type() == ON::AnnotationType::Diameter;
    }
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
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleQueryDiameterDimension object
static class CCommandSampleQueryDiameterDimension theSampleQueryDiameterDimensionCommand;

CRhinoCommand::result CCommandSampleQueryDiameterDimension::RunCommand(const CRhinoCommandContext&)
{
  CRhGetDiameterDimensionObject go;
  go.SetCommandPrompt(L"Select diameter dimension");
  go.SetGeometryFilter(CRhinoGetObject::annotation_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef object_ref = go.Object(0);

  const CRhinoObject* object = object_ref.Object();
  if (0 == object)
    return CRhinoCommand::failure;

  const CRhinoDimRadial* dim_object = CRhinoDimRadial::Cast(object);
  if (nullptr == dim_object)
    return CRhinoCommand::failure;

  const ON_DimRadial* dim = dim_object->GetDimension();
  if (nullptr != dim || dim->Type() != ON::AnnotationType::Diameter)
    return CRhinoCommand::failure;

  ON_3dPoint center_pt, radius_pt, dimline_pt, knee_pt;
  if (!dim->Get3dPoints(&center_pt, &radius_pt, &dimline_pt, &knee_pt))
    return CRhinoCommand::failure;

  ON_wString center_str, radius_str, dimline_str, knee_str;
  RhinoFormatPoint(center_pt, center_str);
  RhinoFormatPoint(radius_pt, radius_str);
  RhinoFormatPoint(dimline_pt, dimline_str);
  RhinoFormatPoint(knee_pt, knee_str);

  RhinoApp().Print(L"Center point: %s\n", center_str);
  RhinoApp().Print(L"Radius point: %s\n", radius_str);
  RhinoApp().Print(L"Dimension line point: %s\n", dimline_str);
  RhinoApp().Print(L"Knee point: %s\n", knee_str);
  RhinoApp().Print(L"Dimension text: %s\n", dim_object->PlainText());

  return CRhinoCommand::success;
}

//
// END SampleQueryDiameterDimension command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
