#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleViewLock command
//

#pragma region SampleViewLock command

class CCommandSampleViewLock : public CRhinoCommand
{
public:
  CCommandSampleViewLock() {}
  ~CCommandSampleViewLock() {}
  UUID CommandUUID()
  {
    // {5D0E88BC-558A-4F86-B220-BBBCD9CC531D}
    static const GUID SampleViewLockCommand_UUID =
    { 0x5D0E88BC, 0x558A, 0x4F86, { 0xB2, 0x20, 0xBB, 0xBC, 0xD9, 0xCC, 0x53, 0x1D } };
    return SampleViewLockCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleViewLock"; }
  const wchar_t* LocalCommandName() { return L"SampleViewLock"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleViewLock object
static class CCommandSampleViewLock theSampleViewLockCommand;

CRhinoCommand::result CCommandSampleViewLock::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoView* view = RhinoApp().ActiveView();
  if (0 == view)
    return CRhinoCommand::failure;

  if (false == view->ActiveViewport().View().m_bLockedProjection)
  {
    ON_3dmView v = view->ActiveViewport().View();
    v.m_bLockedProjection = true;
    view->ActiveViewport().SetView(v);
    view->Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleViewLock command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleViewUnlock command
//

#pragma region SampleViewUnlock command

class CCommandSampleViewUnlock : public CRhinoCommand
{
public:
  CCommandSampleViewUnlock() {}
  ~CCommandSampleViewUnlock() {}
  UUID CommandUUID()
  {
    // {DC9D98BC-7CE2-47D7-846F-E39595D787D2}
    static const GUID SampleViewUnlockCommand_UUID =
    { 0xDC9D98BC, 0x7CE2, 0x47D7, { 0x84, 0x6F, 0xE3, 0x95, 0x95, 0xD7, 0x87, 0xD2 } };
    return SampleViewUnlockCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleViewUnlock"; }
  const wchar_t* LocalCommandName() { return L"SampleViewUnlock"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleViewUnlock object
static class CCommandSampleViewUnlock theSampleViewUnlockCommand;

CRhinoCommand::result CCommandSampleViewUnlock::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoView* view = RhinoApp().ActiveView();
  if (0 == view)
    return CRhinoCommand::failure;

  if (true == view->ActiveViewport().View().m_bLockedProjection)
  {
    ON_3dmView v = view->ActiveViewport().View();
    v.m_bLockedProjection = false;
    view->ActiveViewport().SetView(v);
    view->Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleViewUnlock command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
