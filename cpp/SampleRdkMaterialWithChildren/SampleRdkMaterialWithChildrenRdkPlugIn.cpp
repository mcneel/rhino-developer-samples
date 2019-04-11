
#include "stdafx.h"
#include "SampleRdkMaterialWithChildrenRdkPlugIn.h"
#include "SampleRdkMaterialWithChildrenPlugIn.h"
#include "SampleRdkMaterialWithChildren.h"
#include "Resource.h"

CRhinoPlugIn& CSampleRdkMaterialWithChildrenRdkPlugIn::RhinoPlugIn(void) const
{
	return ::SampleRdkMaterialAutoUIPlugIn();
}

UUID CSampleRdkMaterialWithChildrenRdkPlugIn::RhinoPlugInId(void) // Static.
{
	return ::SampleRdkMaterialAutoUIPlugIn().PlugInID();
}

UUID CSampleRdkMaterialWithChildrenRdkPlugIn::RdkPlugInId(void) // Static.
{
	return RhinoPlugInId();
}

UUID CSampleRdkMaterialWithChildrenRdkPlugIn::RenderEngineId(void) // Static.
{
	return RhinoPlugInId();
}

bool CSampleRdkMaterialWithChildrenRdkPlugIn::Initialize(void)
{
	// TODO: Initialize your plug-in. Return false on failure.

	return __super::Initialize();
}

void CSampleRdkMaterialWithChildrenRdkPlugIn::Uninitialize(void)
{
	// TODO: Do any necessary plug-in clean-up here.

	__super::Uninitialize();
}

void CSampleRdkMaterialWithChildrenRdkPlugIn::AbortRender(void)
{
}

void CSampleRdkMaterialWithChildrenRdkPlugIn::UiContentTypes(OUT ON_SimpleArray<UUID>& aTypes) const
{
	aTypes.Append(CSampleRdkMaterialWithChildren::UuidType());
}

void CSampleRdkMaterialWithChildrenRdkPlugIn::RegisterExtensions(void) const
{
	AddExtension(new CSampleRdkMaterialWithChildrenFactory);
}

bool CSampleRdkMaterialWithChildrenRdkPlugIn::Icon(OUT CRhinoDib& dibOut) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const int s = CRhinoDpi::IconSize(CRhinoDpi::IconType::SmallIcon);
	HICON hIcon = CRhinoDpi::LoadIcon(AfxGetInstanceHandle(), IDI_ICON1, s, s);
	ICONINFO info = { 0 };
	if (!::GetIconInfo(hIcon, &info))
		return false;

	dibOut.SetBitmap(info.hbmColor);

	::DeleteObject(info.hbmColor);
	::DeleteObject(info.hbmMask);

	return true;
}

bool CSampleRdkMaterialWithChildrenRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality,
                                                      const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify,
                                                      OUT CRhinoDib& dibOut)
{
	if (nullptr == pSceneServer)
		return false;

	// We only support material previews.
	if (pSceneServer->ContentKind() != CRhRdkContent::Kinds::Material)
		return false;

	// Set up a dib of the required size.
	CRhinoDib dib(sizeImage.cx, sizeImage.cy, 32);

	// Just get the first object from the scene server.
	auto pObject = pSceneServer->NextObject();
	if (nullptr == pObject)
		return false;

	// Get the object's material.
	const auto pMaterial = pObject->Material();
	if (nullptr == pMaterial)
		return false;

	// Get a simulation of the material and set the dib to the diffuse color.
	// In a real-world scenario we would actually render all the objects properly using
	// all the scene server information (geometry, lights, environment etc).
	const auto& mat = pMaterial->SimulatedMaterial(CRhRdkTexture::TextureGeneration::Allow);
	dib.Clear(mat.Diffuse());

	// Output the result.
	dibOut = dib;

	return true;
}

bool CSampleRdkMaterialWithChildrenRdkPlugIn::SupportsFeature(const UUID&) const
{
	// Support all features.
	return true;
}
