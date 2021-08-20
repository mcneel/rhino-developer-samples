
#include "stdafx.h"
#include "SampleRdkAsyncRendererRdkPlugIn.h"
#include "SampleRdkAsyncRendererPlugIn.h"

UUID CSampleRdkAsyncRendererRdkPlugIn::PlugInId() const
{
	return ::SampleRdkAsyncRendererPlugIn().PlugInID();
}

CRhinoPlugIn& CSampleRdkAsyncRendererRdkPlugIn::RhinoPlugIn() const
{
	return ::SampleRdkAsyncRendererPlugIn();
}

bool CSampleRdkAsyncRendererRdkPlugIn::Initialize()
{
	// TODO: Initialize your plug-in. Return false on failure.

	return __super::Initialize();
}

void CSampleRdkAsyncRendererRdkPlugIn::Uninitialize()
{
	// TODO: Do any necessary plug-in clean-up here.

	__super::Uninitialize();
}

void CSampleRdkAsyncRendererRdkPlugIn::RegisterExtensions() const
{
	// TODO: Add material/environment/texture factories by calling AddExtension(new MyFactory);
	// See SampleRdkMaterialCustomUI project.

	__super::RegisterExtensions();
}

void CSampleRdkAsyncRendererRdkPlugIn::AbortRender()
{
	// TODO:
}

bool CSampleRdkAsyncRendererRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut)
{
	// TODO: Create a rendered preview of the specified scene at the specified size and quality.
	UNREFERENCED_PARAMETER(sizeImage);
	UNREFERENCED_PARAMETER(quality);
	UNREFERENCED_PARAMETER(pSceneServer);
	UNREFERENCED_PARAMETER(pNotify);
	UNREFERENCED_PARAMETER(dibOut);

	return NULL;
}

bool CSampleRdkAsyncRendererRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut)
{
	// TODO: Optionally create a preview of the texture.
	// Return false to allow RDK to produce all texture previews.
	UNREFERENCED_PARAMETER(sizeImage);
	UNREFERENCED_PARAMETER(texture);
	UNREFERENCED_PARAMETER(dibOut);

	return NULL;
}

bool CSampleRdkAsyncRendererRdkPlugIn::SupportsFeature(const UUID& uuidFeature) const
{
	// TODO: Determine which features of the RDK are exposed while this is the current renderer.

	if (uuidFeature == uuidFeatureCustomRenderMeshes)
		return true; // This renderer supports custom render meshes (because it uses the iterator).

	if (uuidFeature == uuidFeatureDecals)
		return false; // This renderer does not support decals.

	if (uuidFeature == uuidFeatureGroundPlane)
		return false; // This renderer does not support a ground plane.

	if (uuidFeature == uuidFeatureSun)
		return false; // This renderer does not support the Sun.

	return true;
}
