/////////////////////////////////////////////////////////////////////////////
// cmdPlugIn1.cpp : command file
//

#include "StdAfx.h"
#include "PlugIn1PlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN PlugIn1SharedAdd command
//

class CCommandPlugIn1SharedAdd : public CRhinoCommand
{
public:
	CCommandPlugIn1SharedAdd() {}
	~CCommandPlugIn1SharedAdd() {}
	UUID CommandUUID()
	{
		// {DBC4075F-3C99-4397-9732-E3480C31224B}
		static const GUID PlugIn1SharedAddCommand_UUID =
		{ 0xDBC4075F, 0x3C99, 0x4397, { 0x97, 0x32, 0xE3, 0x48, 0x0C, 0x31, 0x22, 0x4B } };
		return PlugIn1SharedAddCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"PlugIn1SharedAdd"; }
	const wchar_t* LocalCommandName() { return L"PlugIn1SharedAdd"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandPlugIn1SharedAdd object
static class CCommandPlugIn1SharedAdd thePlugIn1SharedAddCommand;

CRhinoCommand::result CCommandPlugIn1SharedAdd::RunCommand( const CRhinoCommandContext& context )
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
// END PlugIn1SharedAdd command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN PlugIn1SharedGet command
//

class CCommandPlugIn1SharedGet : public CRhinoCommand
{
public:
	CCommandPlugIn1SharedGet() {}
	~CCommandPlugIn1SharedGet() {}
	UUID CommandUUID()
	{
		// {4307651B-1E1A-4655-96A1-46E4B549B437}
		static const GUID PlugIn1SharedGetCommand_UUID =
		{ 0x4307651B, 0x1E1A, 0x4655, { 0x96, 0xA1, 0x46, 0xE4, 0xB5, 0x49, 0xB4, 0x37 } };
		return PlugIn1SharedGetCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"PlugIn1SharedGet"; }
	const wchar_t* LocalCommandName() { return L"PlugIn1SharedGet"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandPlugIn1SharedGet object
static class CCommandPlugIn1SharedGet thePlugIn1SharedGetCommand;

CRhinoCommand::result CCommandPlugIn1SharedGet::RunCommand( const CRhinoCommandContext& context )
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
// END PlugIn1SharedGet command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN PlugIn1SharedRemove command
//

class CCommandPlugIn1SharedRemove : public CRhinoCommand
{
public:
	CCommandPlugIn1SharedRemove() {}
	~CCommandPlugIn1SharedRemove() {}
	UUID CommandUUID()
	{
		// {3EF3659A-D7AA-4164-B72E-39AEFF3C47B3}
		static const GUID PlugIn1SharedRemoveCommand_UUID =
		{ 0x3EF3659A, 0xD7AA, 0x4164, { 0xB7, 0x2E, 0x39, 0xAE, 0xFF, 0x3C, 0x47, 0xB3 } };
		return PlugIn1SharedRemoveCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"PlugIn1SharedRemove"; }
	const wchar_t* LocalCommandName() { return L"PlugIn1SharedRemove"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandPlugIn1SharedRemove object
static class CCommandPlugIn1SharedRemove thePlugIn1SharedRemoveCommand;

CRhinoCommand::result CCommandPlugIn1SharedRemove::RunCommand( const CRhinoCommandContext& context )
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
// END PlugIn1SharedRemove command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
