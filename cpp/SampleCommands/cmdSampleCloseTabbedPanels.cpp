#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCloseTabbedPanels command
//

#pragma region SampleCloseTabbedPanels command

class CCommandSampleCloseTabbedPanels : public CRhinoCommand
{
public:
  CCommandSampleCloseTabbedPanels() = default;
  UUID CommandUUID() override
  {
    // {33207656-4A7B-436D-81FA-0A26EDEB79AE}
    static const GUID SampleCloseTabbedPanelsCommand_UUID =
    { 0x33207656, 0x4A7B, 0x436D, { 0x81, 0xFA, 0x0A, 0x26, 0xED, 0xEB, 0x79, 0xAE } };
    return SampleCloseTabbedPanelsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCloseTabbedPanels"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleCloseTabbedPanels object
static class CCommandSampleCloseTabbedPanels theSampleCloseTabbedPanelsCommand;

CRhinoCommand::result CCommandSampleCloseTabbedPanels::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoAppUiDockBarManager& mgr = RhinoApp().RhinoUiDockBarManager();

  ON_SimpleArray<const CRhinoUiPanelFactory*> factories;
  ::RhinoUiPanelFactories(factories);

  for (int fc = 0; fc < factories.Count(); fc++)
  {
    const auto* panel_factory = factories[fc];
    if (nullptr != panel_factory)
    {
      ON_SimpleArray<ON_UUID> dockbars;
      ::RhinoUiDockBarsForTab(context.m_doc, panel_factory->TabId(), dockbars);

      for (int db = 0; db < dockbars.Count(); db++)
      {
        const auto& dock_bar_id = dockbars[db];

        if (mgr.IsDockBarVisible(dock_bar_id))
        {
          mgr.ShowDockBar(dock_bar_id, false, false);
        }
      }
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCloseTabbedPanels command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
