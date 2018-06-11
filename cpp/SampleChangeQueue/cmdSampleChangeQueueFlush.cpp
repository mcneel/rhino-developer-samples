#include "StdAfx.h"
#include "SampleChangeQueuePlugIn.h"
#include "SampleChangeQueue.h"

#pragma region Flush command

// Do NOT put the definition of class CCommandSampleChangeQueueFlush in a header
// file. There is only ONE instance of a CCommandSampleChangeQueueFlush class
// and that instance is the static theSampleChangeQueueFlushCommand that appears
// immediately below the class definition.

class CCommandSampleChangeQueueFlush : public CRhinoCommand
{
public:
	// The one and only instance of CCommandSampleChangeQueueFlush is created below.
	// No copy constructor or operator= is required.
	// Values of member variables persist for the duration of the application.

	// CCommandSampleChangeQueueFlush::CCommandSampleChangeQueueFlush()
	// is called exactly once when static theSampleChangeQueueFlushCommand is created.
	CCommandSampleChangeQueueFlush() = default;

	// CCommandSampleChangeQueueFlush::~CCommandSampleChangeQueueFlush()
	// is called exactly once when static theSampleChangeQueueFlushCommand is destroyed.
	// The destructor should not make any calls to the Rhino SDK. 
	// If your command has persistent settings, then override 
	// CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
	~CCommandSampleChangeQueueFlush() = default;

	// Returns a unique UUID for this command.
	// If you try to use an id that is already being used, then
	// your command will not work. Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID() override
	{
		// {EC270CBF-4F47-43DD-B2FA-8AD8E98AD011}
		static const GUID SampleChangeQueueCommandFlush_UUID =
		{ 0xec270cbf, 0x4f47, 0x43dd, { 0xb2, 0xfa, 0x8a, 0xd8, 0xe9, 0x8a, 0xd0, 0x11 } };
		return SampleChangeQueueCommandFlush_UUID;
	}

	// Returns the English command name.
	// If you want to provide a localized command name, then override 
	// CRhinoCommand::LocalCommandName.
	const wchar_t* EnglishCommandName() override { return L"SampleChangeQueueFlush"; }

	// Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleChangeQueueFlush object
// Do NOT create any other instance of a CCommandSampleChangeQueueFlush class.
static class CCommandSampleChangeQueueFlush theSampleChangeQueueFlushCommand;

CRhinoCommand::result CCommandSampleChangeQueueFlush::RunCommand(const CRhinoCommandContext& context)
{
	CSampleChangeQueuePlugIn& plugIn = SampleChangeQueuePlugIn();
	if (nullptr == plugIn.m_pSampleChangeQueue) {
		RhinoApp().Print(L"No SampleChangeQueue active, run command '_SampleChangeQueue'\n");
		return CRhinoCommand::nothing;
	}
	// clear out changequeue, but also apply the changes - this will call through all necessary
	// Apply* functions in SampleChangeQueue.
	// note that Flush is allowed to be called off the main thread,
	// but for the sake of simplicity we don't do that here.
	if(plugIn.m_pSampleChangeQueue->HandleFlush())
		return CRhinoCommand::success;
	else {
		RhinoApp().Print(L"No changes were signalled, doing nothing\n");
		return CRhinoCommand::nothing;
	}
}

#pragma endregion
