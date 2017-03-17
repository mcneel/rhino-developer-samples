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
  CCommandSampleSrfPtGrid() {}
  ~CCommandSampleSrfPtGrid() {}
  UUID CommandUUID()
  {
    // {540E45AA-CC50-4724-B78C-94860BBE082A}
    static const GUID SampleSrfPtGridCommand_UUID =
    { 0x540E45AA, 0xCC50, 0x4724, { 0xB7, 0x8C, 0x94, 0x86, 0x0B, 0xBE, 0x08, 0x2A } };
    return SampleSrfPtGridCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSrfPtGrid"; }
  const wchar_t* LocalCommandName() const { return L"SampleSrfPtGrid"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSrfPtGrid object
static class CCommandSampleSrfPtGrid theSampleSrfPtGridCommand;

CRhinoCommand::result CCommandSampleSrfPtGrid::RunCommand(const CRhinoCommandContext& context)
{
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
    context.m_doc.AddSurfaceObject(nurb);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSrfPtGrid command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
