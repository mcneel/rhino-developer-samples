// cmdSampleChangeQueue.cpp : command file
//

#include "StdAfx.h"
#include "SampleChangeQueuePlugIn.h"
#include "SampleChangeQueue.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleChangeQueue command
//

#pragma region SampleChangeQueue command

// Do NOT put the definition of class CCommandSampleChangeQueue in a header
// file. There is only ONE instance of a CCommandSampleChangeQueue class
// and that instance is the static theSampleChangeQueueCommand that appears
// immediately below the class definition.

class CCommandSampleChangeQueue : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleChangeQueue is created below.
  // No copy constructor or operator= is required.
  // Values of member variables persist for the duration of the application.

  // CCommandSampleChangeQueue::CCommandSampleChangeQueue()
  // is called exactly once when static theSampleChangeQueueCommand is created.
  CCommandSampleChangeQueue() = default;

  // CCommandSampleChangeQueue::~CCommandSampleChangeQueue()
  // is called exactly once when static theSampleChangeQueueCommand is destroyed.
  // The destructor should not make any calls to the Rhino SDK. 
  // If your command has persistent settings, then override 
  // CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleChangeQueue() = default;

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work. Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID() override
  {
    // {A2B4F368-533F-4CB9-B955-C19316FF934A}
    static const GUID SampleChangeQueueCommand_UUID =
    { 0xA2B4F368, 0x533F, 0x4CB9, { 0xB9, 0x55, 0xC1, 0x93, 0x16, 0xFF, 0x93, 0x4A } };
    return SampleChangeQueueCommand_UUID;
  }

  // Returns the English command name.
  // If you want to provide a localized command name, then override 
  // CRhinoCommand::LocalCommandName.
  const wchar_t* EnglishCommandName() override { return L"SampleChangeQueue"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleChangeQueue object
// Do NOT create any other instance of a CCommandSampleChangeQueue class.
static class CCommandSampleChangeQueue theSampleChangeQueueCommand;

CRhinoCommand::result CCommandSampleChangeQueue::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CSampleChangeQueuePlugIn& plugIn = SampleChangeQueuePlugIn();
  if (plugIn.m_pSampleChangeQueue) {
  	RhinoApp().Print(L"A SampleChangeQueue is already running\n");
  	return CRhinoCommand::nothing;
  }

  CRhinoView* rhinoView = RhinoApp().ActiveView();
  const ON_3dmView& vw = rhinoView->ActiveViewport().View();

  // construct our changequeue.
  plugIn.m_pSampleChangeQueue = new SampleChangeQueue(*doc, PlugIn()->PlugInID(), vw, nullptr, false, true);

  // we HAVE to run CreateWorld on the main thread, so lets do it here - commands run on the main thread.
  plugIn.m_pSampleChangeQueue->CreateWorld(true);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleChangeQueue command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
