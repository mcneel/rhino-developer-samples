#include "StdAfx.h"
#include "SampleChangeQueuePlugIn.h"
#include "SampleChangeQueue.h"

#pragma region Stop command

// Do NOT put the definition of class CCommandSampleChangeQueueStop in a header
// file. There is only ONE instance of a CCommandSampleChangeQueueStop class
// and that instance is the static theSampleChangeQueueStopCommand that appears
// immediately below the class definition.

class CCommandSampleChangeQueueStop : public CRhinoCommand
{
public:
	// The one and only instance of CCommandSampleChangeQueueStop is created below.
	// No copy constructor or operator= is required.
	// Values of member variables persist for the duration of the application.

	// CCommandSampleChangeQueueStop::CCommandSampleChangeQueueStop()
	// is called exactly once when static theSampleChangeQueueStopCommand is created.
	CCommandSampleChangeQueueStop() = default;

	// CCommandSampleChangeQueueStop::~CCommandSampleChangeQueueStop()
	// is called exactly once when static theSampleChangeQueueStopCommand is destroyed.
	// The destructor should not make any calls to the Rhino SDK. 
	// If your command has persistent settings, then override 
	// CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
	~CCommandSampleChangeQueueStop() = default;

	// Returns a unique UUID for this command.
	// If you try to use an id that is already being used, then
	// your command will not work. Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID() override
	{
		// {77E07D56-F745-4166-B549-47F72C910894}
		static const GUID SampleChangeQueueCommandStop_UUID =
		{ 0x77e07d56, 0xf745, 0x4166, { 0xb5, 0x49, 0x47, 0xf7, 0x2c, 0x91, 0x8, 0x94 } };
		return SampleChangeQueueCommandStop_UUID;
	}

	// Returns the English command name.
	// If you want to provide a localized command name, then override 
	// CRhinoCommand::LocalCommandName.
	const wchar_t* EnglishCommandName() override { return L"SampleChangeQueueStop"; }

	// Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleChangeQueueStop object
// Do NOT create any other instance of a CCommandSampleChangeQueueStop class.
static class CCommandSampleChangeQueueStop theSampleChangeQueueStopCommand;

CRhinoCommand::result CCommandSampleChangeQueueStop::RunCommand(const CRhinoCommandContext& context)
{
	CSampleChangeQueuePlugIn& plugIn = SampleChangeQueuePlugIn();
	if (nullptr == plugIn.m_pSampleChangeQueue) {
		RhinoApp().Print(L"No SampleChangeQueue active, can't stop it. Run command '_SampleChangeQueue'\n");
		return CRhinoCommand::nothing;
	}
	delete plugIn.m_pSampleChangeQueue;
	plugIn.m_pSampleChangeQueue = nullptr;
	return CRhinoCommand::success;
}

#pragma endregion
