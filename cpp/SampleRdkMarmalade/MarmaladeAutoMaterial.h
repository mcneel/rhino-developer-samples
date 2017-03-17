
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
	virtual bool Initialize(void);
	virtual UUID TypeId(void) const;
	virtual ON_wString TypeName(void) const;
	virtual ON_wString TypeDescription(void) const;
	virtual ON_wString InternalName(void) const;
	virtual void AddUISections(void);
	virtual void AddAutoParameters(IRhRdkParamBlock& paramBlock, int iId); /*const*/
	virtual void GetAutoParameters(const IRhRdkParamBlock& paramBlock, int iId);
	virtual eParamSerialMethod ParameterSerializationMethod(void) const { return get_set; }
	virtual bool GetParameters(const IRhRdk_XMLSection& section, eGetParamsContext context);
	virtual bool SetParameters(IRhRdk_XMLSection& section, eSetParamsContext context) const;
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName) const;
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue, eChangeContext cc);
	virtual UUID RenderEngineId(void) const;
	virtual UUID PlugInId(void) const;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const;
	virtual DWORD BitFlags(void) const;
	virtual const wchar_t* Category(void) const;
	virtual bool IsFactoryProductAcceptableAsChild(const IRhRdkContentFactory* pFactory, const wchar_t* wszChildSlotName) const;

	virtual bool GetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraRequirementName,
	                                          CRhRdkVariant& vValue) const;

	virtual bool SetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraRequirementName,
	                                          const CRhRdkVariant& vValue, eSetContext sc);

	virtual CChildSlotIterator* NewChildSlotIterator(eCSI_Context context) const;

public: // Overrides from CRhRdkMaterial.
	virtual void SimulateMaterial(ON_Material& mat, bool bForDataOnlyDec) const;

protected:
	bool IsTexturingSupported(void) const;

private:
	CMarmaladeParamBlock m_ParamBlock;
	const CMarmaladeShader* m_pShader;
};
