#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGrid command
//

#pragma region SampleGrid command

#define MAX_GRID_EXTENTS 10000.0

class CCommandSampleGrid : public CRhinoCommand
{
public:
  CCommandSampleGrid();
  UUID CommandUUID() override
  {
    // {6BDDF142-881C-42FE-9A88-205C653264FB}
    static const GUID SampleGridCommand_UUID =
    { 0x6BDDF142, 0x881C, 0x42FE, { 0x9A, 0x88, 0x20, 0x5C, 0x65, 0x32, 0x64, 0xFB } };
    return SampleGridCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleGrid"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleGrid object
static class CCommandSampleGrid theSampleGridCommand;

CCommandSampleGrid::CCommandSampleGrid()
  : CRhinoCommand(true)
{
}

CRhinoCommand::result CCommandSampleGrid::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoView* pActiveView = RhinoApp().ActiveView();
  if (nullptr == pActiveView)
    return CRhinoCommand::failure;

  ON_SimpleArray<unsigned int> view_list;
  CRhinoView::GetViewList(context.m_rhino_doc_sn, view_list);

  const UUID display_mode_id = pActiveView->ActiveViewport().View().m_display_mode_id;

  DisplayAttrsMgrListDesc* pDisplayMode = CRhinoDisplayAttrsMgr::FindDisplayAttrsDesc(display_mode_id);
  if (nullptr == pDisplayMode)
    return CRhinoCommand::failure;

  CDisplayPipelineAttributes* pAttrs = pDisplayMode->m_pAttrs;
  if (nullptr == pAttrs)
    return CRhinoCommand::failure;

  bool bApplyToAll = false;
  
  bool bShowConstructionGrid = pAttrs->m_bUseDocumentGrid ? pActiveView->ActiveViewport().View().m_bShowConstructionGrid : pAttrs->m_bDrawGrid;
  bool bShowConstructionAxes = pAttrs->m_bUseDocumentGrid ? pActiveView->ActiveViewport().View().m_bShowConstructionAxes : pAttrs->m_bDrawAxes;
  bool bShowWorldAxes = pAttrs->m_bUseDocumentGrid ? pActiveView->ActiveViewport().View().m_bShowWorldAxes : pAttrs->m_bDrawWorldAxes;

  const ON_3dmConstructionPlane& cplane = pActiveView->ActiveViewport().ConstructionPlane();

  double dGridSpacing = cplane.m_grid_spacing;
  double dSnapSpacing = cplane.m_snap_spacing;
  int nGridDivisions = cplane.m_grid_thick_frequency;
  double dGridDivisions = nGridDivisions;
  double dGridExtents = cplane.m_grid_line_count * cplane.m_grid_spacing;

  CRhinoGetOption go;
  go.SetCommandPrompt(L"Choose grid property");
  go.AcceptNothing(true);

  while (true)
  {
    go.ClearCommandOptions();
    
    const int iSnapSpace = go.AddCommandOptionNumber(RHCMDOPTNAME(L"SnapSpacing"), &dSnapSpacing, L"Grid snap spacing", false, 0.0);
    const int iMinorLine = go.AddCommandOptionNumber(RHCMDOPTNAME(L"MinorLineSpacing"), &dGridSpacing, L"Minor grid line spacing", false, 0.0);
    const int iMajorLine = go.AddCommandOptionNumber(RHCMDOPTNAME(L"MajorLineInterval"), &dGridDivisions, L"Major grid line interval", true, 0.0);
    const int iExtents = go.AddCommandOptionNumber(RHCMDOPTNAME(L"Extents"), &dGridExtents, L"Grid extents", false, 0.0, MAX_GRID_EXTENTS);
    const int iShowGrid = go.AddCommandOptionToggle(RHCMDOPTNAME(L"ShowGrid"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bShowConstructionGrid, &bShowConstructionGrid);
    const int iShowGAxes = go.AddCommandOptionToggle(RHCMDOPTNAME(L"ShowGridAxes"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bShowConstructionAxes, &bShowConstructionAxes);
    const int iShowWAxes = go.AddCommandOptionToggle(RHCMDOPTNAME(L"ShowWorldAxes"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), bShowWorldAxes, &bShowWorldAxes);
    const int iApplyToAll = go.AddCommandOptionToggle(RHCMDOPTNAME(L"ApplyTo"), RHCMDOPTVALUE(L"ActiveViewport"), RHCMDOPTVALUE(L"AllViewports"), bApplyToAll, &bApplyToAll);

    CRhinoGet::result res = go.GetOption();

    if (res == CRhinoGet::nothing)
      break;

    if (res != CRhinoGet::option)
      return CRhinoCommand::cancel;

    const CRhinoCommandOption* pOption = go.Option();
    if (nullptr == pOption)
      return CRhinoCommand::failure;

    for (int i = 0; i < view_list.Count(); i++)
    {
      CRhinoView* pView = nullptr;
      if (bApplyToAll)
        pView = CRhinoView::FromRuntimeSerialNumber(view_list[i]);
      else
      {
        pView = pActiveView;
        i = view_list.Count();
      }

      if (nullptr == pView)
        continue;

      if (CRhinoView::rhino_view_type != pView->RhinoViewType())
        continue;

      ON_3dmView view = pView->Viewport().View();
      ON_3dmConstructionPlane& cplane = view.m_cplane;

      bool bDoAll = false;
      if (iApplyToAll == pOption->m_option_index)
      {
        if (!bApplyToAll)
          continue;
        else
          bDoAll = true;
      }

      bool bNeedUpdateDisplayModes = false;

      if (bDoAll || iShowGrid == pOption->m_option_index)
      {
        if (pAttrs->m_bUseDocumentGrid)
        {
          view.m_bShowConstructionGrid = bShowConstructionGrid;
        }
        else
        {
          pAttrs->m_bDrawGrid = bShowConstructionGrid;
          bNeedUpdateDisplayModes = true;
        }
      }

      if (bDoAll || iShowGAxes == pOption->m_option_index)
      {
        if (pAttrs->m_bUseDocumentGrid)
        {
          view.m_bShowConstructionAxes = bShowConstructionAxes;
        }
        else
        {
          pAttrs->m_bDrawAxes = bShowConstructionAxes;
          bNeedUpdateDisplayModes = true;
        }
      }

      if (bDoAll || iShowWAxes == pOption->m_option_index)
      {
        if (pAttrs->m_bUseDocumentGrid)
        {
          view.m_bShowWorldAxes = bShowWorldAxes;
        }
        else
        {
          pAttrs->m_bDrawWorldAxes = bShowWorldAxes;
          bNeedUpdateDisplayModes = true;
        }
      }

      if (bNeedUpdateDisplayModes)
      {
        CRhinoDisplayAttrsMgr::SetIsModified(true);
        CRhinoDisplayAttrsMgr::SaveProfile(RhinoApp().ProfileContext());
        doc->Regen();
      }

      if (bDoAll || iExtents == pOption->m_option_index)
      {
        dGridExtents = min(dGridExtents, MAX_GRID_EXTENTS);
        if (dGridExtents > 0)
          cplane.m_grid_line_count = (int)(dGridExtents / cplane.m_grid_spacing);
      }

      if (bDoAll || iSnapSpace == pOption->m_option_index)
      {
        if (dSnapSpacing > 0.0)
          cplane.m_snap_spacing = dSnapSpacing;
        else
          continue;
      }

      if (bDoAll || iMinorLine == pOption->m_option_index)
      {
        if (dGridSpacing > 0.0)
        {
          cplane.m_grid_spacing = dGridSpacing;
          dGridExtents = cplane.m_grid_line_count * cplane.m_grid_spacing;
        }
        else
          continue;
      }

      if (bDoAll || iMajorLine == pOption->m_option_index)
      {
        cplane.m_grid_thick_frequency = (int)dGridDivisions;
      }

      pView->ActiveViewport().SetView(view);
      pView->Redraw();
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleGrid command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
