
#include "stdafx.h"
#include "SampleRdkRendererRdkPlugIn.h"
#include "SampleRdkRendererPlugIn.h"

UUID CSampleRdkRendererRdkPlugIn::PlugInId() const
{
	return ::SampleRdkRendererPlugIn().PlugInID();
}

CRhinoPlugIn& CSampleRdkRendererRdkPlugIn::RhinoPlugIn() const
{
	return ::SampleRdkRendererPlugIn();
}

bool CSampleRdkRendererRdkPlugIn::Initialize()
{
	// TODO: Initialize your plug-in. Return false on failure.

	return __super::Initialize();
}

void CSampleRdkRendererRdkPlugIn::Uninitialize()
{
	// TODO: Do any necessary plug-in clean-up here.

	__super::Uninitialize();
}

void CSampleRdkRendererRdkPlugIn::RegisterExtensions() const
{
	// TODO: Add material/environment/texture factories.
	// See SampleRdkMaterialCustomUI project.

	__super::RegisterExtensions();
}

void CSampleRdkRendererRdkPlugIn::AbortRender()
{
	// TODO:
}

bool CSampleRdkRendererRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality, const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut)
{
	// TODO: Create a rendered preview of the specified scene at the specified size and quality.
	UNREFERENCED_PARAMETER(sizeImage);
	UNREFERENCED_PARAMETER(quality);
	UNREFERENCED_PARAMETER(pSceneServer);
	UNREFERENCED_PARAMETER(pNotify);
	UNREFERENCED_PARAMETER(dibOut);

	return NULL;
}

bool CSampleRdkRendererRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, CRhinoDib& dibOut)
{
	// TODO: Optionally create a preview of the texture.
	// Return false to allow RDK to produce all texture previews.
	UNREFERENCED_PARAMETER(sizeImage);
	UNREFERENCED_PARAMETER(texture);
	UNREFERENCED_PARAMETER(dibOut);

	return NULL;
}

bool CSampleRdkRendererRdkPlugIn::SupportsFeature(const UUID& uuidFeature) const
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
