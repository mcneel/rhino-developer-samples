/////////////////////////////////////////////////////////////////////////////
// cmdSpaceGizmo.cpp : command file
//

#include "StdAfx.h"
#include "SpaceGizmoPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SpaceGizmo command
//

#pragma region SpaceGizmo command

class CCommandSpaceGizmo : public CRhinoScriptCommand
{
public:
  CCommandSpaceGizmo() {}
  ~CCommandSpaceGizmo() {}
  UUID CommandUUID()
  {
    // {AD4939EC-AFC4-4226-BD4E-7BF596D6220C}
    static const GUID SpaceGizmoCommand_UUID =
    { 0xAD4939EC, 0xAFC4, 0x4226, { 0xBD, 0x4E, 0x7B, 0xF5, 0x96, 0xD6, 0x22, 0x0C } };
    return SpaceGizmoCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SpaceGizmo"; }
  const wchar_t* LocalCommandName() const { return L"SpaceGizmo"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSpaceGizmo object.  
// Do NOT create any other instance of a CCommandSpaceGizmo class.
static class CCommandSpaceGizmo theSpaceGizmoCommand;

CRhinoCommand::result CCommandSpaceGizmo::RunCommand( const CRhinoCommandContext& context )
{
  // This will bring up Rhino's Options dialog with our page selected
  RhinoApp().RunScript( L"! _OptionsPage _SpaceGizmo", 0 );
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SpaceGizmo command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
