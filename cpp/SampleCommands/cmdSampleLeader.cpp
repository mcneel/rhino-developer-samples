#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLeader command
//

class CCommandSampleLeader : public CRhinoCommand
{
public:
  CCommandSampleLeader() {}
  ~CCommandSampleLeader() {}
  UUID CommandUUID()
  {
    // {7D5E43E9-D737-49CB-BD44-39E1C6957B54}
    static const GUID SampleLeaderCommand_UUID =
    { 0x7D5E43E9, 0xD737, 0x49CB, { 0xBD, 0x44, 0x39, 0xE1, 0xC6, 0x95, 0x7B, 0x54 } };
    return SampleLeaderCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleLeader"; }
  const wchar_t* LocalCommandName() { return L"SampleLeader"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleLeader object
static class CCommandSampleLeader theSampleLeaderCommand;

CRhinoCommand::result CCommandSampleLeader::RunCommand(const CRhinoCommandContext& context)
{
  ON_3dPointArray points;
  points.Append(ON_3dPoint(1.0, 1.0, 0.0));
  points.Append(ON_3dPoint(5.0, 1.0, 0.0));
  points.Append(ON_3dPoint(5.0, 5.0, 0.0));
  points.Append(ON_3dPoint(9.0, 5.0, 0.0));

  const CRhinoLeader* dim_obj = context.m_doc.AddLeaderObject(
    L"Leader",
    ON_Plane::World_xy,
    points.Count(),
    points.Array()
  );

  if (nullptr != dim_obj)
    context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleLeader command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
