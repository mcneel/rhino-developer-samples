
#include "stdafx.h"
#include "MarmaladeMaterial.h"
#include "MarmaladeMaterialSection.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeUtilities.h"

CRhRdkMaterial* CMarmaladeMaterialFactory::NewMaterial(void) const
{
	return new CMarmaladeMaterial;
}

/*	This is the original Marmalade Material and as such it works in the old-fashioned way.
	It has been retained for completeness and so that it can be compared with the old (V5) version,
	but it is not recomended to use this method of making a material. Please see MarmaladeNewMaterial.cpp
	for the recommended example.

	This old-style material does not use content fields, so each parameter is handled explicitly for
	each operation. This is only retained for reference and is not recommended for new plug-in designs.
	Using content fields instead (see CMarmaladeNewMaterial) automates a great deal of the necessary
	behavior, thus making the plug-in smaller and easier to implement.

*/
CMarmaladeMaterial::CMarmaladeMaterial()
{
}

CMarmaladeMaterial::~CMarmaladeMaterial()
{
}

CRhRdkVariant CMarmaladeMaterial::GetParameter(const wchar_t* wszName) const
{
	// This function is usually called by the UI if it directly uses the selected contents
	// as CRhRdkContent. The alternative (which CMarmaladeMaterialSection uses) is to dynamic
	// cast the selected contents to CMarmaladeMaterial and call the Get methods directly.

	if (0 == _wcsicmp(wszName, wszColor))
	{
		return Color();
	}
	else
	if (0 == _wcsicmp(wszName, wszTransparency))
	{
		return Transparency();
	}
	else
	if (0 == _wcsicmp(wszName, wszIOR))
	{
		return IOR();
	}

	return __super::GetParameter(wszName);
}

bool CMarmaladeMaterial::SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue)
{
	// This function is usually called by the UI if it directly uses the selected contents
	// as CRhRdkContent. The alternative (which CMarmaladeMaterialSection uses) is to dynamic
	// cast the selected contents to CMarmaladeMaterial and call the Set methods directly.

	if (0 == _wcsicmp(wszName, wszColor))
	{
		SetColor(vValue);
		return true;
	}
	else
	if (0 == _wcsicmp(wszName, wszTransparency))
	{
		SetTransparency(vValue);
		return true;
	}
	else
	if (0 == _wcsicmp(wszName, wszIOR))
	{
		SetIOR(vValue);
		return true;
	}

	return __super::SetParameter(wszName, vValue);
}

CRhRdkColor CMarmaladeMaterial::Color(void) const
{
	const auto* pParam = m_shader.ParamBlock().FindParameter(wszColor);
	ASSERT(nullptr != pParam);

	return pParam->m_vValue.AsRdkColor();
}

void CMarmaladeMaterial::SetColor(const CRhRdkColor& col)
{
	auto* pParam = m_shader.ParamBlock().FindParameter(wszColor);
	ASSERT(nullptr != pParam);

	if (pParam->m_vValue.AsRdkColor() != col)
	{
		pParam->m_vValue = col;
		Changed();
	}
}

double CMarmaladeMaterial::Transparency(void) const
{
	const auto* pParam = m_shader.ParamBlock().FindParameter(wszTransparency);
	ASSERT(nullptr != pParam);

	return pParam->m_vValue;
}

void CMarmaladeMaterial::SetTransparency(double d)
{
	auto* pParam = m_shader.ParamBlock().FindParameter(wszTransparency);
	ASSERT(nullptr != pParam);

	if (pParam->m_vValue.AsDouble() != d)
	{
		pParam->m_vValue = d;
		Changed();
	}
}

double CMarmaladeMaterial::IOR(void) const
{
	const auto* pParam = m_shader.ParamBlock().FindParameter(wszIOR);
	ASSERT(nullptr != pParam);

	return pParam->m_vValue;
}

void CMarmaladeMaterial::SetIOR(double d)
{
	auto* pParam = m_shader.ParamBlock().FindParameter(wszIOR);
	ASSERT(nullptr != pParam);

	if (pParam->m_vValue.AsDouble() != d)
	{
		pParam->m_vValue = d;
		Changed();
	}
}

bool CMarmaladeMaterial::TextureOn(void) const
{
	const auto* pParam = m_shader.ParamBlock().FindParameter(wszTextureOn);
	ASSERT(nullptr != pParam);

	return pParam->m_vValue;
}

void CMarmaladeMaterial::SetTextureOn(bool b)
{
	auto* pParam = m_shader.ParamBlock().FindParameter(wszTextureOn);
	ASSERT(nullptr != pParam);

	if (pParam->m_vValue.AsBool() != b)
	{
		pParam->m_vValue = b;
		Changed();
	}
}

double CMarmaladeMaterial::TextureAmount(void) const
{
	const auto* pParam = m_shader.ParamBlock().FindParameter(wszTextureAmount);
	ASSERT(nullptr != pParam);

	return pParam->m_vValue;
}

void CMarmaladeMaterial::SetTextureAmount(double d)
{
	auto* pParam = m_shader.ParamBlock().FindParameter(wszTextureAmount);
	ASSERT(nullptr != pParam);

	if (pParam->m_vValue.AsDouble() != d)
	{
		pParam->m_vValue = d;
		Changed();
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
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		0x69afa258, 0xcaae, 0x4f75, { 0xba, 0xfa, 0x3b, 0xed, 0xe1, 0x3c, 0xa7, 0xe8 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
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

ON_wString CMarmaladeMaterial::ChildSlotNameFromParamName(const wchar_t* wszParamName) const
{
	// If for whatever reason you choose to make the child slot name different to the
	// parameter name it's associated with, you must override this method and convert
	// the parameter name to the child slot name. This material's 'color' parameter is
	// associated with the child slot called 'ColorChildSlot'.

	if (0 == _wcsicmp(wszParamName, wszColor))
		return wszColorChildSlot;

	return wszParamName;
}

ON_wString CMarmaladeMaterial::ParamNameFromChildSlotName(const wchar_t* wszChildSlotName) const
{
	// If for whatever reason you choose to make the child slot name different to the
	// parameter name it's associated with, you must override this method and convert
	// the child slot name to the parameter name. This material's 'color' parameter is
	// associated with the child slot called 'ColorChildSlot'.

	if (0 == _wcsicmp(wszChildSlotName, wszColorChildSlot))
		return wszColor;

	return wszChildSlotName;
}

void CMarmaladeMaterial::SimulateMaterial(ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSize, const CRhinoObject* pObject) const
{
	mat.m_diffuse = Color().OnColor();
	mat.SetTransparency(Transparency());
	mat.m_index_of_refraction = IOR();

	const CRhRdkContent* pChild = nullptr;
	if (TextureOn() && (TextureAmount() > 0.0) && (nullptr != (pChild = FindChild(wszColorChildSlot))))
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

	CRhRdkMaterial::AddUISections(ui);
}

bool CMarmaladeMaterial::ReadParametersFromSection(const IRhRdk_XMLSection& section, ReadParamsContext context)
{
	CRhRdkVariant v;

	if (section.GetParam(wszColor, v))
		SetColor(v.AsRdkColor());

	if (section.GetParam(wszTransparency, v))
		SetTransparency(v);

	if (section.GetParam(wszIOR, v))
		SetIOR(v);

	if (section.GetParam(wszTextureOn, v))
		SetTextureOn(v);

	if (section.GetParam(wszTextureAmount, v))
		SetTextureAmount(v);

	return true;
}

bool CMarmaladeMaterial::WriteParametersToSection(IRhRdk_XMLSection& section, WriteParamsContext context) const
{
	section.SetParam(wszColor,         Color());
	section.SetParam(wszTransparency,  Transparency());
	section.SetParam(wszIOR,           IOR());
	section.SetParam(wszTextureOn,     TextureOn());
	section.SetParam(wszTextureAmount, TextureAmount());

	return true;
}

void* CMarmaladeMaterial::GetShader(const UUID& uuidRenderEngine, void* pvData) const
{
	if (!IsCompatible(uuidRenderEngine))
		return nullptr;

	auto* pShader = m_shader.Clone();
	pShader->ParamBlock() = m_shader.ParamBlock();

	return (void*)pShader;
}

bool CMarmaladeMaterial::IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* /*wszChildSlotName*/) const
{
	if (f.Kind() == Kinds::Texture)
		return true; // Factory produces textures.

	return false; // Factory produces something "unpalatable".
}

unsigned int CMarmaladeMaterial::BitFlags(void) const
{
	return CRhRdkMaterial::BitFlags() & ~bfTextureSummary; // No texture summary required.
}
