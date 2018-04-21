
#pragma once

class CMarmaladeContentIOPlugIn : public CRhRdkContentIOPlugIn
{
public:
	virtual UUID RdkPlugInId(void) const override;
	virtual const wchar_t* FileExtension(void) const override { return L".marm"; }
	virtual const wchar_t* Description(void) const override { return L"Marmalade Files"; }
	virtual bool CanLoad(void) const override { return false; }
	virtual bool CanSave(void) const override { return true; }
	virtual CRhRdkContent* Load(const CRhinoDoc*, const wchar_t*, CRhRdkContent::Kinds) const override { return nullptr; }
	virtual bool Save(const wchar_t* wszFilename, const CRhRdkContent& content, const IRhRdkPreviewSceneServer* pSceneServer) const override;
	virtual CRhRdkContentKindList SupportedKinds(void) const override { return CRhRdkContent::Kinds::Material; }
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) override { return nullptr; }
};
