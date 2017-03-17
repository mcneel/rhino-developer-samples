/////////////////////////////////////////////////////////////////////////////
// cmdPlugIn2.cpp : command file
//

#include "StdAfx.h"
#include "PlugIn2PlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN PlugIn2SharedAdd command
//

class CCommandPlugIn2SharedAdd : public CRhinoCommand
{
public:
	CCommandPlugIn2SharedAdd() {}
	~CCommandPlugIn2SharedAdd() {}
	UUID CommandUUID()
	{
		// {85B810DE-73D3-41EF-8D0E-55ACE573388A}
		static const GUID PlugIn2SharedAddCommand_UUID =
		{ 0x85B810DE, 0x73D3, 0x41EF, { 0x8D, 0x0E, 0x55, 0xAC, 0xE5, 0x73, 0x38, 0x8A } };
		return PlugIn2SharedAddCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"PlugIn2SharedAdd"; }
	const wchar_t* LocalCommandName() { return L"PlugIn2SharedAdd"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandPlugIn2SharedAdd object
static class CCommandPlugIn2SharedAdd thePlugIn2SharedAddCommand;

CRhinoCommand::result CCommandPlugIn2SharedAdd::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select object" );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  const CRhinoObject* object = go.Object(0).Object();
  if( 0 == object )
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt( L"Pick point" );
  gp.GetPoint();
  if( gp.CommandResult() != CRhinoCommand::success )
    return gp.CommandResult();

  CRhinoGetString gs;
  gp.SetCommandPrompt( L"Description" );
  gs.GetString();
  if( gs.CommandResult() != CRhinoCommand::success )
    return gs.CommandResult();

  bool rc = AddPlugInUserData( context.m_doc, object, gp.Point(), gs.String() );
  RhinoApp().Print( L"%s\n", rc ? L"Succeeded!" : L"Failed!" );

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END PlugIn2SharedAdd command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN PlugIn2SharedGet command
//

class CCommandPlugIn2SharedGet : public CRhinoCommand
{
public:
	CCommandPlugIn2SharedGet() {}
	~CCommandPlugIn2SharedGet() {}
	UUID CommandUUID()
	{
		// {D023438A-1174-472C-A5A7-6B6A5A009942}
		static const GUID PlugIn2SharedGetCommand_UUID =
		{ 0xD023438A, 0x1174, 0x472C, { 0xA5, 0xA7, 0x6B, 0x6A, 0x5A, 0x00, 0x99, 0x42 } };
		return PlugIn2SharedGetCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"PlugIn2SharedGet"; }
	const wchar_t* LocalCommandName() { return L"PlugIn2SharedGet"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandPlugIn2SharedGet object
static class CCommandPlugIn2SharedGet thePlugIn2SharedGetCommand;

CRhinoCommand::result CCommandPlugIn2SharedGet::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select object" );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  const CRhinoObject* object = go.Object(0).Object();
  if( 0 == object )
    return CRhinoCommand::failure;

  ON_3dPoint point;
  ON_wString string;
  bool rc = GetPlugInUserData( object, point, string );
  if( rc )
  {
    ON_wString pointstr;
    RhinoFormatPoint( point, pointstr );
    RhinoApp().Print( L"point = %s, string = %s\n", pointstr, string );
  }
  else
  {
    RhinoApp().Print( L"Failed!\n" );
  }

  return CRhinoCommand::success;
}

//
// END PlugIn2SharedGet command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN PlugIn2SharedRemove command
//

class CCommandPlugIn2SharedRemove : public CRhinoCommand
{
public:
	CCommandPlugIn2SharedRemove() {}
	~CCommandPlugIn2SharedRemove() {}
	UUID CommandUUID()
	{
		// {54C223BD-6848-4329-8DCB-64BD9EE82C60}
		static const GUID PlugIn2SharedRemoveCommand_UUID =
		{ 0x54C223BD, 0x6848, 0x4329, { 0x8D, 0xCB, 0x64, 0xBD, 0x9E, 0xE8, 0x2C, 0x60 } };
		return PlugIn2SharedRemoveCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"PlugIn2SharedRemove"; }
	const wchar_t* LocalCommandName() { return L"PlugIn2SharedRemove"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandPlugIn2SharedRemove object
static class CCommandPlugIn2SharedRemove thePlugIn2SharedRemoveCommand;

CRhinoCommand::result CCommandPlugIn2SharedRemove::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetCommandPrompt( L"Select object" );
  go.GetObjects( 1, 1 );
  if( go.CommandResult() != CRhinoCommand::success )
    return go.CommandResult();

  const CRhinoObject* object = go.Object(0).Object();
  if( 0 == object )
    return CRhinoCommand::failure;

  bool rc = RemovePlugInUserData( context.m_doc, object );
  RhinoApp().Print( L"%s\n", rc ? L"Succeeded!" : L"Failed!" );

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END PlugIn2SharedRemove command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
