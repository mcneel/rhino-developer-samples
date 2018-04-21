
#include "stdafx.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeMaterial.h"
#include "MarmaladeRdkPlugIn.h"

// Do NOT put the definition of class CCommandMarmalade in a header
// file. There is only ONE instance of a CCommandMarmalade class
// and that instance is the static theMarmaladeCommand that appears
// immediately below the class definition.

class CCommandMarmalade : public CRhinoCommand
{
public:
	// The one and only instance of CCommandMarmalade is created below.
	// No copy constructor or operator= is required.  Values of
	// member variables persist for the duration of the application.

	// CCommandMarmalade::CCommandMarmalade()
	// is called exactly once when static theMarmaladeCommand is created.
	CCommandMarmalade() { }

	// CCommandMarmalade::~CCommandMarmalade()
	// is called exactly once when static theMarmaladeCommand is
	// destroyed.  The destructor should not make any calls to
	// the Rhino SDK.  If your command has persistent settings,
	// then override CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
	~CCommandMarmalade() { }

	// Returns a unique UUID for this command.
	// If you try to use an id that is already being used, then
	// your command will not work.  Use GUIDGEN.EXE to make unique UUID.
	UUID CommandUUID()
	{
		static const GUID uuid =
		{
			// {FE313F11-E94F-4494-B339-90C983CD2AC4}
			0xFE313F11, 0xE94F, 0x4494, { 0xB3, 0x39, 0x90, 0xC9, 0x83, 0xCD, 0x2A, 0xC4 }
		};

		return uuid;
	}

	// Returns the English command name.
	const wchar_t* EnglishCommandName() { return L"Marmalade"; }

	// Returns the localized command name.
	const wchar_t* LocalCommandName() { return L"Marmalade"; }

	// Rhino calls RunCommand to run the command.
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandMarmalade object.  
// Do NOT create any other instance of a CCommandMarmalade class.
static class CCommandMarmalade theMarmaladeCommand;

CRhinoCommand::result CCommandMarmalade::RunCommand(const CRhinoCommandContext& context)
{
	// CCommandMarmalade::RunCommand() is called when the user runs the "Marmalade"
	// command or the "Marmalade" command is run by a history operation.

	// This demonstrates how to find a content by its instance id. There is no need to
	// know what kind of content the instance id refers to. Even temporary contents
	// can be found as shown here.

	const auto* pDoc = context.Document();
	if (nullptr == pDoc)
		return failure;

	CMarmaladeMaterial material; // It's a material but it could be any kind.

	const UUID uuidInstance = material.InstanceId();
	const auto* pContent = ::RhRdkFindContentInstance(pDoc, uuidInstance);
	if (nullptr != pContent)
	{
		// Rhino commands that display a dialog box interface should also support
		// a command-line, or scriptable interface.
		const ON_wString wStr = L"Found content";
		if (context.IsInteractive())
		{
			RhinoMessageBox(wStr, PlugIn()->PlugInName(), MB_OK);
		}
		else
		{
			RhinoApp().Print(wStr);
		}

		return success;
	}

	// TODO: Return one of the following values:
	//
	//   CRhinoCommand::success:  The command worked.
	//
	//   CRhinoCommand::failure:  The command failed because of invalid input, inability
	//                            to compute the desired result, or some other
	//                            computation reason.
	//
	//   CRhinoCommand::cancel:   The user interactively canceled the command 
	//                            (by pressing ESCAPE, clicking a CANCEL button, etc.)
	//                            in a Get operation, dialog, time consuming computation, etc.

	return failure;
}

class CMarmaladeOptionsCmd : public CRhinoCommand
{
protected:
	virtual UUID CommandUUID()
	{
		static const GUID uuid =
		{
			// {DEE34308-8314-42cb-ABE8-A894D1CAC7CA}
			0xdee34308, 0x8314, 0x42cb, { 0xab, 0xe8, 0xa8, 0x94, 0xd1, 0xca, 0xc7, 0xca }
		};

		return uuid;
	}

	virtual const wchar_t* EnglishCommandName() { return L"MarmaladeOptions"; }
	virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

static CMarmaladeOptionsCmd theOptionsCmd;

CRhinoCommand::result CMarmaladeOptionsCmd::RunCommand(const CRhinoCommandContext& context)
{
	// Check if script command is running.
	if (context.m_style == CRhinoCommandContext::script_ui)
	{	
		MarmaladePlugIn().ToggleNonModalOptionsDialog();
	}
	else
	{
		MarmaladePlugIn().ShowNonModalOptionsDialog(true);
	}

	return CRhinoCommand::success;
}
