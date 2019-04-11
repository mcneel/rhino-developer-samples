
#include "stdafx.h"
#include "SampleRdkAddRdkMaterialsRdkPlugIn.h"
#include "SampleRdkAddRdkMaterialsPlugIn.h"
#include "Resource.h"

CRhinoPlugIn& CSampleRdkAddRdkMaterialsRdkPlugIn::RhinoPlugIn(void) const
{
	return ::SampleRdkMaterialAutoUIPlugIn();
}

UUID CSampleRdkAddRdkMaterialsRdkPlugIn::RhinoPlugInId(void) // Static.
{
	return ::SampleRdkMaterialAutoUIPlugIn().PlugInID();
}

UUID CSampleRdkAddRdkMaterialsRdkPlugIn::RdkPlugInId(void) // Static.
{
	return RhinoPlugInId();
}

bool CSampleRdkAddRdkMaterialsRdkPlugIn::Initialize(void)
{
	// TODO: Initialize your plug-in. Return false on failure.

	return __super::Initialize();
}

void CSampleRdkAddRdkMaterialsRdkPlugIn::Uninitialize(void)
{
	// TODO: Do any necessary plug-in clean-up here.

	__super::Uninitialize();
}

bool CSampleRdkAddRdkMaterialsRdkPlugIn::Icon(OUT CRhinoDib& dibOut) const
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
