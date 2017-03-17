#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLockProjection command
//

#pragma region SampleLockProjection command

class CCommandSampleLockProjection : public CRhinoCommand
{
public:
  CCommandSampleLockProjection() {}
  ~CCommandSampleLockProjection() {}
  UUID CommandUUID()
  {
    // {ECC5468-EBAC-43F9-93D3-4387C93C3653}
    static const GUID SampleLockProjectionCommand_UUID =
    { 0xECC5468, 0xEBAC, 0x43F9, { 0x93, 0xD3, 0x43, 0x87, 0xC9, 0x3C, 0x36, 0x53 } };
    return SampleLockProjectionCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleLockProjection"; }
  const wchar_t* LocalCommandName() const { return L"SampleLockProjection"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleLockProjection object
static class CCommandSampleLockProjection theSampleLockProjectionCommand;

CRhinoCommand::result CCommandSampleLockProjection::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoView* pView = RhinoApp().ActiveView();
  if (0 == pView)
    return CRhinoCommand::failure;

  ON_3dmView view = pView->ActiveViewport().View();
  view.m_bLockedProjection = !view.m_bLockedProjection;
  pView->ActiveViewport().SetView(view);
  pView->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleLockProjection command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
