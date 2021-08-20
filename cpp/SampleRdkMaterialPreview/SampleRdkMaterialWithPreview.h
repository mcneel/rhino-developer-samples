
#pragma once

#include "Resource.h"

#define DIFFUSE_PARAM_NAME  L"diffuse"

class CSampleRdkMaterialWithPreview : public CRhRdkMaterial
{
public:
	CSampleRdkMaterialWithPreview();

	static UUID UuidType(void);

public:
	virtual bool Initialize(void) override;
	virtual UUID TypeId(void) const override;
	virtual ON_wString TypeName(void) const override;
	virtual ON_wString TypeDescription(void) const override;
	virtual ON_wString InternalName(void) const override;
	virtual void AddUISections(IRhRdkExpandableContentUI& ui) override;
	virtual UUID RenderEngineId(void) const override;
	virtual UUID PlugInId(void) const override;
	virtual unsigned int BitFlags(void) const override;
	virtual ParamSerialMethod ParameterSerializationMethod(void) const override;
	virtual void AddAutoParameters(IRhRdkParamBlock& paramBlock, int sectionId) const override;
	virtual void GetAutoParameters(const IRhRdkParamBlock& paramBlock, int sectionId) override;
	virtual void SimulateMaterial(ON_Material& matOut, CRhRdkTexture::TextureGeneration tg, int iSimulatedTextureSize, const CRhinoObject* pObject) const override;
	virtual bool CreatePreview(CRhRdkRenderPlugIn& plugIn, const ON_2iSize& sizeImage, RhRdkPreviewQuality qual,
	                           const IRhRdkPreviewSceneServer* pSceneServer_CanBeNull, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) const override;

private:
	CRhRdkTexturedContentField m_diffuse_color;
};

class CSampleRdkMaterialWithPreviewFactory : public CRhRdkMaterialFactory
{
protected:
	virtual CRhRdkMaterial* NewMaterial(void) const override { return new CSampleRdkMaterialWithPreview; }
};

class CCustomContentSection : public CRhRdkMaterialUISection_MFC, protected CRhRdkEventWatcher
{
public:
	CCustomContentSection();

private:
	enum { IDD = IDD_MATERIAL_SECTION };
	CStatic m_placeholder;

protected:
	virtual BOOL OnInitDialog() override;
	virtual void DoDataExchange(CDataExchange* pDX) override;
	virtual void DisplayData(void) override;
	virtual void EnableDisableControls(void) override { }
	virtual UUID Uuid(void) const override;
	virtual ON_wString Caption(bool bAlwaysEnglish) const override;
	virtual AFX_MODULE_STATE* GetModuleState(void) const override { return AfxGetStaticModuleState(); }
	virtual unsigned int GetIDD(void) const override { return IDD; }
	virtual bool SupportsVaries(const CRhRdkContentArray& aContent) const override { return true; }
	virtual bool IsHidden(void) const override { return false; }

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CRhinoDib m_dib;
};
