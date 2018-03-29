
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
	virtual UUID TypeId(void) const;
	virtual ON_wString TypeName(void) const;
	virtual ON_wString TypeDescription(void) const;
	virtual ON_wString InternalName(void) const;
	virtual void AddUISections(void);
	virtual eParamSerialMethod ParameterSerializationMethod(void) const { return get_set; }
	virtual bool GetParameters(const IRhRdk_XMLSection& section, eGetParamsContext context);
	virtual bool SetParameters(IRhRdk_XMLSection& section, eSetParamsContext context) const;
	virtual UUID RenderEngineId(void) const;
	virtual UUID PlugInId(void) const;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const;
	virtual bool IsFactoryProductAcceptableAsChild(const IRhRdkContentFactory* pFactory, const wchar_t* wszChildSlotName) const;
	virtual DWORD BitFlags(void) const;

public: // Overrides from CRhRdkMaterial.
	virtual void SimulateMaterial(ON_Material& mat, bool bForDataOnlyDec) const;

private:
	// We keep a shader in the class as an easy way to  make this material represent
	// the values in the shader. This may not be a good idea for your own plug-in depending
	// on whether shaders are easy to embed in objects.
	CMarmaladeOrangeShader m_shader;
};
