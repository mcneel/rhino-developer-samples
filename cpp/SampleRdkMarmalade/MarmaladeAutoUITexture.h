
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
	virtual UUID RenderEngineId(void) const;
	virtual UUID PlugInId(void) const;
	virtual void* GetShader(const UUID& uuidRenderEngine, void* pvData) const;
	virtual bool IsFactoryProductAcceptableAsChild(const IRhRdkContentFactory* pFactory, const wchar_t* wszChildSlotName) const;
	virtual DWORD BitFlags(void) const;
	virtual const wchar_t* Category(void) const { return RDK_CAT_PROCEDURAL_2D; }
	virtual IRhRdkTextureEvaluator* NewTextureEvaluator(void) const;
	virtual bool IsImageBased(void) const { return false; }

public: // Overrides from CRhRdkTexture.
	virtual void SimulateTexture(CRhRdkSimulatedTexture& mat, bool bForDataOnlyDec) const;

private:
	class Evaluator;
	CRhRdkColor m_color1;
	CRhRdkColor m_color2;
};
