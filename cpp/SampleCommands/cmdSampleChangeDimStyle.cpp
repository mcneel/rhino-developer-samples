#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleChangeDimStyle command
//

#pragma region SampleChangeDimStyle command

class CCommandSampleChangeDimStyle : public CRhinoCommand
{
public:
  CCommandSampleChangeDimStyle() = default;
  UUID CommandUUID() override
  {
    // {EE8D20C8-AEDD-4643-9A15-F223391F36DF}
    static const GUID SampleChangeDimStyleCommand_UUID =
    { 0xEE8D20C8, 0xAEDD, 0x4643, { 0x9A, 0x15, 0xF2, 0x23, 0x39, 0x1F, 0x36, 0xDF } };
    return SampleChangeDimStyleCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleChangeDimStyle"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleChangeDimStyle object
static class CCommandSampleChangeDimStyle theSampleChangeDimStyleCommand;

CRhinoCommand::result CCommandSampleChangeDimStyle::RunCommand(const CRhinoCommandContext& context)
{
  //int dimstyle_count = context.m_doc.m_dimstyle_table.DimStyleCount();

  const CRhinoDimStyle& ds = context.m_doc.m_dimstyle_table.CurrentDimStyle();

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select Objects");
  go.SetGeometryFilter(ON::object_type::annotation_object);
  CRhinoGet::result res = go.GetObjects(1, 0);
  if (res == CRhinoGet::object)
  {
    for (int i = 0; i < go.ObjectCount(); i++)
    {
      const CRhinoObjRef& objref = go.Object(i);
      const CRhinoAnnotation* ann_obj = CRhinoAnnotation::Cast(go.Object(i).Object());
      if (ann_obj)
      {
        const ON_Annotation* on_anno = ann_obj->Annotation();
        if (nullptr == on_anno)
          continue;

        switch (ann_obj->AnnotationType())
        {
          case ON::AnnotationType::Text:
          {
            ON_Text* new_text = ON_Text::Cast(on_anno->Duplicate());
            CRhinoText* rhtext = new CRhinoText(ann_obj->Attributes());
            if (nullptr != new_text && nullptr != rhtext)
            {
              new_text->SetDimensionStyleId(ds.Id());
              rhtext->SetTextObject(new_text);
              context.m_doc.ReplaceObject(objref, rhtext);
            }
          }
        }
      }
    }
  }
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleChangeDimStyle command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
