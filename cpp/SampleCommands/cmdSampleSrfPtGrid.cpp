#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSrfPtGrid command
//

#pragma region SampleSrfPtGrid command

class CCommandSampleSrfPtGrid : public CRhinoCommand
{
public:
  CCommandSampleSrfPtGrid() = default;
  ~CCommandSampleSrfPtGrid() = default;
  UUID CommandUUID() override
  {
    // {540E45AA-CC50-4724-B78C-94860BBE082A}
    static const GUID SampleSrfPtGridCommand_UUID =
    { 0x540E45AA, 0xCC50, 0x4724, { 0xB7, 0x8C, 0x94, 0x86, 0x0B, 0xBE, 0x08, 0x2A } };
    return SampleSrfPtGridCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSrfPtGrid"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleSrfPtGrid object
static class CCommandSampleSrfPtGrid theSampleSrfPtGridCommand;

CRhinoCommand::result CCommandSampleSrfPtGrid::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  int point_count[2] = { 20, 20 };
  int degree[2] = { 3, 3 };
  BOOL is_closed[2] = { false, false };

  ON_3dPointArray point_array(point_count[0] * point_count[1]);
  for (int u = 0; u < point_count[0]; u++)
  {
    for (int v = 0; v < point_count[1]; v++)
    {
      // TODO: add points here
      ON_3dPoint point((double)u, (double)v, sin((double)u) + sin((double)v));
      point_array.Append(point);
    }
  }

  ON_NurbsSurface nurb;
  if (&nurb == RhinoSrfPtGrid(point_count, degree, is_closed, point_array, &nurb))
  {
    doc->AddSurfaceObject(nurb);
    doc->Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSrfPtGrid command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
