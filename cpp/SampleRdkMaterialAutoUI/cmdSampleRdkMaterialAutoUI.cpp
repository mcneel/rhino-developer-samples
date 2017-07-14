
#include "StdAfx.h"
#include "SampleRdkMaterialAutoUIPlugIn.h"
#include "SampleRdkMaterial.h"

#pragma region SampleRdkMaterialAutoUI command

class CCommandSampleRdkMaterialAutoUI : public CRhinoCommand
{
public:
  virtual UUID CommandUUID() override { static const UUID uuid = { 0xD9C87E8A, 0xAB48, 0x4D47, { 0x90, 0x51, 0x1C, 0x7A, 0xDD, 0x13, 0xDF, 0x61 } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleRdkMaterialAutoUI"; }
  virtual CRhinoCommand::result RunCommand( const CRhinoCommandContext& ) override;
};

// The one and only CCommandSampleRdkMaterialAutoUI object
static class CCommandSampleRdkMaterialAutoUI theSampleRdkMaterialAutoUICommand;

CRhinoCommand::result CCommandSampleRdkMaterialAutoUI::RunCommand( const CRhinoCommandContext& context )
{
	auto pDoc = context.Document();
	if (nullptr == pDoc)
		return failure;

	auto& rdkDoc = pDoc->RdkDocument().BeginChange(RhRdkChangeContext::UI);

	auto pContent = new CSampleRdkMaterial;
	pContent->Initialize();

	rdkDoc.AttachContent(*pContent);

	rdkDoc.EndChange();

	return success;
}

#pragma endregion

//
// END SampleRdkMaterialAutoUI command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
