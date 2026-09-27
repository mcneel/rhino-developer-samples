
#include "stdafx.h"
#include "SampleRdkRemoveUnusedContentRdkPlugIn.h"
#include "SampleRdkRemoveUnusedContentPlugIn.h"
#include "Resource.h"

CRhinoPlugIn& CSampleRdkRemoveUnusedContentRdkPlugIn::RhinoPlugIn(void) const
{
	return ::SampleRdkRemoveUnusedContentPlugIn();
}

UUID CSampleRdkRemoveUnusedContentRdkPlugIn::RhinoPlugInId(void) // Static.
{
	return ::SampleRdkRemoveUnusedContentPlugIn().PlugInID();
}

UUID CSampleRdkRemoveUnusedContentRdkPlugIn::RdkPlugInId(void) // Static.
{
	return RhinoPlugInId();
}

bool CSampleRdkRemoveUnusedContentRdkPlugIn::Initialize(void)
{
	// TODO: Initialize your plug-in. Return false on failure.

	return CRhRdkPlugIn::Initialize();
}

void CSampleRdkRemoveUnusedContentRdkPlugIn::Uninitialize(void)
{
	// TODO: Do any necessary plug-in clean-up here.

	CRhRdkPlugIn::Uninitialize();
}

bool CSampleRdkRemoveUnusedContentRdkPlugIn::Icon(OUT CRhinoDib& dibOut) const
{
#if defined(ON_RUNTIME_WIN)
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
#else
	// The icon comes out of the plug-in's Windows resources, and a Mac bundle
	// has none.  Returning false means the plug-in has no icon, which Rhino
	// handles; to supply one, read an image out of the bundle's Resources and
	// hand it to dibOut.
	UNREFERENCED_PARAMETER(dibOut);
	return false;
#endif
}
