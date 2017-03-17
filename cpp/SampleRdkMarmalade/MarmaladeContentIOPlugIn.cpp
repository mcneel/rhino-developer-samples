
#include "stdafx.h"
#include "MarmaladeContentIOPlugIn.h"
#include "MarmaladeRdkPlugIn.h"

UUID CMarmaladeContentIOPlugIn::RdkPlugInId(void) const
{
	return MarmaladePlugIn().RdkPlugIn().PlugInId();
}

bool CMarmaladeContentIOPlugIn::Save(const wchar_t* wszFilename, const CRhRdkContent& content, const IRhRdkPreviewSceneServer* pSceneServer) const
{
	// Just do simple content saving; you could do something more complicated.

	return content.SaveToLibraryFile(wszFilename, pSceneServer);
}
