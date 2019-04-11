
#pragma once

class CSampleRdkMaterialWithChildren : public CRhRdkMaterial
{
public:
	CSampleRdkMaterialWithChildren();

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

private:
	CRhRdkTexturedContentField m_diffuse_color;
};

class CSampleRdkMaterialWithChildrenFactory : public CRhRdkMaterialFactory
{
protected:
	virtual CRhRdkMaterial* NewMaterial(void) const override { return new CSampleRdkMaterialWithChildren; }
};
