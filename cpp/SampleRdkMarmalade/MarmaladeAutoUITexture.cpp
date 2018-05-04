
#include "stdafx.h"
#include "MarmaladeAutoUITexture.h"
#include "MarmaladePlugIn.h"

CMarmaladeAutoUITextureFactory::CMarmaladeAutoUITextureFactory()
{
}

CRhRdkTexture* CMarmaladeAutoUITextureFactory::NewTexture(void) const
{
	return new CMarmaladeAutoUITexture;
}

static const wchar_t* wszColor1 = L"color-1";
static const wchar_t* wszColor2 = L"color-2";

class CMarmaladeAutoUITexture::Evaluator : public CRhRdkTextureEvaluator
{
public:
	Evaluator(CEvalFlags ef, const CRhRdkColor& col1, const CRhRdkColor& col2, const ON_Xform& xform);

	virtual void DeleteThis(void) { delete this; }
	virtual bool GetColor(const ON_3dPoint& uvw, const ON_3dVector& duvwdx,
		                  const ON_3dVector& duvwdy, CRhRdkColor& colOut, void* pvData = nullptr) const;
	virtual void* EVF(const wchar_t* wszFunc, void* pvData) { return nullptr; }

private:
	CRhRdkColor m_color1;
	CRhRdkColor m_color2;
	ON_Xform m_xform;
};

CMarmaladeAutoUITexture::Evaluator::Evaluator(CEvalFlags ef, const CRhRdkColor& col1, const CRhRdkColor& col2, const ON_Xform& xform)
	:
	CRhRdkTextureEvaluator(ef),
	m_color1(col1),
	m_color2(col2),
	m_xform (xform)
{
}

bool CMarmaladeAutoUITexture::Evaluator::GetColor(const ON_3dPoint& uvw, const ON_3dVector& duvwdx,
                                                  const ON_3dVector& duvwdy, CRhRdkColor& colOut, void* pvData) const
{
	// This produces a checker effect.
	const ON_3dVector uvwNew = m_xform * (ON_Xform(2) * uvw);
	const int sum = int(uvwNew.x + 10000.0) + int(uvwNew.y + 10000.0);
	colOut = (sum % 2) ? m_color1 : m_color2;

	return true;
}

//---------------------

CMarmaladeAutoUITexture::CMarmaladeAutoUITexture()
{
	m_color1 = RGB(255, 140, 0);
	m_color2 = RGB(255, 255, 255);
}

CMarmaladeAutoUITexture::~CMarmaladeAutoUITexture()
{
}

UUID CMarmaladeAutoUITexture::RenderEngineId(void) const
{
	return CMarmaladePlugIn::ID();
}

UUID CMarmaladeAutoUITexture::PlugInId(void) const
{
	return CMarmaladePlugIn::ID();
}

UUID CMarmaladeAutoUITexture::TypeId(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuidTypeId = { 0x5e33dc89, 0x8217, 0x4048, { 0xb1, 0xef, 0xf0, 0xeb, 0x4a, 0x8f, 0x03, 0xd2 } };
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuidTypeId;
}

ON_wString CMarmaladeAutoUITexture::TypeName(void) const
{
	return L"Marmalade Automatic UI Texture";
}

ON_wString CMarmaladeAutoUITexture::TypeDescription(void) const
{
	return L"Marmalade Automatic UI Texture";
}

ON_wString CMarmaladeAutoUITexture::InternalName(void) const
{
	return L"MarmaladeAutoUITexture";
}

void CMarmaladeAutoUITexture::AddUISections(IRhRdkExpandableContentUI& ui)
{
	const auto* wsz = L"Marmalade parameters";
	AddAutomaticUISection(ui, wsz, wsz);

	CRhRdkTexture::AddUISections(ui);
}

unsigned int CMarmaladeAutoUITexture::BitFlags(void) const
{
	return CRhRdkTexture::BitFlags() & ~bfTextureSummary; // No texture summary required.
	                              // |  bfSharedUI;       // Shared UI is mandatory now.
}

void CMarmaladeAutoUITexture::AddAutoParameters(IRhRdkParamBlock& paramBlock, int iId) const
{
	// It is through this method that the values in your content get transferred into the automatic UI.
	paramBlock.Add(wszColor1, L"", L"Color 1", m_color1, CRhRdkVariant(), CRhRdkVariant());
	paramBlock.Add(wszColor2, L"", L"Color 2", m_color2, CRhRdkVariant(), CRhRdkVariant());
}

void CMarmaladeAutoUITexture::GetAutoParameters(const IRhRdkParamBlock& paramBlock, int iId)
{
	// This method is called when something changes in the automatic UI as a result
	// of the user clicking on a control. It can be called from more than one UI section
	// even a section that doesn't contain our color parameters. This happens for example
	// with the Local Mapping section whose parameters are managed by the base class
	// CRhRdkTexture. Because of this, it is important to make sure that you don't
	// set your members if IRhRdkParamBlock::Get() returns false.

	CRhRdkVariant vValue;

	if (paramBlock.Get(wszColor1, vValue))
	{
		m_color1 = vValue.AsRdkColor();
	}

	if (paramBlock.Get(wszColor2, vValue))
	{
		m_color2 = vValue.AsRdkColor();
	}
}

bool CMarmaladeAutoUITexture::ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context)
{
	CRhRdkVariant v;

	if (section.GetParam(wszColor1, v))
		m_color1 = v.AsRdkColor();

	if (section.GetParam(wszColor2, v))
		m_color2 = v.AsRdkColor();

	return __super::ReadParametersFromSection(section, context);
}

bool CMarmaladeAutoUITexture::WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const
{
	section.SetParam(wszColor1, m_color1);
	section.SetParam(wszColor2, m_color2);

	return __super::WriteParametersToSection(section, context);
}

void CMarmaladeAutoUITexture::SimulateTexture(CRhRdkSimulatedTexture& texOut, CRhRdkTexture::TextureGeneration tg, int iSimulatedTextureSize, const CRhinoObject * pObject) const
{
	__super::SimulateTexture(texOut, tg, iSimulatedTextureSize, pObject);
}

void* CMarmaladeAutoUITexture::GetShader(const UUID& uuidRenderEngine, void* pvData) const
{
	return nullptr;
}

bool CMarmaladeAutoUITexture::IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const
{
	if (f.Kind() == Kinds::Texture)
		return true; // Factory produces textures.

	return false; // Factory produces something "unpalatable".
}

IRhRdkTextureEvaluator* CMarmaladeAutoUITexture::NewTextureEvaluator(IRhRdkTextureEvaluator::CEvalFlags ef) const
{
	return new Evaluator(ef, m_color1, m_color2, LocalMappingTransform());
}
