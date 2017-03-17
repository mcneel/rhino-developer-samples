
#pragma once

class CSampleRdkMaterialIOPlugIn : public CRhRdkContentIOPlugIn
{
public:
	virtual UUID PlugInId(void) const override;
	virtual UUID RdkPlugInId(void) const override;
	virtual const wchar_t* FileExtension(void) const override { return L"sample"; }
	virtual const wchar_t* Description(void) const override { return L"Sample files"; }
	virtual bool CanLoad(void) const override { return true; }
	virtual bool CanSave(void) const override { return true; }
	virtual CRhRdkContent* Load(const CRhinoDoc* pDoc, const wchar_t* wszFilename, const wchar_t* wszKind) const override;
	virtual bool Save(const wchar_t* wszFilename, const CRhRdkContent& content, const IRhRdkPreviewSceneServer* pSceneServer) const override;
	virtual const wchar_t* SupportedKinds(void) const override;
	virtual void* EVF(const wchar_t*, void*) override { return nullptr; }

protected:
	bool Parse(char*& p, const char* pSubString, bool bExpect=true) const;
	bool ParseBool(char*& p, bool& value) const;
	bool ParseDouble(char*& p, double& value) const;
	bool ParseFloat(char*& p, float& value) const;
	bool ParseFloatList(char*& p, float* a, int num) const;
	CRhRdkVariant GetValue(char* p) const;

private:
	mutable int m_iLine = 0;
	mutable ON_wString m_sFilename;
};
