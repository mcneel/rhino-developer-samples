#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleScaleTextHeight command
//

#pragma region SampleScaleTextHeight command

class CGetText : public CRhinoGetObject
{
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const;
};

bool CGetText::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const
{
  UNREFERENCED_PARAMETER(geometry);
  UNREFERENCED_PARAMETER(component_index);
  bool rc = false;
  if (object)
  {
    const CRhinoText* text_object = CRhinoText::Cast(object);
    rc = (nullptr == text_object) ? false : true;
  }
  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleScaleTextHeight : public CRhinoCommand
{
public:
  CCommandSampleScaleTextHeight() = default;
  UUID CommandUUID() override
  {
    // {9AC0101A-4870-4330-8677-B5A64C229C56}
    static const GUID SampleScaleTextHeightCommand_UUID =
    { 0x9AC0101A, 0x4870, 0x4330, { 0x86, 0x77, 0xB5, 0xA6, 0x4C, 0x22, 0x9C, 0x56 } };
    return SampleScaleTextHeightCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleScaleTextHeight"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleScaleTextHeight object
static class CCommandSampleScaleTextHeight theSampleScaleTextHeightCommand;

CRhinoCommand::result CCommandSampleScaleTextHeight::RunCommand(const CRhinoCommandContext& context)
{
  CGetText go;
  go.SetCommandPrompt(L"Select text to scale height");
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Scale factor to apply to text height");
  gn.SetDefaultNumber(1.0);
  gn.SetLowerLimit(0.0, TRUE);
  gn.AcceptNothing();
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  double scale = gn.Number();
  if (1.0 == scale)
    return CRhinoCommand::nothing;
  
  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& object_ref = go.Object(i);
    const CRhinoText* text_object = CRhinoText::Cast(object_ref.Object());
    if (nullptr == text_object)
      return CRhinoCommand::failure;

    const ON_DimStyle& dimstyle = text_object->GetEffectiveDimensionStyle(&context.m_doc);
    const ON_Text* text = text_object->TextObject(&dimstyle);
    if (nullptr == text)
      return CRhinoCommand::failure;

    ON_Text* new_text = text->Duplicate();

    ON_DimStyle* overrides = new ON_DimStyle(dimstyle);
    overrides->SetFieldOverride(ON_DimStyle::field::TextHeight, true);
    overrides->SetTextHeight(dimstyle.TextHeight() * scale);

    new_text->SetOverrideDimensionStyle(overrides);

    CRhinoText* new_text_object = new CRhinoText();
    new_text_object->SetTextObject(new_text);

    context.m_doc.ReplaceObject(object_ref, new_text_object);
  }
  
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleScaleTextHeight command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
