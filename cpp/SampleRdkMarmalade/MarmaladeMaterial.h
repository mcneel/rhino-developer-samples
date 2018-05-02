
#pragma once

#include "MarmaladeActualShaders.h"

/*	This is the original Marmalade Material and as such it works in the old-fashioned way.
	It has been retained for completeness and so that it can be compared with the new material,
	but it is not recomended to use this method of making a material. Please see MarmaladeNewMaterial.cpp
	for the recommended example.

*/
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

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Having explicit getters and setters is the old-style way of doing things.
	// The recommended way for new designs is to use content fields (see CMarmaladeNewMaterial).
	// The fields themselves would then be called on Value() and SetValue() and the following
	// functions would not be needed.
	CRhRdkColor Color(void) const;
	void SetColor(const CRhRdkColor& col);
	double Transparency(void) const;
	void SetTransparency(double d);
	double IOR(void) const;
	void SetIOR(double d);
	bool TextureOn(void) const;
	void SetTextureOn(bool b);
	double TextureAmount(void) const;
	void SetTextureAmount(double d);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

public: // Overrides from CRhRdkContent.
	virtual UUID TypeId(void) const override;
	virtual ON_wString TypeName(void) const override;
	virtual ON_wString TypeDescription(void) const override;
	virtual ON_wString InternalName(void) const override;
	virtual void AddUISections(IRhRdkExpandableContentUI& ui) override;
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName) const override;
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue) override;
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
