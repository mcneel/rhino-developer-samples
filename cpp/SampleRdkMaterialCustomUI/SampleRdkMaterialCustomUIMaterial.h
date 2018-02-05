
#pragma once

class CSampleRdkMaterialCustomUIMaterial : public CRhRdkMaterial
{
public:
	CSampleRdkMaterialCustomUIMaterial();

	// CRhRdkContent overrides
	virtual UUID TypeId(void) const override;
	virtual ON_wString TypeName(void) const override;
	virtual ON_wString TypeDescription(void) const override;
	virtual ON_wString InternalName(void) const override;
	virtual UUID RenderEngineId(void) const override;
	virtual UUID PlugInId(void) const override;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const override;
	virtual bool IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& factory, const wchar_t* wszChildSlotName) const override;
	virtual unsigned int BitFlags(void) const override;
	virtual IRhRdkContentUI* CreateUI(IRhRdkContentEditor& e, const UUID& uuidUI, const CRhRdkContentArray& aInitialClients, void* pReserved) override;
	virtual CRhRdkVariant GetParameter(const wchar_t* wszName) const override;
	virtual bool SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue) override;

	// CRhRdkMaterial overrides.
	void SimulateMaterial(OUT ON_Material& mat, CRhRdkTexture::TextureGeneration tg = CRhRdkTexture::TextureGeneration::Allow,
	                      int iSimulatedTextureSize = -1, const CRhinoObject* pObject = nullptr) const override;

private:
	CRhRdkContentField m_color;
};

class CSampleRdkMaterialCustomUIMaterialFactory : public CRhRdkMaterialFactory
{
public:
	CSampleRdkMaterialCustomUIMaterialFactory() = default;
  
protected:
	virtual bool IsElevated(void) const override { return true; }
	virtual CRhRdkMaterial* NewMaterial(void) const override { return new CSampleRdkMaterialCustomUIMaterial; }
};
