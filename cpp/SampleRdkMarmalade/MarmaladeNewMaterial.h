
#pragma once

#include "MarmaladeActualShaders.h"

/*	This is the new Marmalade Material which demostrates the recommended way to make a material.

*/
class CMarmaladeNewMaterialFactory : public CRhRdkMaterialFactory
{
protected:
	virtual CRhRdkMaterial* NewMaterial(void) const;
};

class CMarmaladeNewMaterial : public CRhRdkMaterial
{
public:
	CMarmaladeNewMaterial();
	virtual ~CMarmaladeNewMaterial();

public: // Overrides from CRhRdkContent.
	virtual UUID TypeId(void) const override;
	virtual ON_wString TypeName(void) const override;
	virtual ON_wString TypeDescription(void) const override;
	virtual ON_wString InternalName(void) const override;
	virtual void AddUISections(IRhRdkExpandableContentUI& ui) override;
	virtual UUID RenderEngineId(void) const override;
	virtual UUID PlugInId(void) const override;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const override;
	virtual bool IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const override;
	virtual unsigned int BitFlags(void) const override;

public: // Overrides from CRhRdkMaterial.
	virtual void SimulateMaterial(ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSize, const CRhinoObject* pObject) const override;

private:
	CRhRdkTexturedContentField m_Color;
	CRhRdkContentField m_Trans;
	CRhRdkContentField m_IOR;
};
