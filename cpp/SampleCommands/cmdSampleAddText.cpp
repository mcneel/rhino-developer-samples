#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAddText command
//

#pragma region SampleAddText command

class CCommandSampleAddText : public CRhinoCommand
{
public:
  CCommandSampleAddText() = default;
  UUID CommandUUID() override
  {
    // {4C5D737C-7BB3-4152-A866-D062F7518ABA}
    static const GUID SampleAddTextCommand_UUID =
    { 0x4C5D737C, 0x7BB3, 0x4152,{ 0xA8, 0x66, 0xD0, 0x62, 0xF7, 0x51, 0x8A, 0xBA } };
    return SampleAddTextCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleAddText"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  CRhinoText* CreateText(
    CRhinoDoc& doc,
    const ON_3dPoint& pt,
    const wchar_t* text,
    const wchar_t* font_name,
    double height,
    int style,
    double rotate,
    ON::TextVerticalAlignment valign,
    ON::TextHorizontalAlignment halign
  );

};

// The one and only CCommandSampleAddText object
static class CCommandSampleAddText theSampleAddTextCommand;

CRhinoCommand::result CCommandSampleAddText::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Text location");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint pt = gp.Point();
  ON_wString text = L"Hello Rhino!";

  CRhinoText* text_obj = CreateText(context.m_doc, pt, text, L"Times New Roman", 1.0, 0, 0.0, ON::TextVerticalAlignment::Bottom, ON::TextHorizontalAlignment::Left);
  if (nullptr != text_obj)
  {
    context.m_doc.AddObject(text_obj);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

CRhinoText* CCommandSampleAddText::CreateText(
  CRhinoDoc& doc,
  const ON_3dPoint& pt,
  const wchar_t* text,
  const wchar_t* font_name,
  double height,
  int style,
  double rotate,
  ON::TextVerticalAlignment valign,
  ON::TextHorizontalAlignment halign
)
{
  CRhinoText* rc = nullptr;

  if (!pt.IsValid() || nullptr == text || 0 == text[0])
    return rc;

  ON_wString face_name(font_name);
  if (face_name.IsEmpty())
    face_name = L"Arial";

  style = RHINO_CLAMP(style, 0, 3);
  const bool bBold = (0 != (style & 1));
  const bool bItalic = (0 != (style & 2));

  // Get a font managed by the application from the font characteristics.
  const ON_Font* font = ON_Font::GetManagedFont(face_name, bBold, bItalic);
  if (nullptr == font)
    return rc;

  height = fabs(height);
  
  ON_Plane plane = RhinoActiveCPlane();
  plane.Rotate(rotate, plane.zaxis);

  // Get the current dimension style.
  const ON_DimStyle& parent_dimstyle = doc.m_dimstyle_table.CurrentDimStyle();

  // Create a new dimension style from properties.
  ON_DimStyle dim_style = ON_DimStyle::CreateFromProperties(
    parent_dimstyle,
    ON::AnnotationType::Text,
    font,
    ON_UNSET_VALUE,
    height,
    doc.UnitSystem(),
    valign,
    halign
  );

  // Create a new text object
  rc = doc.CreateTextObject(text, plane, pt, &dim_style);

  return rc;
}


#pragma endregion

//
// END SampleAddText command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
