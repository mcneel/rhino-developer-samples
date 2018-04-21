
#include "stdafx.h"
#include "MarmaladeAutoMaterial.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeColorSection.h"

static const wchar_t* wszColor         = L"color";
static const wchar_t* wszTransparency  = L"transparency";
static const wchar_t* wszTextureOn     = L"texture-on";
static const wchar_t* wszTextureAmount = L"texture-amount";
static const wchar_t* wszColorChildSlot= L"color-child-slot";

//---- CMarmaladeAutoMaterialFactory

CMarmaladeAutoMaterialFactory::CMarmaladeAutoMaterialFactory(const UUID& uuidShader)
	:
	m_uuid(uuidShader)
{
}

CRhRdkMaterial* CMarmaladeAutoMaterialFactory::NewMaterial(void) const
{
	return new CMarmaladeAutoMaterial(m_uuid);
}

//---- CMarmaladeAutoMaterial

CMarmaladeAutoMaterial::CMarmaladeAutoMaterial(const UUID& uuidShader)
{
	m_pShader = Shaders().FindShader(uuidShader);

	ASSERT(nullptr != m_pShader);
}

bool CMarmaladeAutoMaterial::Initialize(void)
{
	m_ParamBlock = m_pShader->ParamBlock();

	return CRhRdkContent::Initialize();
}

UUID CMarmaladeAutoMaterial::RenderEngineId(void) const
{
	return MarmaladePlugIn().PlugInID();
}

UUID CMarmaladeAutoMaterial::PlugInId(void) const
{
	return MarmaladePlugIn().PlugInID();
}

UUID CMarmaladeAutoMaterial::TypeId(void) const
{
	return m_pShader->Uuid();
}

ON_wString CMarmaladeAutoMaterial::TypeName(void) const
{
	return m_pShader->FriendlyName();
}

ON_wString CMarmaladeAutoMaterial::TypeDescription(void) const
{
	return m_pShader->Description();
}

ON_wString CMarmaladeAutoMaterial::InternalName(void) const
{
	return m_pShader->Name();
}

bool CMarmaladeAutoMaterial::TextureOn(void) const
{
	const CMarmaladeParam* pParam = m_ParamBlock.FindParameter(wszTextureOn);
	ASSERT(nullptr != pParam);

	return pParam->m_vValue;
}

void CMarmaladeAutoMaterial::SetTextureOn(bool b, eChangeContext cc)
{
	CMarmaladeParam* pParam = m_ParamBlock.FindParameter(wszTextureOn);
	ASSERT(nullptr != pParam);

	if (pParam->m_vValue.AsBool() != b)
	{
		pParam->m_vValue = b;
		Changed(cc);
	}
}

double CMarmaladeAutoMaterial::TextureAmount(void) const
{
	const CMarmaladeParam* pParam = m_ParamBlock.FindParameter(wszTextureAmount);
	ASSERT(nullptr != pParam);

	return pParam->m_vValue;
}

void CMarmaladeAutoMaterial::SetTextureAmount(double d, eChangeContext cc)
{
	CMarmaladeParam* pParam = m_ParamBlock.FindParameter(wszTextureAmount);
	ASSERT(nullptr != pParam);

	if (pParam->m_vValue.AsDouble() != d)
	{
		pParam->m_vValue = d;
		Changed(cc);
	}
}

void CMarmaladeAutoMaterial::SimulateMaterial(ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSimulatedTextureSize, const CRhinoObject* pObject) const
{
	const CMarmaladeParam* pParam = m_ParamBlock.FindParameter(L"color");
	if (nullptr != pParam)
	{
		mat.SetDiffuse(pParam->m_vValue.AsRdkColor().ColorRefAlpha());
	}

	pParam = m_ParamBlock.FindParameter(L"transparency");
	if (nullptr != pParam)
	{
		mat.SetTransparency(pParam->m_vValue);
	}

	const CRhRdkContent* pChild = nullptr;
	if ((nullptr != (pChild = FindChild(wszColorChildSlot)) && TextureOn() && (TextureAmount() > 0.0)))
	{
		// IsFactoryProductAcceptableAsChild should ensure that the child is a RDK_KIND_TEXTURE
		// but it never hurts to check.
		if (pChild->IsKind(CRhRdkContent::Kinds::Texture))
		{
			const CRhRdkTexture* pTexture = static_cast<const CRhRdkTexture*>(pChild);

			CRhRdkSimulatedTexture onTexture;
			pTexture->SimulateTexture(onTexture, bForDataOnly);

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

const int idParameters1 = 0;
const int idParameters2 = 1;

void CMarmaladeAutoMaterial::AddUISections(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UI()->AddSection(new CMarmaladeColorSection);

	AddAutomaticUISection(L"Parameters 1", idParameters1);
	AddAutomaticUISection(L"Parameters 2", idParameters2);

	CRhRdkMaterial::AddUISections();
}

void CMarmaladeAutoMaterial::AddAutoParameters(IRhRdkParamBlock& paramBlock, int iId)
{
	for (int i = 0; i < m_ParamBlock.ParameterCount(); i++)
	{
		const CMarmaladeParam* p = m_ParamBlock.Parameter(i);

		const bool bColor = (0 == p->m_sName.CompareNoCase(wszColor));

		if ( bColor && (iId == CRhRdkCoreContent::eIdTextureSummary) ||
			 bColor && (iId == idParameters1) ||
			!bColor && (iId == idParameters2))
		{
			if (bColor && IsTexturingSupported())
			{
				paramBlock.Add(p->m_sName, wszColorChildSlot, p->m_sFriendlyName, p->m_vValue,
							   p->m_vMin, p->m_vMax, RDK_TEXTURING, this);
			}
			else
			{
				paramBlock.Add(p->m_sName, L"", p->m_sFriendlyName, p->m_vValue,
							   p->m_vMin, p->m_vMax, RDK_DECIMAL_PLACES, this);
			}
		}
	}
}

void CMarmaladeAutoMaterial::GetAutoParameters(const IRhRdkParamBlock& paramBlock, int iId)
{
	IRhRdkParamBlock::Iterator* pIterator = paramBlock.NewIterator();

	ON_wString sParamName;
	CRhRdkVariant vParamValue;

	while (paramBlock.Next(pIterator, sParamName, vParamValue))
	{
		CMarmaladeParam* pParam = m_ParamBlock.FindParameter(sParamName);
		if (nullptr != pParam)
		{
			pParam->m_vValue = vParamValue;
		}
	}

	delete pIterator;
}

bool CMarmaladeAutoMaterial::SetParameters(IRhRdk_XMLSection& section, eSetParamsContext context) const
{
	for (int i = 0; i < m_ParamBlock.ParameterCount(); i++)
	{
		const CMarmaladeParam* pParam = m_ParamBlock.Parameter(i);
		section.SetParam(pParam->m_sName, pParam->m_vValue);
	}

	return true;
}

bool CMarmaladeAutoMaterial::GetParameters(const IRhRdk_XMLSection& section, eGetParamsContext context)
{
	IRhRdk_XMLSection::Iterator* pIterator = section.NewIterator();

	ON_wString sParamName;
	CRhRdkVariant vParamValue;

	while (section.NextParam(pIterator, sParamName, vParamValue))
	{
		CMarmaladeParam* pParam = m_ParamBlock.FindParameter(sParamName);
		if (nullptr != pParam)
		{
			pParam->m_vValue = vParamValue;
		}
	}

	delete pIterator;

	return true;
}

CRhRdkVariant CMarmaladeAutoMaterial::GetParameter(const wchar_t* wszName) const
{
	const CMarmaladeParam* pParam = m_ParamBlock.FindParameter(wszName);
	if (nullptr == pParam)
	{
		return CRhRdkMaterial::GetParameter(wszName);
	}

	return pParam->m_vValue;
}

bool CMarmaladeAutoMaterial::SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue, CRhRdkContent::eChangeContext cc)
{
	CMarmaladeParam* pParam = m_ParamBlock.FindParameter(wszName);
	if (nullptr != pParam)
	{
		if (pParam->m_vValue != vValue)
		{
			pParam->m_vValue = vValue;

			Changed(cc);
		}
	}

	return true;
}

void* CMarmaladeAutoMaterial::GetShader(const UUID& uuidRenderEngine, void* pvData) const
{
	if (!IsCompatible(uuidRenderEngine))
		return nullptr;

	CMarmaladeShader* pShader = m_pShader->Clone();

	pShader->ParamBlock() = m_ParamBlock;

	return reinterpret_cast<void*>(pShader);
}

bool CMarmaladeAutoMaterial::IsTexturingSupported(void) const
{
	return (nullptr != m_ParamBlock.FindParameter(wszColor)) &&
	       (nullptr != m_ParamBlock.FindParameter(wszTextureOn)) &&
	       (nullptr != m_ParamBlock.FindParameter(wszTextureAmount));
}

DWORD CMarmaladeAutoMaterial::BitFlags(void) const
{
	DWORD dw = CRhRdkMaterial::BitFlags() | bfSharedUI; // Shared UI supported.

	if (!IsTexturingSupported()) // If texturing not supported then
		dw &= ~bfTextureSummary; // no texture summary required.

	return dw;
}

const wchar_t* CMarmaladeAutoMaterial::Category(void) const
{
	return RDK_CAT_GENERAL;
}

bool CMarmaladeAutoMaterial::IsFactoryProductAcceptableAsChild(const IRhRdkContentFactory* pFactory,
                                                               const wchar_t* wszChildSlotName) const
{
	const ON_wString sFactoryKind = pFactory->Kind();

	if (0 == sFactoryKind.CompareNoCase(RDK_KIND_TEXTURE))
		return true; // Factory produces textures.

	return false; // Factory produces something "unpalatable".
}

bool CMarmaladeAutoMaterial::GetExtraRequirementParameter(const wchar_t* wszParamName, const wchar_t* wszExtraRequirementName, CRhRdkVariant& vValueOut) const
{
	if (CRhRdkMaterial::GetExtraRequirementParameter(wszParamName, wszExtraRequirementName, vValueOut))
		return true;

	const ON_wString sParamName = wszParamName;

	const ON_wString sExtraRequirementName = wszExtraRequirementName;

	if (sExtraRequirementName == RDK_DECIMAL_PLACES_AMOUNT)
	{
		if (sParamName == wszTransparency)
		{
			vValueOut = 2;
		}
		else
		{
			vValueOut = 3;
		}

		return true;
	}

	// Handle texturing extra requirements for the color parameter.

	if (sParamName == wszColor)
	{
		if (sExtraRequirementName == RDK_TEXTURE_ON)
		{
			vValueOut = TextureOn();
			return true;
		}

		if (sExtraRequirementName == RDK_TEXTURE_AMOUNT)
		{
			vValueOut = TextureAmount() * 100.0;
			return true;
		}
	}

	return false;
}

bool CMarmaladeAutoMaterial::SetExtraRequirementParameter(const wchar_t* wszParamName,
                                                          const wchar_t* wszExtraRequirementName,
	                                                      const CRhRdkVariant& vValue, eSetContext sc)
{
	const ON_wString sParamName = wszParamName;

	if (sParamName == wszColor)
	{
		const eChangeContext cc = ChangeContextFromSetContext(sc);

		const ON_wString sExtraRequirementName = wszExtraRequirementName;

		if (sExtraRequirementName == RDK_TEXTURE_ON)
		{
			SetTextureOn(vValue, cc);
			return true;
		}

		if (sExtraRequirementName == RDK_TEXTURE_AMOUNT)
		{
			SetTextureAmount(vValue.AsFloat() / 100.0f, cc);
			return true;
		}
	}

	return false;
}

class CMarmaladeAutoMaterialCSI : public CRhRdkContent::CChildSlotIterator
{
public:
	CMarmaladeAutoMaterialCSI(const CMarmaladeAutoMaterial* pMaterial, CRhRdkContent::eCSI_Context context)
		: CRhRdkContent::CChildSlotIterator(pMaterial, context) { m_iIndex = -1; }

	virtual bool NextChildSlot(ON_wString& sInternalNameOut, ON_wString& sChildSlotNameOut, ON_wString& sDisplayNameOut);

private:
	int m_iIndex;
};

bool CMarmaladeAutoMaterialCSI::NextChildSlot(ON_wString& sInternalNameOut, ON_wString& sChildSlotNameOut, ON_wString& sDisplayNameOut)
{
	m_iIndex++;

	switch (m_iIndex)
	{
	case 0:
		sInternalNameOut  = wszColor;
		sDisplayNameOut   = L"Color";
		sChildSlotNameOut = wszColorChildSlot;
		return true;
	}

	return false;
}

CRhRdkContent::CChildSlotIterator* CMarmaladeAutoMaterial::NewChildSlotIterator(eCSI_Context context) const
{
	if (!IsTexturingSupported())
		return nullptr;

	return new CMarmaladeAutoMaterialCSI(this, context);
}
