
#pragma once

#include "MarmaladeShader.h"

class CMarmaladeAutoMaterialFactory : public CRhRdkMaterialFactory
{
public:
	CMarmaladeAutoMaterialFactory(const UUID& uuidShader);

protected:
	virtual CRhRdkMaterial* NewMaterial(void) const;

private:
	UUID m_uuid;
};

class CMarmaladeAutoMaterial : public CRhRdkMaterial
{
public:
	CMarmaladeAutoMaterial(const UUID& uuidShader);

	bool TextureOn(void) const;
	void SetTextureOn(bool b, eChangeContext cc);

	double TextureAmount(void) const;
	void SetTextureAmount(double d, eChangeContext cc);

public: // Overrides from CRhRdkContent.
	virtual bool Initialize(void) override;
	virtual UUID TypeId(void) const override;
	virtual ON_wString TypeName(void) const override;
	virtual ON_wString TypeDescription(void) const override;
	virtual ON_wString InternalName(void) const override;
	virtual void AddUISections(IRhRdkExpandableContentUI& ui) override;
	virtual void AddAutoParameters(IRhRdkParamBlock& paramBlock, int iId) const override;
	virtual void GetAutoParameters(const IRhRdkParamBlock& paramBlock, int iId) override;
	virtual ParamSerialMethod ParameterSerializationMethod(void) const override { return ParamSerialMethod::GetSet; }
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context) override;
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const override;
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName) const override;
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue) override;
	virtual UUID RenderEngineId(void) const override;
	virtual UUID PlugInId(void) const override;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const override;
	virtual unsigned int BitFlags(void) const override;
	virtual const wchar_t* Category(void) const override;
	virtual bool IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const override;

	virtual bool GetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraRequirementName,
	                                          CRhRdkVariant& vValue) const override;

	virtual bool SetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraRequirementName,
	                                          const CRhRdkVariant& vValue, SetContext sc) override;

	virtual CChildSlotIterator* NewChildSlotIterator(CSIContext context) const override;

public: // Overrides from CRhRdkMaterial.
	virtual void SimulateMaterial(ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSize, const CRhinoObject* pObject) const override;

protected:
	bool IsTexturingSupported(void) const;

private:
	CMarmaladeParamBlock m_ParamBlock;
	const CMarmaladeShader* m_pShader;
};
