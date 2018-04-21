
#pragma once

class CMarmaladeAutoUITextureFactory : public CRhRdkTextureFactory
{
public:
	CMarmaladeAutoUITextureFactory();

protected:
	virtual CRhRdkTexture* NewTexture(void) const;
};

class CMarmaladeAutoUITexture : public CRhRdkTexture
{
public:
	CMarmaladeAutoUITexture();
	virtual ~CMarmaladeAutoUITexture();

public: // Overrides from CRhRdkContent.
	virtual UUID TypeId(void) const override;
	virtual ON_wString TypeName(void) const override;
	virtual ON_wString TypeDescription(void) const override;
	virtual ON_wString InternalName(void) const override;
	virtual void AddUISections(IRhRdkExpandableContentUI& ui) override;
	virtual void AddAutoParameters(IRhRdkParamBlock& paramBlock, int iId) const override;
	virtual void GetAutoParameters(const IRhRdkParamBlock& paramBlock, int iId) override;
	virtual ParamSerialMethod ParameterSerializationMethod(void) const override { return ParamSerialMethod ::GetSet; }
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context) override;
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const override;
	virtual UUID RenderEngineId(void) const override;
	virtual UUID PlugInId(void) const override;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const override;
	virtual bool IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const override;
	virtual unsigned int BitFlags(void) const override;
	virtual const wchar_t* Category(void) const override { return RDK_CAT_PROCEDURAL_2D; }
	virtual IRhRdkTextureEvaluator* NewTextureEvaluator(IRhRdkTextureEvaluator::CEvalFlags ef) const override;
	virtual bool IsImageBased(void) const override { return false; }

private:
	class Evaluator;
	CRhRdkColor m_color1;
	CRhRdkColor m_color2;
};
