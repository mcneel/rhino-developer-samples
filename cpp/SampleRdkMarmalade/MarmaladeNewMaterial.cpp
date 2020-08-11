
#include "stdafx.h"
#include "MarmaladeNewMaterial.h"
#include "MarmaladeNewMaterialSection.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeUtilities.h"

CRhRdkMaterial* CMarmaladeNewMaterialFactory::NewMaterial(void) const
{
	return new CMarmaladeNewMaterial;
}

CMarmaladeNewMaterial::CMarmaladeNewMaterial()
	:
	m_Color(*this, MARM_SHADER_COLOR,        L"Color",        L"Color"),
	m_Trans(*this, MARM_SHADER_TRANSPARENCY, L"Transparency", L"Transparency"),
	m_IOR  (*this, MARM_SHADER_IOR,          L"IOR",          L"IOR")
{
	// The textured field m_Color also includes the 'on' and 'amount'
	// state for texturing, so there are no fields for those values.
	m_Color.Initialize(CRhRdkColor::white);

	m_Trans = 0.0;
	m_IOR   = 1.0;
}

CMarmaladeNewMaterial::~CMarmaladeNewMaterial()
{
}

UUID CMarmaladeNewMaterial::RenderEngineId(void) const
{
	return CMarmaladePlugIn::ID();
}

UUID CMarmaladeNewMaterial::PlugInId(void) const
{
	return CMarmaladePlugIn::ID();
}

UUID CMarmaladeNewMaterial::TypeId(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {60AD776C-CB35-4026-8F90-6E391CFB1A3A}
		0x60ad776c, 0xcb35, 0x4026, { 0x8f, 0x90, 0x6e, 0x39, 0x1c, 0xfb, 0x1a, 0x3a }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

ON_wString CMarmaladeNewMaterial::TypeName(void) const
{
	return L"Marmalade New Material";
}

ON_wString CMarmaladeNewMaterial::TypeDescription(void) const
{
	return "New (V6) version of Marmalade Material";
}

ON_wString CMarmaladeNewMaterial::InternalName(void) const
{
	return L"MarmaladeNewMaterial";
}

void CMarmaladeNewMaterial::SimulateMaterial(ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSize, const CRhinoObject* pObject) const
{
	mat.m_diffuse = m_Color;
	mat.SetTransparency(m_Trans);
	mat.m_index_of_refraction = m_IOR;

	bool bTextureOn = m_Color.TextureOn();
	double dTextureAmount = m_Color.TextureDoubleAmount();

	const CRhRdkContent* pChild = nullptr;
	if (bTextureOn && (dTextureAmount > 0.0) && (nullptr != (pChild = FindChild(MARM_SHADER_COLOR_CSN))))
	{
		// IsFactoryProductAcceptableAsChild should ensure that the child is a texture.
		const auto* pTexture = dynamic_cast<const CRhRdkTexture*>(pChild);
		if (nullptr != pTexture)
		{
			CRhRdkSimulatedTexture onTexture;
			pTexture->SimulateTexture(onTexture, tg);
			mat.AddTexture(onTexture.Filename(), ON_Texture::TYPE::bitmap_texture);

			if (1 == mat.m_textures.Count())
			{
				auto& tex = mat.m_textures[0];
				tex.m_bOn = bTextureOn;
				tex.m_magfilter = tex.m_minfilter = ON_Texture::FILTER::linear_filter;
				tex.m_blend_constant_A = dTextureAmount;
				tex.m_mode = (tex.m_blend_constant_A < 1.0) ? ON_Texture::MODE::blend_texture : ON_Texture::MODE::decal_texture;
			}
		}
	}
}

void CMarmaladeNewMaterial::AddUISections(IRhRdkExpandableContentUI& ui)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ui.AddSection(new CMarmaladeNewMaterialSection);

	CRhRdkMaterial::AddUISections(ui);
}

void* CMarmaladeNewMaterial::GetShader(const UUID& uuidRenderEngine, void* pvData) const
{
	if (!IsCompatible(uuidRenderEngine))
		return nullptr;

	auto* pShader = new CMarmaladeOrangeShader;

	auto& paramBlock = pShader->ParamBlock();

	auto* pParam = paramBlock.FindParameter(MARM_SHADER_COLOR);
	ASSERT(nullptr != pParam);
	pParam->m_vValue = m_Color.Value();

	pParam = paramBlock.FindParameter(MARM_SHADER_TRANSPARENCY);
	ASSERT(nullptr != pParam);
	pParam->m_vValue = m_Trans.Value();

	pParam = paramBlock.FindParameter(MARM_SHADER_IOR);
	ASSERT(nullptr != pParam);
	pParam->m_vValue = m_IOR.Value();

	pParam = paramBlock.FindParameter(MARM_SHADER_TEXTURE_ON);
	ASSERT(nullptr != pParam);
	pParam->m_vValue = m_Color.TextureOn();

	pParam = paramBlock.FindParameter(MARM_SHADER_TEXTURE_AMOUNT);
	ASSERT(nullptr != pParam);
	pParam->m_vValue = m_Color.TextureDoubleAmount();

	return (void*)pShader;
}

bool CMarmaladeNewMaterial::IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* /*wszChildSlotName*/) const
{
	if (f.Kind() == Kinds::Texture)
		return true; // Factory produces textures.

	return false; // Factory produces something "unpalatable".
}

unsigned int CMarmaladeNewMaterial::BitFlags(void) const
{
	auto f = CRhRdkMaterial::BitFlags();

	f |=  bfFields;         // Use content fields.
	f &= ~bfTextureSummary; // No texture summary required.

	return f;
}
