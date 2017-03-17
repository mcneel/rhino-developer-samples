
#pragma once

class CMarmaladeContentIOPlugIn : public IRhRdkContentIOPlugIn
{
public:
	virtual UUID RdkPlugInId(void) const;
	virtual void DeleteThis(void) { delete this; }
	virtual const wchar_t* FileExtension(void) const { return L".marm"; }
	virtual const wchar_t* Description(void) const { return L"Marmalade Files"; }
	virtual bool CanLoad(void) const { return false; }
	virtual bool CanSave(void) const { return true; }
	virtual CRhRdkContent* Load(const wchar_t* wszFilename) const { return NULL; }
	virtual bool Save(const wchar_t* wszFilename, const CRhRdkContent& content, const IRhRdkPreviewSceneServer* pSceneServer) const;
	virtual const wchar_t* Kind(void) const { return RDK_KIND_MATERIAL; }
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) { return NULL; }
};
