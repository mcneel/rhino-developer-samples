
#include "stdafx.h"
#include "SampleRdkMaterialPreviewRdkPlugIn.h"
#include "SampleRdkMaterialPreviewPlugIn.h"
#include "SampleRdkMaterialWithPreview.h"

UUID CSampleRdkMaterialPreviewRdkPlugIn::PlugInId() const
{
	return ::SampleRdkMaterialPreviewPlugIn().PlugInID();
}

CRhinoPlugIn& CSampleRdkMaterialPreviewRdkPlugIn::RhinoPlugIn() const
{
	return ::SampleRdkMaterialPreviewPlugIn();
}

bool CSampleRdkMaterialPreviewRdkPlugIn::Initialize()
{
	// TODO: Initialize your plug-in. Return false on failure.

	return __super::Initialize();
}

void CSampleRdkMaterialPreviewRdkPlugIn::Uninitialize()
{
	// TODO: Do any necessary plug-in clean-up here.

	__super::Uninitialize();
}

void CSampleRdkMaterialPreviewRdkPlugIn::RegisterExtensions() const
{
	AddExtension(new CSampleRdkMaterialWithPreviewFactory);

	// TODO: Add further material/environment/texture factories by calling AddExtension(new MyFactory);

	__super::RegisterExtensions();
}

void CSampleRdkMaterialPreviewRdkPlugIn::AbortRender()
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

bool CSampleRdkMaterialPreviewRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality,
                                                       const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify,
                                                       OUT CRhinoDib& dibOut)
{
	// TODO: Create a rendered preview of the specified scene at the specified size and quality.
	if (nullptr == pSceneServer)
		return false;

	CRhRdkColor colTarget;

	// The scene server contains one or more objects that have materials associated with them.
	// A real renderer would render those objects using their materials, but we just try to find
	// our customized material and use that directly.
	const IRhRdkPreviewSceneServer::IObject* pObject = nullptr;
	while (nullptr != (pObject = pSceneServer->NextObject()))
	{
		const auto* pMaterial = dynamic_cast<const CSampleRdkMaterialWithPreview*>(pObject->Material());
		if (nullptr != pMaterial)
		{
			// Found it, so get the diffuse color as the target color.
			colTarget = pMaterial->GetParameter(DIFFUSE_PARAM_NAME).AsRdkColor();
			break;
		}
	}

	CCancelPreview cp(m_bCancelPreview);
	if (nullptr != pNotify)
	{
		pNotify->SetCancelCallback(&cp);
	}

	ASSERT(PreviewRenderType() == PreviewRenderTypes::Progressive);

	// Because PreviewRenderType() returns PreviewRenderTypes::Progressive, we are
	// only called for 'full' quality. It's up to us to render progressively
	// better previews and send them out via pNotify.
	ASSERT(RhRdkPreviewQuality::Full == quality);

	// Create the dib at the requested size.
	CRhinoDib dib(sizeImage.cx, sizeImage.cy, 24);

	// This loop generates progressively better previews where 'better' means 'more like colTarget'.
	float amount = 0.0f;
	while ((amount < 1.01f) && !m_bCancelPreview)
	{
		CRhRdkColor col(RGB(255, 255, 255));
		col.BlendTo(amount, colTarget);
		dib.FillSolid(col.ColorRef());

		if (nullptr != pNotify)
		{
			// This updates the preview in the material editor.
			pNotify->NotifyIntermediateUpdate(dib);

			// Fake a long rendering time.
			::Sleep(300);
		}

		amount += 0.1f;
	}

	if (nullptr != pNotify)
	{
		pNotify->SetCancelCallback(nullptr);
	}

	// Output the final version of the preview.
	dibOut = dib;

	return true; // If you don't want to implement this, return false.
}

bool CSampleRdkMaterialPreviewRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, const CRhRdkTexture& texture, OUT CRhinoDib& dibOut)
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

bool CSampleRdkMaterialPreviewRdkPlugIn::SupportsFeature(const UUID& uuidFeature) const
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
