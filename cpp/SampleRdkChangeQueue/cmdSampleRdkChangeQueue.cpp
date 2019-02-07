// cmdSampleRdkChangeQueue.cpp : command file
//

#include "StdAfx.h"
#include "SampleRdkChangeQueuePlugIn.h"
#include "CustomChangeQueue.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRdkChangeQueue command
//

#pragma region SampleRdkChangeQueue command

// Do NOT put the definition of class CCommandSampleRdkChangeQueue in a header
// file. There is only ONE instance of a CCommandSampleRdkChangeQueue class
// and that instance is the static theSampleRdkChangeQueueCommand that appears
// immediately below the class definition.

class CCommandSampleRdkChangeQueue : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleRdkChangeQueue is created below.
  // No copy constructor or operator= is required.
  // Values of member variables persist for the duration of the application.

  // CCommandSampleRdkChangeQueue::CCommandSampleRdkChangeQueue()
  // is called exactly once when static theSampleRdkChangeQueueCommand is created.
  CCommandSampleRdkChangeQueue() = default;

  // CCommandSampleRdkChangeQueue::~CCommandSampleRdkChangeQueue()
  // is called exactly once when static theSampleRdkChangeQueueCommand is destroyed.
  // The destructor should not make any calls to the Rhino SDK. 
  // If your command has persistent settings, then override 
  // CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleRdkChangeQueue() = default;

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work. Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID() override
  {
    // {86123FA5-E970-4833-BC6D-8A60EB7F22A1}
    static const GUID SampleRdkChangeQueueCommand_UUID =
    { 0x86123FA5, 0xE970, 0x4833, { 0xBC, 0x6D, 0x8A, 0x60, 0xEB, 0x7F, 0x22, 0xA1 } };
    return SampleRdkChangeQueueCommand_UUID;
  }

  // Returns the English command name.
  // If you want to provide a localized command name, then override 
  // CRhinoCommand::LocalCommandName.
  const wchar_t* EnglishCommandName() override { return L"SampleRdkChangeQueue"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleRdkChangeQueue object
// Do NOT create any other instance of a CCommandSampleRdkChangeQueue class.
static class CCommandSampleRdkChangeQueue theSampleRdkChangeQueueCommand;

CRhinoCommand::result CCommandSampleRdkChangeQueue::RunCommand(const CRhinoCommandContext& context)
{
	const CRhinoDoc* pDoc = context.Document();
	if (nullptr == pDoc)
		return failure;

	const CRhinoView* pView = RhinoApp().ActiveView();
	if (nullptr == pView)
		return failure;


	RhRdk::Realtime::ChangeQueue* pQueue = nullptr;
	
	RhinoApp().Print(L"Starting CustomChangeQueue\n");
	pQueue = new CustomChangeQueue(*pDoc, pView->MainViewport().View());
	// create the world. This sets up the changequeue. Once the setup is
	// done all collected data is flushed. This means that as part of
	// CreateWorld all implemented Apply* functions are called.
	// The Apply functions allow us to gather all necessary data.
	// the order in which these functions are called is:
	// ApplyViewChange
	// ApplyLinearWorkflowChanges
	// ApplyDynamicObjectTransforms
	// ApplyDynamicLightChanges
	// ApplyRenderSettingsChanges
	// ApplyEnvironmentChanges(IRhRdkCurrentEnvironment::Usage::Background);
	// ApplyEnvironmentChanges(IRhRdkCurrentEnvironment::Usage::Skylighting);
	// ApplyEnvironmentChanges(IRhRdkCurrentEnvironment::Usage::Reflection);
	// ApplySkylightChanges
	// ApplySunChanges
	// ApplyLightChanges
	// ApplyMaterialChanges *this is not called on CreateWorld, since materials haven't changed yet
	// ApplyMeshChanges
	// ApplyMeshInstanceChanges
	// ApplyGroundPlaneChanges
	// ApplyClippingPlaneChanges
	// const_cast<ChangeQueue*>(this)->EVF(L"ApplyDisplayAttributesChanges", m_pImpl->m_pDisplayPipelineAttributes);
	// If there is data you are not interested in just don't implement that Apply function.
	pQueue->CreateWorld();

	// now we have collected all data you are free to do with it whatever you want.
	RhinoApp().Print(L"Destroying CustomChangeQueue\n");
	delete pQueue;
	pQueue = nullptr;
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRdkChangeQueue command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
