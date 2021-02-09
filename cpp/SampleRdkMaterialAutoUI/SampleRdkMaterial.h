
#pragma once

class CSampleRdkMaterial : public CRhRdkMaterial
{
public:
	CSampleRdkMaterial();

	void AddDynamicField(const wchar_t* wszName, const CRhRdkVariant vValue);

	static UUID UuidType(void);

public:
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
	virtual bool IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const override;

private:
	CRhRdkTexturedContentField m_diffuse_color;
	CRhRdkTexturedContentField m_emission_color;
	CRhRdkContentField m_ambient_color;
	CRhRdkContentField m_glossiness;
	CRhRdkContentField m_description;
};

class CSampleRdkMaterialFactory : public CRhRdkMaterialFactory
{
protected:
	virtual bool IsElevated(void) const override { return true; }
	virtual CRhRdkMaterial* NewMaterial(void) const override { return new CSampleRdkMaterial; }
};
