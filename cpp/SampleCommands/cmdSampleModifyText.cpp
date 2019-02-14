#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleModifyText command
//

static ON_wString RhinoPlainTextToRtf(const wchar_t* text)
{
  static const wchar_t* placeholder = L"E368C572-BF39-4EA3-B02B-F7D9C63D6580";

  ON_wString rc;
  if (nullptr == text || 0 == text[0])
    return rc;

  ON_wString str(text);
  str.Replace(L"\n", placeholder);
  str.Replace(L"\r\n", placeholder);
  str.Replace(L"\\", L"\\\\");
  str.Replace(L"{", L"\\{");
  str.Replace(L"}", L"\\}");
  str.Replace(placeholder, L"}\\par{ ");

  rc = rc + L"{\\rtf1{\\ltrch ";
  rc = rc + str;
  rc = rc + L"}";

  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CGetTextObject : public CRhinoGetObject
{
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const;
};

bool CGetTextObject::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const
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

class CCommandSampleModifyText : public CRhinoCommand
{
public:
  CCommandSampleModifyText() = default;
  UUID CommandUUID() override
  {
    // {6C5182A-DC41-42B1-8F7A-4ED49A0CF6F1}
    static const GUID SampleModifyTextCommand_UUID =
    { 0x6C5182A, 0xDC41, 0x42B1, { 0x8F, 0x7A, 0x4E, 0xD4, 0x9A, 0x0C, 0xF6, 0xF1 } };
    return SampleModifyTextCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleModifyText"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleModifyText object
static class CCommandSampleModifyText theSampleModifyTextCommand;

CRhinoCommand::result CCommandSampleModifyText::RunCommand(const CRhinoCommandContext& context)
{
  CGetTextObject go;
  go.SetCommandPrompt(L"Select text to modify");
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& object_ref = go.Object(0);
  const CRhinoText* text_object = CRhinoText::Cast(object_ref.Object());
  if (nullptr == text_object)
    return CRhinoCommand::failure;

  const ON_DimStyle& dimstyle = text_object->GetEffectiveDimensionStyle(&context.m_doc);
  const ON_Text* text = text_object->TextObject(&dimstyle);
  if (nullptr == text)
    return CRhinoCommand::failure;

  ON_wString old_string = text->PlainText();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"New text");
  gs.SetDefaultString(static_cast<const wchar_t*>(old_string));
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  ON_wString new_string = gs.String();
  new_string.TrimLeftAndRight();
  if (new_string.IsEmpty())
    return CRhinoCommand::nothing;

  if (0 == new_string.CompareOrdinal(old_string, false))
    return CRhinoCommand::nothing;

  ON_wString rtf_string = RhinoPlainTextToRtf(new_string);

  ON_Text* new_text = text->Duplicate();
  new_text->ReplaceTextString(static_cast<const wchar_t*>(rtf_string), &dimstyle);

  CRhinoText* new_text_object = new CRhinoText();
  new_text_object->SetTextObject(new_text);

  context.m_doc.ReplaceObject(object_ref, new_text_object);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleModifyText command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
