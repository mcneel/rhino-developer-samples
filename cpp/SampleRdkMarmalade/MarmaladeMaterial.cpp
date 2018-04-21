
#include "stdafx.h"
#include "MarmaladeMaterial.h"
#include "MarmaladeMaterialSection.h"
#include "MarmaladePlugIn.h"

CRhRdkMaterial* CMarmaladeMaterialFactory::NewMaterial(void) const
{
	return new CMarmaladeMaterial;
}

static const wchar_t* wszColor         = L"color";
static const wchar_t* wszTransparency  = L"transparency";
static const wchar_t* wszIOR           = L"ior";
static const wchar_t* wszTextureOn     = L"texture-on";
static const wchar_t* wszTextureAmount = L"texture-amount";

CMarmaladeMaterial::CMarmaladeMaterial()
{
}

CMarmaladeMaterial::~CMarmaladeMaterial()
{
}

COLORREF CMarmaladeMaterial::Color(void) const
{
	const CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszColor);
	ASSERT(NULL != pParam);

	return pParam->m_vValue.AsRdkColor().ColorRef();
}

void CMarmaladeMaterial::SetColor(COLORREF col, CRhRdkContent::eChangeContext cc)
{
	CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszColor);
	ASSERT(NULL != pParam);

	if (pParam->m_vValue.AsRdkColor() != CRhRdkColor(col))
	{
		pParam->m_vValue = ON_Color(col);

		if (CRhRdkContent::ccProgram != cc)
		{
			Changed(cc);
		}
	}
}

double CMarmaladeMaterial::Transparency(void) const
{
	const CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszTransparency);
	ASSERT(NULL != pParam);

	return pParam->m_vValue;
}

void CMarmaladeMaterial::SetTransparency(double d, CRhRdkContent::eChangeContext cc)
{
	CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszTransparency);
	ASSERT(NULL != pParam);

	if (pParam->m_vValue.AsDouble() != d)
	{
		pParam->m_vValue = d;
		Changed(cc);
	}
}

double CMarmaladeMaterial::IOR(void) const
{
	const CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszIOR);
	ASSERT(NULL != pParam);

	return pParam->m_vValue;
}

void CMarmaladeMaterial::SetIOR(double d, CRhRdkContent::eChangeContext cc)
{
	CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszIOR);
	ASSERT(NULL != pParam);

	if (pParam->m_vValue.AsDouble() != d)
	{
		pParam->m_vValue = d;
		Changed(cc);
	}
}

bool CMarmaladeMaterial::TextureOn(void) const
{
	const CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszTextureOn);
	ASSERT(NULL != pParam);

	return pParam->m_vValue;
}

void CMarmaladeMaterial::SetTextureOn(bool b, CRhRdkContent::eChangeContext cc)
{
	CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszTextureOn);
	ASSERT(NULL != pParam);

	if (pParam->m_vValue.AsBool() != b)
	{
		pParam->m_vValue = b;
		Changed(cc);
	}
}

double CMarmaladeMaterial::TextureAmount(void) const
{
	const CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszTextureAmount);
	ASSERT(NULL != pParam);

	return pParam->m_vValue;
}

void CMarmaladeMaterial::SetTextureAmount(double d, CRhRdkContent::eChangeContext cc)
{
	CMarmaladeParam* pParam = m_shader.ParamBlock().FindParameter(wszTextureAmount);
	ASSERT(NULL != pParam);

	if (pParam->m_vValue.AsDouble() != d)
	{
		pParam->m_vValue = d;
		Changed(cc);
	}
}

UUID CMarmaladeMaterial::RenderEngineId(void) const
{
	return CMarmaladePlugIn::ID();
}

UUID CMarmaladeMaterial::PlugInId(void) const
{
	return CMarmaladePlugIn::ID();
}

UUID CMarmaladeMaterial::TypeId(void) const
{
	//**********************************************************************
	// Generated for this class using GUIDGEN  *** Don't reuse this GUID ***
	// {69AFA258-CAAE-4f75-BAFA-3BEDE13CA7E8}
	static const GUID uuidTypeId =
	{
		0x69afa258, 0xcaae, 0x4f75, { 0xba, 0xfa, 0x3b, 0xed, 0xe1, 0x3c, 0xa7, 0xe8 }
	};
	//**********************************************************************

	return uuidTypeId;
}

ON_wString CMarmaladeMaterial::TypeName(void) const
{
	return L"Marmalade Material";
}

ON_wString CMarmaladeMaterial::TypeDescription(void) const
{
	return "Marmalade Material";
}

ON_wString CMarmaladeMaterial::InternalName(void) const
{
	return L"MarmaladeMaterial";
}

void CMarmaladeMaterial::SimulateMaterial(ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSize, const CRhinoObject* pObject) const
{
	mat.m_diffuse = Color();
	mat.SetTransparency(Transparency());
	mat.m_index_of_refraction = IOR();

	const CRhRdkContent* pChild = NULL;
	if (TextureOn() && (TextureAmount() > 0.0) && (NULL != (pChild = FindChild(L"ColorChildSlot"))))
	{
		// IsFactoryProductAcceptableAsChild should ensure that the child is a RDK_KIND_TEXTURE
		// but it never hurts to check.
		if (pChild->IsKind(Kinds::Texture))
		{
			const CRhRdkTexture* pTexture = static_cast<const CRhRdkTexture*>(pChild);

			CRhRdkSimulatedTexture onTexture;
			pTexture->SimulateTexture(onTexture, tg);

			mat.AddTexture(onTexture.Filename(), ON_Texture::TYPE::bitmap_texture);

			if (1 == mat.m_textures.Count())
			{
				ON_Texture& tex = mat.m_textures[0];
				tex.m_bOn = TextureOn();
				tex.m_magfilter = tex.m_minfilter = ON_Texture::FILTER::linear_filter;
				tex.m_blend_constant_A = TextureAmount();
				tex.m_mode = (tex.m_blend_constant_A < 1.0) ? ON_Texture::MODE::blend_texture : ON_Texture::MODE::decal_texture;
			}
		}
	}
}

void CMarmaladeMaterial::AddUISections(IRhRdkExpandableContentUI& ui)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ui.AddSection(new CMarmaladeMaterialSection);

	CRhRdkMaterial::AddUISections();
}

bool CMarmaladeMaterial::ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context)
{
	CRhRdkVariant v;

	if (section.GetParam(wszColor, v))
		SetColor(v.AsOnColor(), CRhRdkContent::ccProgram);

	if (section.GetParam(wszTransparency, v))
		SetTransparency(v, CRhRdkContent::ccProgram);

	if (section.GetParam(wszIOR, v))
		SetIOR(v, CRhRdkContent::ccProgram);

	if (section.GetParam(wszTextureOn, v))
		SetTextureOn(v, CRhRdkContent::ccProgram);

	if (section.GetParam(wszTextureAmount, v))
		SetTextureAmount(v, CRhRdkContent::ccProgram);

	return true;
}

bool CMarmaladeMaterial::WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const
{
	section.SetParam(wszColor, ON_Color(Color()));
	section.SetParam(wszTransparency,   Transparency());
	section.SetParam(wszIOR,            IOR());
	section.SetParam(wszTextureOn,      TextureOn());
	section.SetParam(wszTextureAmount,  TextureAmount());

	return true;
}

void* CMarmaladeMaterial::GetShader(const UUID& uuidRenderEngine, void* pvData) const
{
	if (!IsCompatible(uuidRenderEngine))
		return NULL;

	CMarmaladeShader* pShader = m_shader.Clone();
	pShader->ParamBlock() = m_shader.ParamBlock();

	return (void*)pShader;
}

bool CMarmaladeMaterial::IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const
{
	if (f.Kind() == Kinds::Texture)
		return true; // Factory produces textures.

	return false; // Factory produces something "unpalatable".
}

unsigned int CMarmaladeMaterial::BitFlags(void) const
{
	return CRhRdkMaterial::BitFlags() & ~bfTextureSummary; // No texture summary required.
	                               // |  bfSharedUI;       // Shared UI is mandatory now.
}
