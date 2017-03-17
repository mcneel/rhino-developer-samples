#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleHatch command
//

class CCommandSampleHatch : public CRhinoCommand
{
public:
  CCommandSampleHatch() {}
  ~CCommandSampleHatch() {}
  UUID CommandUUID()
  {
    // {7825BF2E-8579-4344-B3C3-7B2A8F099956}
    static const GUID SampleHatchCommand_UUID =
    { 0x7825BF2E, 0x8579, 0x4344, { 0xB3, 0xC3, 0x7B, 0x2A, 0x8F, 0x09, 0x99, 0x56 } };
    return SampleHatchCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleHatch"; }
  const wchar_t* LocalCommandName() { return L"SampleHatch"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleHatch object
static class CCommandSampleHatch theSampleHatchCommand;

CRhinoCommand::result CCommandSampleHatch::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select closed planar curve");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::closed_curve);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* crv = go.Object(0).Curve();
  if (0 == crv || !crv->IsClosed() || !crv->IsPlanar())
    return CRhinoCommand::failure;

  CRhinoHatchPatternTable& table = context.m_doc.m_hatchpattern_table;

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Hatch pattern");
  gs.SetDefaultString(table.CurrentHatchPattern().Name());
  gs.GetString();
  if (gs.CommandResult() != CRhinoCommand::success)
    return gs.CommandResult();

  ON_wString name = gs.String();
  name.TrimLeftAndRight();
  if (name.IsEmpty())
    return CRhinoCommand::nothing;

  const CRhinoHatchPattern* hatch_pattern = table.HatchPatternFromName(name, true);
  if (nullptr == hatch_pattern)
  {
    RhinoApp().Print(L"Hatch pattern does not exist.\n");
    return CRhinoCommand::nothing;
  }

  CArgsRhinoHatch args;
  args.m_loops.Append(crv);
  args.SetPatternIndex(hatch_pattern->Index());
  args.SetPatternScale(1.0); // default
  args.SetPatternRotation(0.0); // default

  ON_SimpleArray<ON_Hatch*> hatches;
  bool rc = RhinoCreateHatches(args, context.m_doc.AbsoluteTolerance(), hatches);
  if (rc && hatches.Count())
  {
    int i, num_added = 0;
    for (i = 0; i < hatches.Count(); i++)
    {
      ON_Hatch* hatch = hatches[0];
      if (hatch)
      {
        CRhinoHatch* hatch_obj = new CRhinoHatch();
        if (hatch_obj)
        {
          hatch_obj->SetHatch(hatch);
          if (context.m_doc.AddObject(hatch_obj))
            num_added++;
          else
            delete hatch_obj;
        }
        else
          delete hatch;
      }
    }

    if (num_added)
      context.m_doc.Redraw();
  }

  return rc ? CRhinoCommand::success : CRhinoCommand::failure;
}

//
// END SampleHatch command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
