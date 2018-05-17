
#include "stdafx.h"
#include "MarmaladeNewAutoUITexture.h"
#include "MarmaladeAutoUITexture.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeActualShaders.h"

// This is similar to CMarmaladeAutoUITexture but it uses content fields to store
// the texture's colors. This makes it a bit easier to implement and is the
// recommended way to create custom textures.

CMarmaladeNewAutoUITextureFactory::CMarmaladeNewAutoUITextureFactory()
{
}

CRhRdkTexture* CMarmaladeNewAutoUITextureFactory::NewTexture(void) const
{
	return new CMarmaladeNewAutoUITexture;
}

CMarmaladeNewAutoUITexture::CMarmaladeNewAutoUITexture()
	:
	m_color1(*this, MARM_TEXTURE_COLOR_1, L"Color1", L"Color1"),
	m_color2(*this, MARM_TEXTURE_COLOR_2, L"Color2", L"Color2")
{
	m_color1 = CRhRdkColor(255, 140, 0);
	m_color2 = CRhRdkColor(255, 255, 255);
}

CMarmaladeNewAutoUITexture::~CMarmaladeNewAutoUITexture()
{
}

UUID CMarmaladeNewAutoUITexture::RenderEngineId(void) const
{
	return CMarmaladePlugIn::ID();
}

UUID CMarmaladeNewAutoUITexture::PlugInId(void) const
{
	return CMarmaladePlugIn::ID();
}

UUID CMarmaladeNewAutoUITexture::TypeId(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuidTypeId = { 0x5313b92f, 0x9357, 0x443c, { 0x95, 0x5a, 0x43, 0xa1, 0x57, 0xcf, 0xe5, 0x13 } };
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuidTypeId;
}

ON_wString CMarmaladeNewAutoUITexture::TypeName(void) const
{
	return L"Marmalade New Automatic UI Texture";
}

ON_wString CMarmaladeNewAutoUITexture::TypeDescription(void) const
{
	return L"Marmalade New Automatic UI Texture";
}

ON_wString CMarmaladeNewAutoUITexture::InternalName(void) const
{
	return L"MarmaladeNewAutoUITexture";
}

void CMarmaladeNewAutoUITexture::AddUISections(IRhRdkExpandableContentUI& ui)
{
	const auto* wsz = L"Marmalade parameters";
	AddAutomaticUISection(ui, wsz, wsz);

	CRhRdkTexture::AddUISections(ui);
}

unsigned int CMarmaladeNewAutoUITexture::BitFlags(void) const
{
	auto flags = CRhRdkTexture::BitFlags(); // | bfSharedUI;       // Shared UI is mandatory now.

	flags &= ~bfTextureSummary; // No texture summary required.
	flags |=  bfFields;         // Use content fields.

	return flags;
}

void CMarmaladeNewAutoUITexture::AddAutoParameters(IRhRdkParamBlock& paramBlock, int id) const
{
	// Since we are using fields, we can just get the fields to do the necessary work.
	Fields().AddValuesToParamBlock(paramBlock, id);
}

void CMarmaladeNewAutoUITexture::GetAutoParameters(const IRhRdkParamBlock& paramBlock, int id)
{
	// This method is called when something changes in the automatic UI as a result
	// of the user clicking on a control. Since we are using fields, we can just get
	// the fields to do the necessary work.
	Fields().GetValuesFromParamBlock(paramBlock, id);
}

bool CMarmaladeNewAutoUITexture::IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const
{
	if (f.Kind() == Kinds::Texture)
		return true; // Factory produces textures.

	return false; // Factory produces something "unpalatable".
}

IRhRdkTextureEvaluator* CMarmaladeNewAutoUITexture::NewTextureEvaluator(IRhRdkTextureEvaluator::CEvalFlags ef) const
{
	return new CMarmaladeAutoUITexture::Evaluator(ef, m_color1, m_color2, LocalMappingTransform());
}
