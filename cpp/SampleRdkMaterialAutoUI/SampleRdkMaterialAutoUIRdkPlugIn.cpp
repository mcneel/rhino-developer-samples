
#include "stdafx.h"
#include "SampleRdkMaterialAutoUIRdkPlugIn.h"
#include "SampleRdkMaterialAutoUIPlugIn.h"
#include "SampleRdkMaterial.h"
#include "SampleRdkMaterialIOPlugIn.h"

CRhinoPlugIn& CSampleRdkMaterialAutoUIRdkPlugIn::RhinoPlugIn(void) const
{
	return ::SampleRdkMaterialAutoUIPlugIn();
}

UUID CSampleRdkMaterialAutoUIRdkPlugIn::RhinoPlugInId(void) // Static.
{
	return ::SampleRdkMaterialAutoUIPlugIn().PlugInID();
}

UUID CSampleRdkMaterialAutoUIRdkPlugIn::RdkPlugInId(void) // Static.
{
	return RhinoPlugInId();
}

UUID CSampleRdkMaterialAutoUIRdkPlugIn::RenderEngineId(void) // Static.
{
	return RhinoPlugInId();
}

bool CSampleRdkMaterialAutoUIRdkPlugIn::Initialize(void)
{
	// TODO: Initialize your plug-in. Return false on failure.

	return __super::Initialize();
}

void CSampleRdkMaterialAutoUIRdkPlugIn::Uninitialize(void)
{
	// TODO: Do any necessary plug-in clean-up here.

	__super::Uninitialize();
}

void CSampleRdkMaterialAutoUIRdkPlugIn::AbortRender(void)
{
	// TODO:
}

void CSampleRdkMaterialAutoUIRdkPlugIn::RegisterExtensions(void) const
{
	AddExtension(new CSampleRdkMaterialFactory);
	AddExtension(new CSampleRdkMaterialIOPlugIn);
}

bool CSampleRdkMaterialAutoUIRdkPlugIn::SupportsFeature(const UUID&) const
{
	return true;
}
