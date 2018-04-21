
#pragma once

#include "MarmaladeActualShaders.h"

class CMarmaladeMaterialFactory : public CRhRdkMaterialFactory
{
protected:
	virtual CRhRdkMaterial* NewMaterial(void) const;
};

class CMarmaladeMaterial : public CRhRdkMaterial
{
public:
	CMarmaladeMaterial();
	virtual ~CMarmaladeMaterial();

	COLORREF Color(void) const;
	void SetColor(COLORREF col, CRhRdkContent::eChangeContext cc);

	double Transparency(void) const;
	void SetTransparency(double d, CRhRdkContent::eChangeContext cc);

	double IOR(void) const;
	void SetIOR(double d, CRhRdkContent::eChangeContext cc);

	bool TextureOn(void) const;
	void SetTextureOn(bool b, CRhRdkContent::eChangeContext cc);

	double TextureAmount(void) const;
	void SetTextureAmount(double d, CRhRdkContent::eChangeContext cc);

public: // Overrides from CRhRdkContent.
	virtual UUID TypeId(void) const override;
	virtual ON_wString TypeName(void) const override;
	virtual ON_wString TypeDescription(void) const override;
	virtual ON_wString InternalName(void) const override;
	virtual void AddUISections(IRhRdkExpandableContentUI& ui) override;
	virtual ParamSerialMethod ParameterSerializationMethod(void) const override { return ParamSerialMethod::GetSet; }
	virtual bool ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context) override;
	virtual bool WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const override;
	virtual UUID RenderEngineId(void) const override;
	virtual UUID PlugInId(void) const override;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const override;
	virtual bool IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const override;
	virtual unsigned int BitFlags(void) const override;

public: // Overrides from CRhRdkMaterial.
	virtual void SimulateMaterial(ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSize, const CRhinoObject* pObject) const override;

private:
	// We keep a shader in the class as an easy way to  make this material represent
	// the values in the shader. This may not be a good idea for your own plug-in depending
	// on whether shaders are easy to embed in objects.
	CMarmaladeOrangeShader m_shader;
};
