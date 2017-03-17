#include "stdafx.h"
#include "SampleBlockViewDialog.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleBlockView command
//

class CCommandSampleBlockView : public CRhinoCommand
{
public:
  CCommandSampleBlockView() {}
  ~CCommandSampleBlockView() {}
  UUID CommandUUID()
  {
    // {8425DD03-A57D-4197-A4D7-F6CC07E9B0F2}
    static const GUID SampleBlockViewCommand_UUID =
    { 0x8425DD03, 0xA57D, 0x4197, { 0xA4, 0xD7, 0xF6, 0xCC, 0x07, 0xE9, 0xB0, 0xF2 } };
    return SampleBlockViewCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleBlockView"; }
  const wchar_t* LocalCommandName() { return L"SampleBlockView"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleBlockView object
static class CCommandSampleBlockView theSampleBlockViewCommand;

CRhinoCommand::result CCommandSampleBlockView::RunCommand(const CRhinoCommandContext& context)
{
  const CRhinoInstanceDefinitionTable& idef_table = context.m_doc.m_instance_definition_table;

  bool bFound = false;
  int i;
  for (i = 0; i < idef_table.InstanceDefinitionCount(); i++)
  {
    const CRhinoInstanceDefinition* idef = idef_table[i];
    if (idef && !idef->IsDeleted())
    {
      bFound = true;
      break;
    }
  }

  if (!bFound)
  {
    ON_wString msg(L"No block definitions to view.\n");
    if (context.IsInteractive())
      RhinoMessageBox(msg, EnglishCommandName(), MB_OK);
    else
      RhinoApp().Print(msg);
    return nothing;
  }

  CSampleBlockViewDialog dialog(CWnd::FromHandle(RhinoApp().MainWnd()), context.m_doc);
  dialog.DoModal();

  return CRhinoCommand::success;
}

//
// END SampleBlockView command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
