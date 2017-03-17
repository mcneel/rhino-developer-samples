#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDimAngular command
//

#pragma region SampleDimAngular command

class CCommandSampleDimAngular : public CRhinoCommand
{
public:
  CCommandSampleDimAngular() {}
  ~CCommandSampleDimAngular() {}
  UUID CommandUUID()
  {
  // {D6AE4A0E-5056-452A-888C-B309E7CB12FF}
  static const GUID SampleDimAngularCommand_UUID =
  { 0xD6AE4A0E, 0x5056, 0x452A, { 0x88, 0x8C, 0xB3, 0x09, 0xE7, 0xCB, 0x12, 0xFF } };
  return SampleDimAngularCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDimAngular"; }
  const wchar_t* LocalCommandName() const { return L"SampleDimAngular"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleDimAngular object
static class CCommandSampleDimAngular theSampleDimAngularCommand;

CRhinoCommand::result CCommandSampleDimAngular::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoAngularDimension* pDim = 0;
  CRhinoCommand::result rc = GetAngularDimension(context.m_doc, context.IsInteractive(), pDim, 0);
  if( rc == CRhinoCommand::success && 0 != pDim )
  {
    context.m_doc.AddObject( pDim, FALSE );
    context.m_doc.Redraw();
  } 

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDimAngular command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
