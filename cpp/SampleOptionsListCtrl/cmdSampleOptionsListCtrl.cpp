/////////////////////////////////////////////////////////////////////////////
// cmdSampleOptionsListCtrl.cpp : command file
//

#include "StdAfx.h"
#include "SampleOptionsListCtrlPlugIn.h"
#include "SampleOptionsListCtrlDlg.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleOptionsListCtrl command
//

#pragma region SampleOptionsListCtrl command

class CCommandSampleOptionsListCtrl : public CRhinoCommand
{
public:
	CCommandSampleOptionsListCtrl() {}
  ~CCommandSampleOptionsListCtrl() {}
	UUID CommandUUID()
	{
		// {C9704599-3C83-40AE-A4A8-A0A98827B1B6}
    static const GUID SampleOptionsListCtrlCommand_UUID =
    { 0xC9704599, 0x3C83, 0x40AE, { 0xA4, 0xA8, 0xA0, 0xA9, 0x88, 0x27, 0xB1, 0xB6 } };
    return SampleOptionsListCtrlCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"SampleOptionsListCtrl"; }
	const wchar_t* LocalCommandName() const { return L"SampleOptionsListCtrl"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};
static class CCommandSampleOptionsListCtrl theSampleOptionsListCtrlCommand;

CRhinoCommand::result CCommandSampleOptionsListCtrl::RunCommand( const CRhinoCommandContext& context )
{
  ON_UUID tabId = CSampleOptionsListCtrlDlg::ID();

  if( context.IsInteractive() )
  {
    CRhinoTabbedDockBarDialog::OpenDockbarTab( tabId );
  }
  else
  {
    bool bVisible = CRhinoTabbedDockBarDialog::IsTabVisible( tabId );
 
    ON_wString str;
    str.Format( L"%s is %s. New value", LocalCommandName(), bVisible ? L"visible" : L"hidden" );
 
    CRhinoGetOption go;
    go.SetCommandPrompt( str );
    int h_option = go.AddCommandOption( RHCMDOPTNAME(L"Hide") );
    int s_option = go.AddCommandOption( RHCMDOPTNAME(L"Show") );
    int t_option = go.AddCommandOption( RHCMDOPTNAME(L"Toggle") );
    go.GetOption();
    if( go.CommandResult() != CRhinoCommand::success )
      return go.CommandResult();
 
    const CRhinoCommandOption* option = go.Option();
    if( 0 == option )
      return CRhinoCommand::failure;
 
    int option_index = option->m_option_index;

    if( h_option == option_index && bVisible )
      CRhinoTabbedDockBarDialog::ShowDockbarTab( tabId, false );
    else if( s_option == option_index && !bVisible  )
      CRhinoTabbedDockBarDialog::ShowDockbarTab( tabId, true );
    else if( t_option == option_index )
      CRhinoTabbedDockBarDialog::ShowDockbarTab( tabId, !bVisible );
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleOptionsListCtrl command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
