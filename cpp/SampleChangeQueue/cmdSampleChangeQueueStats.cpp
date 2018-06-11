#include "StdAfx.h"
#include "SampleChangeQueuePlugIn.h"
#include "SampleChangeQueue.h"

#pragma region Stats command

// Do NOT put the definition of class CCommandSampleChangeQueueStats in a header
// file. There is only ONE instance of a CCommandSampleChangeQueueStats class
// and that instance is the static theSampleChangeQueueStatsCommand that appears
// immediately below the class definition.

class CCommandSampleChangeQueueStats : public CRhinoCommand
{
public:
	// The one and only instance of CCommandSampleChangeQueueStats is created below.
	// No copy constructor or operator= is required.
	// Values of member variables persist for the duration of the application.

	// CCommandSampleChangeQueueStats::CCommandSampleChangeQueueStats()
	// is called exactly once when static theSampleChangeQueueStatsCommand is created.
	CCommandSampleChangeQueueStats() = default;

	// CCommandSampleChangeQueueStats::~CCommandSampleChangeQueueStats()
	// is called exactly once when static theSampleChangeQueueStatsCommand is destroyed.
	// The destructor should not make any calls to the Rhino SDK. 
	// If your command has persistent settings, then override 
	// CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
	~CCommandSampleChangeQueueStats() = default;

	// Returns a unique UUID for this command.
	// If you try to use an id that is already being used, then
	// your command will not work. Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID() override
	{
		// {FA3E367D-B058-4482-B3A4-72E0AA9E8545}
		static const GUID SampleChangeQueueCommandStats_UUID =
		{ 0xfa3e367d, 0xb058, 0x4482, { 0xb3, 0xa4, 0x72, 0xe0, 0xaa, 0x9e, 0x85, 0x45 } };
		return SampleChangeQueueCommandStats_UUID;
	}

	// Returns the English command name.
	// If you want to provide a localized command name, then override 
	// CRhinoCommand::LocalCommandName.
	const wchar_t* EnglishCommandName() override { return L"SampleChangeQueueStats"; }

	// Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleChangeQueueStats object
// Do NOT create any other instance of a CCommandSampleChangeQueueStats class.
static class CCommandSampleChangeQueueStats theSampleChangeQueueStatsCommand;

CRhinoCommand::result CCommandSampleChangeQueueStats::RunCommand(const CRhinoCommandContext& context)
{
	CSampleChangeQueuePlugIn& plugIn = SampleChangeQueuePlugIn();
	if (nullptr == plugIn.m_pSampleChangeQueue) {
		RhinoApp().Print(L"No SampleChangeQueue active, run command '_SampleChangeQueue'\n");
		return CRhinoCommand::nothing;
	}
	// clear out changequeue, but also apply the changes - this will call through all necessary
	// Apply* functions in SampleChangeQueue.
	// note that Stats is allowed to be called off the main thread,
	// but for the sake of simplicity we don't do that here.
	plugIn.m_pSampleChangeQueue->PrintStats();
  return CRhinoCommand::success;
}

#pragma endregion
