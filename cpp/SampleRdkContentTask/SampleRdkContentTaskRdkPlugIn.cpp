
#include "stdafx.h"
#include "SampleRdkContentTaskRdkPlugIn.h"
#include "SampleRdkContentTaskPlugIn.h"
#include "SampleRdkContentTask.h"

UUID CSampleRdkContentTaskRdkPlugIn::PlugInId() const
{
	return ::SampleRdkContentTaskPlugIn().PlugInID();
}

CRhinoPlugIn& CSampleRdkContentTaskRdkPlugIn::RhinoPlugIn() const
{
	return ::SampleRdkContentTaskPlugIn();
}

bool CSampleRdkContentTaskRdkPlugIn::Initialize()
{
	// TODO: Initialize your plug-in. Return false on failure.

	return __super::Initialize();
}

void CSampleRdkContentTaskRdkPlugIn::Uninitialize()
{
	// TODO: Do any necessary plug-in clean-up here.

	__super::Uninitialize();
}

void CSampleRdkContentTaskRdkPlugIn::RegisterExtensions() const
{
	AddExtension(new CContentTask);

	// TODO: Add material/environment/texture factories.

	__super::RegisterExtensions();
}

void CSampleRdkContentTaskRdkPlugIn::AbortRender()
{
}

class CCancelPreview : public IRhRdkPreviewCallbacks::ICancel
{
public:
	CCancelPreview(bool& b) : m_bCancel(b) { m_bCancel = false; }

	virtual void CancelPreview() const override { m_bCancel = true; }

	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override { return nullptr; }

private:
	bool& m_bCancel;
};

bool CSampleRdkContentTaskRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality,
                                                       const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify,
                                                       OUT CRhinoDib& dibOut)
{
	// TODO: Create a rendered preview of the specified scene at the specified size and quality.
	UNREFERENCED_PARAMETER(pSceneServer);

	CCancelPreview cp(m_bCancelPreview);
	pNotify->SetCancelCallback(&cp);

	ASSERT(PreviewRenderType() == PreviewRenderTypes::Progressive);

	// Because PreviewRenderType() returns PreviewRenderTypes::Progressive, we are
	// only called for 'full' quality. It's up to us to render progressively
	// better previews and send them out via pNotify.
	ASSERT(RhRdkPreviewQuality::Full == quality);

	// Create the dib at the requested size.
	CRhinoDib dib(sizeImage.cx, sizeImage.cy, 24);

	// Target color is red.
	const CRhRdkColor colTarget(RGB(255, 0, 0));

	// This loop generates progressively better previews where 'better' means 'more red'.
	float amount = 0.0f;
	while ((amount < 1.001f) && !m_bCancelPreview)
	{
		// Fake a long rendering time.
		::Sleep(700);

		CRhRdkColor col(RGB(255, 255, 255));
		col.BlendTo(amount, colTarget);
		dib.FillSolid(col.ColorRef());

		// This updates the preview in the material editor.
		pNotify->NotifyIntermediateUpdate(dib);

		amount += 0.1f;
	}

	pNotify->SetCancelCallback(nullptr);

	// Output the final version of the preview.
	dibOut = dib;

	return true; // If you don't want to implement this, return false.
}

bool CSampleRdkContentTaskRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, OUT CRhinoDib& dibOut)
{
	// TODO: Optionally create a preview of the texture.
	// Return false to allow RDK to produce all texture previews.
	UNREFERENCED_PARAMETER(texture);

	// Create the dib at the requested size.
	CRhinoDib dib(sizeImage.cx, sizeImage.cy, 24);

	// Set the dib to blue. In reality you would generate the dib from the texture.
	dib.FillSolid(RGB(0, 128, 255));

	dibOut = dib;

	return true; // If you don't want to implement this, return false.
}

bool CSampleRdkContentTaskRdkPlugIn::SupportsFeature(const UUID& uuidFeature) const
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
