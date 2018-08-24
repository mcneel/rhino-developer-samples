
#include "stdafx.h"
#include "SampleRdkEffectsPanelRdkPlugIn.h"
#include "SampleRdkEffectsPanelPlugIn.h"
#include "SampleRdkEffectsPanelDialog.h"

UUID CSampleRdkEffectsPanelRdkPlugIn::PlugInId() const
{
	return ::SampleRdkEffectsPanelPlugIn().PlugInID();
}

CRhinoPlugIn& CSampleRdkEffectsPanelRdkPlugIn::RhinoPlugIn() const
{
	return ::SampleRdkEffectsPanelPlugIn();
}

bool CSampleRdkEffectsPanelRdkPlugIn::Initialize(void)
{
	// TODO: Initialize your plug-in. Return false on failure.

	return __super::Initialize();
}

void CSampleRdkEffectsPanelRdkPlugIn::Uninitialize(void)
{
	// TODO: Do any necessary plug-in clean-up here.

	__super::Uninitialize();
}

void CSampleRdkEffectsPanelRdkPlugIn::RegisterExtensions(void) const
{
	AddExtension(new CSampleRdkEffectsPanelTabFactory);

	__super::RegisterExtensions();
}

void CSampleRdkEffectsPanelRdkPlugIn::AbortRender()
{
}

bool CSampleRdkEffectsPanelRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut)
{
	// TODO: Create a rendered preview of the specified scene at the specified size and quality.
	UNREFERENCED_PARAMETER(sizeImage);
	UNREFERENCED_PARAMETER(quality);
	UNREFERENCED_PARAMETER(pSceneServer);
	UNREFERENCED_PARAMETER(pNotify);
	UNREFERENCED_PARAMETER(dibOut);

	return NULL;
}

bool CSampleRdkEffectsPanelRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut)
{
	// TODO: Optionally create a preview of the texture.
	// Return false to allow RDK to produce all texture previews.
	UNREFERENCED_PARAMETER(sizeImage);
	UNREFERENCED_PARAMETER(texture);
	UNREFERENCED_PARAMETER(dibOut);

	return NULL;
}

bool CSampleRdkEffectsPanelRdkPlugIn::SupportsFeature(const UUID& uuidFeature) const
{
	// TODO: Determine which features of the RDK are exposed while this is the current renderer.

//	if (uuidFeature == uuidFeatureSun)
//		return false; // This renderer does not support the Sun.

	return true;
}
