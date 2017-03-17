#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleIdleProcessor command
//

#pragma region CSampleIdleEventWatcher class

class CSampleIdleEventWatcher : public CRhinoIsIdle
{
public:
  CSampleIdleEventWatcher();
  void Notify(const class CRhinoIsIdle::CParameters& params);

private:
  __time64_t m_time0;
};

// The one and only CSampleIdleEventWatcher object
static class CSampleIdleEventWatcher theSampleIdleEventWatcher;

CSampleIdleEventWatcher::CSampleIdleEventWatcher()
  : CRhinoIsIdle(RhinoApp().Rhino5_UUID())
  , m_time0(0)
{
}

void CSampleIdleEventWatcher::Notify(const class CRhinoIsIdle::CParameters& params)
{
  const __time64_t time_interval = 5;
  __time64_t time1 = _time64(0);
  if (0 == m_time0 || time1 > m_time0 + time_interval)
  {
    RhinoApp().Print(L">> CSampleIdleEventWatcher: count =  %d\n", params.m_count),
      m_time0 = time1;
  }
}

#pragma endregion


#pragma region SampleIdleProcessor command

class CCommandSampleIdleProcessor : public CRhinoCommand
{
public:
  CCommandSampleIdleProcessor() {}
  ~CCommandSampleIdleProcessor() {}
  UUID CommandUUID()
  {
    // {C2FF9FE9-51B6-4CA2-8509-CE6CEF256940}
    static const GUID SampleIdleProcessorCommand_UUID =
    { 0xC2FF9FE9, 0x51B6, 0x4CA2, { 0x85, 0x09, 0xCE, 0x6C, 0xEF, 0x25, 0x69, 0x40 } };
    return SampleIdleProcessorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleIdleProcessor"; }
  const wchar_t* LocalCommandName() const { return L"SampleIdleProcessor"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleIdleProcessor object
static class CCommandSampleIdleProcessor theSampleIdleProcessorCommand;

CRhinoCommand::result CCommandSampleIdleProcessor::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  if (!theSampleIdleEventWatcher.IsRegistered())
    theSampleIdleEventWatcher.Register();
  else
    theSampleIdleEventWatcher.Enable(theSampleIdleEventWatcher.IsEnabled() ? false : true);

  RhinoApp().Print(
    L"%s %s\n",
    EnglishCommandName(),
    theSampleIdleEventWatcher.IsEnabled() ? L"enabled" : L"disabled"
  );

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleIdleProcessor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
