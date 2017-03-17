#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSwapHatch command
//

class CCommandSampleSwapHatch : public CRhinoCommand
{
public:
  CCommandSampleSwapHatch() {}
  ~CCommandSampleSwapHatch() {}
  UUID CommandUUID()
  {
    // {2C4D0F6F-C9B-46AF-8CF6-DF1E3506B25D}
    static const GUID SampleSwapHatchCommand_UUID =
    { 0x2C4D0F6F, 0xC9B, 0x46AF, { 0x8C, 0xF6, 0xDF, 0x1E, 0x35, 0x06, 0xB2, 0x5D } };
    return SampleSwapHatchCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSwapHatch"; }
  const wchar_t* LocalCommandName() { return L"SampleSwapHatch"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSwapHatch object
static class CCommandSampleSwapHatch theSampleSwapHatchCommand;

CRhinoCommand::result CCommandSampleSwapHatch::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select hatches to replace pattern");
  go.SetGeometryFilter(CRhinoGetObject::hatch_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Name of replacement hatch pattern");
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  ON_wString pattern_name = gs.String();
  pattern_name.TrimLeftAndRight();
  if (pattern_name.IsEmpty())
    return CRhinoCommand::nothing;

  const CRhinoHatchPattern* hatch_pattern = context.m_doc.m_hatchpattern_table.HatchPatternFromName(pattern_name, true);
  if (nullptr == hatch_pattern)
  {
    RhinoApp().Print(L"Specified hatch pattern not found in the document.\n");
    return CRhinoCommand::nothing;
  }

  int i, replaced = 0;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoHatch* hatch_obj = CRhinoHatch::Cast(go.Object(i).Object());
    if (0 == hatch_obj)
      continue;

    if (hatch_pattern->Index() == hatch_obj->PatternIndex())
      continue;

    const ON_Hatch* hatch = hatch_obj->Hatch();
    if (0 == hatch)
      continue;

    ON_Hatch* dup_hatch = hatch->DuplicateHatch();
    if (0 == dup_hatch)
      continue;

    dup_hatch->SetPatternIndex(hatch_pattern->Index());

    CRhinoHatch* dup_obj = hatch_obj->Duplicate();
    if (0 == dup_obj)
    {
      delete dup_hatch;
      continue;
    }

    dup_obj->SetHatch(dup_hatch);
    if (!context.m_doc.ReplaceObject(CRhinoObjRef(hatch_obj), dup_obj))
    {
      delete dup_obj;
      continue;
    }

    replaced++;
  }

  if (replaced > 0)
  {
    context.m_doc.Redraw();
    if (1 == replaced)
      RhinoApp().Print(L"1 hatch pattern replaced.\n");
    else
      RhinoApp().Print(L"%d hatch patterns replaced.\n", replaced);
  }
  else
    RhinoApp().Print(L"0 hatch patterns replaced.\n");

  return CRhinoCommand::success;
}

//
// END SampleSwapHatch command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
