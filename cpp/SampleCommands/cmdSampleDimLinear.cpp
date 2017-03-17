#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimLinear command
//

#pragma region SampleDimLinear command

class CCommandSampleDimLinear : public CRhinoCommand
{
public:
  CCommandSampleDimLinear() {}
  ~CCommandSampleDimLinear() {}
  UUID CommandUUID()
  {
    // {472985B0-C869-4513-A91B-E718450A55B0}
    static const GUID SampleDimLinearCommand_UUID =
    { 0x472985B0, 0xC869, 0x4513, { 0xA9, 0x1B, 0xE7, 0x18, 0x45, 0x0A, 0x55, 0xB0 } };
    return SampleDimLinearCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDimLinear"; }
  const wchar_t* LocalCommandName() const { return L"SampleDimLinear"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDimLinear object
static class CCommandSampleDimLinear theSampleDimLinearCommand;

CRhinoCommand::result CCommandSampleDimLinear::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoLinearDimension* pDim = 0;
 
  CArgsRhinoDimLinear args;
  args.SetFirstPointPrompt( L"First dimension point" );
  args.SetSecondPointPrompt( L"Second dimension point" );
  args.SetDragPointPrompt( L"Dimension location" );
  args.SetIsInteractive( context.IsInteractive() ? true : false );
 
  CRhinoCommand::result rc = RhinoGetDimLinear( args, pDim, 0 );
 
  if( rc == CRhinoCommand::success && 0 != pDim )
  {
    context.m_doc.AddObject( pDim, FALSE );
    context.m_doc.Redraw();
  } 
 
  return rc;
}

#pragma endregion

//
// END SampleDimLinear command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
