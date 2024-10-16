#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLine command
//

class CCommandSampleLine : public CRhinoCommand
{
public:
  CCommandSampleLine() = default;
  ~CCommandSampleLine() = default;
  UUID CommandUUID() override
  {
    // {F4226F9A-132-4D66-AAB1-EE483B80C221}
    static const GUID SampleLineCommand_UUID =
    { 0xF4226F9A, 0x132, 0x4D66, { 0xAA, 0xB1, 0xEE, 0x48, 0x3B, 0x80, 0xC2, 0x21 } };
    return SampleLineCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleLine"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleLine object
static class CCommandSampleLine theSampleLineCommand;

CRhinoCommand::result CCommandSampleLine::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CArgsRhinoGetLine args;
  args.EnableFromMidPointOption(TRUE);
  args.EnableShowVariations(TRUE);

  ON_Line line;
  CRhinoCommand::result rc = RhinoGetLine(args, line);
  if (rc == CRhinoCommand::success)
  {
    doc->AddCurveObject(line);
    doc->Redraw();
  }

  return rc;
}

//
// END SampleLine command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
