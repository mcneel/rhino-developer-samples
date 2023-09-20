#include "StdAfx.h"
#include "SampleDrawAnnotationDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawAnnotation command
//

class CCommandSampleDrawAnnotation : public CRhinoCommand
{
public:
  CCommandSampleDrawAnnotation() = default;
  ~CCommandSampleDrawAnnotation() = default;
  UUID CommandUUID() override
  {
    // {2B5505DF-E8EC-4912-B1A1-5422EAE32C72}
    static const GUID SampleDrawAnnotationCommand_UUID =
    { 0x2B5505DF, 0xE8EC, 0x4912, { 0xB1, 0xA1, 0x54, 0x22, 0xEA, 0xE3, 0x2C, 0x72 } };
    return SampleDrawAnnotationCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDrawAnnotation"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleDrawAnnotation object
static class CCommandSampleDrawAnnotation theSampleDrawAnnotationCommand;

CRhinoCommand::result CCommandSampleDrawAnnotation::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CWnd* pWnd = CWnd::FromHandle(RhinoApp().MainWnd());
  CSampleDrawAnnotationDialog dialog(pWnd, *doc);
  dialog.DoModal();
  
  return CRhinoCommand::success;
}

//
// END SampleDrawAnnotation command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
