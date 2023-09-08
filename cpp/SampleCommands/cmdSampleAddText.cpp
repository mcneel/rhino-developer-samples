#include "stdafx.h"

CRhinoText* RhinoCreateText(
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
};

// The one and only CCommandSampleAddText object
static class CCommandSampleAddText theSampleAddTextCommand;

CRhinoCommand::result CCommandSampleAddText::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Text location");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint pt = gp.Point();
  ON_wString text = L"Hello Rhino!";

  CRhinoText* text_obj = RhinoCreateText(*doc, pt, text, L"Times New Roman", 1.0, 0, 0.0, ON::TextVerticalAlignment::Bottom, ON::TextHorizontalAlignment::Left);
  if (nullptr != text_obj)
  {
    doc->AddObject(text_obj);
    doc->Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleAddText command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAddTextObject command
//

#pragma region SampleAddTextObject command

class CCommandSampleAddTextObject : public CRhinoCommand
{
public:
  CCommandSampleAddTextObject() = default;
  UUID CommandUUID() override
  {
    // {87743F94-5612-4855-BD0C-59CD91148785}
    static const GUID SampleAddTextObjectCommand_UUID =
    { 0x87743F94, 0x5612, 0x4855, { 0xBD, 0x0C, 0x59, 0xCD, 0x91, 0x14, 0x87, 0x85 } };
    return SampleAddTextObjectCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleAddTextObject"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleAddTextObject object
static class CCommandSampleAddTextObject theSampleAddTextObjectCommand;

CRhinoCommand::result CCommandSampleAddTextObject::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Text object location");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  CRhinoView* view = gp.View();
  if (nullptr == view)
    return CRhinoCommand::failure;

  ON_3dPoint pt = gp.Point();
  ON_wString text = L"Hello Rhino!";

  CRhinoText* text_obj = RhinoCreateText(*doc, pt, text, L"Times New Roman", 1.0, 0, 0.0, ON::TextVerticalAlignment::Bottom, ON::TextHorizontalAlignment::Left);
  if (nullptr == text_obj)
    return CRhinoCommand::failure;

  const ON_Annotation* annotation = text_obj->Annotation();
  if (nullptr == annotation)
    return CRhinoCommand::failure;

  const ON_DimStyle* dim_style = &annotation->DimensionStyle(
    doc->m_dimstyle_table[doc->m_dimstyle_table.FindDimStyleFromId(annotation->DimensionStyleId(),
    true,
    true,
    doc->m_dimstyle_table.CurrentDimStyleIndex())]);

  ON_SimpleArray<ON_Object*> output;
  bool rc = RhinoCreateTextObjectGeometry(
    annotation,
    &view->Viewport().VP(),
    dim_style,
    1.0,
    true,
    doc->AbsoluteTolerance(),
    1.0,
    ON::object_type::extrusion_object,
    1.0,
    0.0,
    output
  );
  if (!rc)
    return CRhinoCommand::failure;

  for (int i = 0; i < output.Count(); i++)
  {
    if (nullptr != output[i])
    {
      CRhinoExtrusionObject* extrusion_obj = new CRhinoExtrusionObject();
      extrusion_obj->SetExtrusion(ON_Extrusion::Cast(output[i]));
      doc->AddObject(extrusion_obj);
    }
  }

  doc->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleAddTextObject command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
