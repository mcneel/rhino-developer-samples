#include "stdafx.h"
#include "SampleEventWatcherPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleIdleWatcher command
//

#pragma region SampleIdleWatcher command

class CSampleIdleWatcherIsIdleEventWatcher : public CRhinoIsIdle
{
public:
  CSampleIdleWatcherIsIdleEventWatcher();
  void Notify(const class CRhinoIsIdle::CParameters& params);

private:
  __time64_t m_time0;
};

// The one and only CSampleIdleWatcherIsIdleEventWatcher object
static class CSampleIdleWatcherIsIdleEventWatcher theSampleIdleWatcherIsIdleEventWatcher;

CSampleIdleWatcherIsIdleEventWatcher::CSampleIdleWatcherIsIdleEventWatcher()
  : CRhinoIsIdle(SampleEventWatcherPlugIn().PlugInID())
  , m_time0(0)
{
}

void CSampleIdleWatcherIsIdleEventWatcher::Notify(const class CRhinoIsIdle::CParameters& params)
{
  const __time64_t time_interval = 5;
  __time64_t time1 = _time64(0);
  if (0 == m_time0 || time1 > m_time0 + time_interval)
  {
    RhinoApp().Print(L"Sample Idle Watcher: count =  %d\n", params.m_count);
    m_time0 = time1;
  }
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleIdleWatcher : public CRhinoScriptCommand
{
public:
  CCommandSampleIdleWatcher() {}
  ~CCommandSampleIdleWatcher() {}
  UUID CommandUUID()
  {
    // {9CBA28EF-71B-4B3E-8851-446BDC72DF21}
    static const GUID SampleIdleWatcherCommand_UUID =
    { 0x9CBA28EF, 0x71B, 0x4B3E, { 0x88, 0x51, 0x44, 0x6B, 0xDC, 0x72, 0xDF, 0x21 } };
    return SampleIdleWatcherCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleIdleWatcher"; }
  const wchar_t* LocalCommandName() const { return L"SampleIdleWatcher"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleIdleWatcher object
static class CCommandSampleIdleWatcher theSampleIdleWatcherCommand;

CRhinoCommand::result CCommandSampleIdleWatcher::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  if (!theSampleIdleWatcherIsIdleEventWatcher.IsRegistered())
    theSampleIdleWatcherIsIdleEventWatcher.Register();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleIdleWatcher command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
