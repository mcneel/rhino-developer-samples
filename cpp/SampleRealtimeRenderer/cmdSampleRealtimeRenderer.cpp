#include "StdAfx.h"
#include "SampleRealtimeRendererPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRealtimeRenderer command
//

#pragma region SampleRealtimeRenderer command

class CCommandSampleRealtimeRenderer : public CRhinoCommand
{
public:
	CCommandSampleRealtimeRenderer() = default;
	~CCommandSampleRealtimeRenderer() = default;
	UUID CommandUUID() override
	{
		// {2597832E-2F14-4F39-8607-6D03DEDD8D33}
		static const GUID id =
		{ 0x2597832e, 0x2f14, 0x4f39, { 0x86, 0x7, 0x6d, 0x3, 0xde, 0xdd, 0x8d, 0x33 } };
		return id;
	}
	const wchar_t* EnglishCommandName() override { return L"SampleRealtimeRenderer"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleRealtimeRenderer object
static class CCommandSampleRealtimeRenderer theSampleRealtimeRendererCommand;

CRhinoCommand::result CCommandSampleRealtimeRenderer::RunCommand(const CRhinoCommandContext& context)
{
	UNREFERENCED_PARAMETER(context);
	RhinoApp().Print(L"%s plug-in loaded.\n", SampleRealtimeRendererPlugIn().PlugInName());
	return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRealtimeRenderer command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
