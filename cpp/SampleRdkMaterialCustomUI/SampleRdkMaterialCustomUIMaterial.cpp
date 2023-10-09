
#include "stdafx.h"
#include "SampleRdkMaterialCustomUIMaterial.h"
#include "SampleRdkMaterialCustomUIDlg.h"
#include "SampleRdkMaterialCustomUIPlugIn.h"

/*	CSampleRdkMaterialCustomUIMaterial demonstrates how to create a custom content UI.

	This kind of UI does not use the standard 'holder' and 'sections' that RDK creates by
	default. To do this, it is necessary to override CRhRdkContent::CreateUI() and create
	an instance of a dialog which is derived from both CRhinoDialog and CRhRdkCustomContentUI.

	Note: because of RDK's UI sharing system, this UI will only be created once and shared
	between all CSampleRdkMaterialCustomUIMaterial materials.

*/

using F = CRhRdkContentField::Filter;

CSampleRdkMaterialCustomUIMaterial::CSampleRdkMaterialCustomUIMaterial()
	:
	m_color(*this, L"color", L"Color", L"Color", F::All, F::All)
{
	m_color = CRhRdkColor(200, 200, 200);
}

UUID CSampleRdkMaterialCustomUIMaterial::RenderEngineId(void) const
{
	return ::SampleRdkMaterialCustomUIPlugIn().PlugInID();
}

UUID CSampleRdkMaterialCustomUIMaterial::PlugInId(void) const
{
	return ::SampleRdkMaterialCustomUIPlugIn().PlugInID();
}

UUID CSampleRdkMaterialCustomUIMaterial::TypeId(void) const
{
	static const UUID uuid =
	{
		// {6B4CA36E-0D2A-40AC-9881-89FE269B3B62}
		0x6B4CA36E, 0xD2A, 0x40AC, { 0x98, 0x81, 0x89, 0xFE, 0x26, 0x9B, 0x3B, 0x62 }
	};

	return uuid;
}

ON_wString CSampleRdkMaterialCustomUIMaterial::TypeName(void) const
{
	// Return the user-friendly type name of the material.
	return L"SampleRdkMaterialCustomUI";
}

ON_wString CSampleRdkMaterialCustomUIMaterial::TypeDescription(void) const
{
	// Return a description of the material.
	return "Demo material for SampleRdkMaterialCustomUI";
}

ON_wString CSampleRdkMaterialCustomUIMaterial::InternalName(void) const
{
	// Return the material's internal name (not usually seen by the user).
	return L"sample-rdk-material-custom-ui";
}

void CSampleRdkMaterialCustomUIMaterial::SimulateMaterial(OUT ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSimulatedTextureSize, const CRhinoObject* pObject) const
{
	CRhRdkMaterial::SimulateMaterial(mat, tg, iSimulatedTextureSize, pObject);

	// Set up the ON_Material based on your material state so that it looks as much
	// as possible like your material will look when it is rendered.

	mat.m_diffuse = m_color;
	mat.m_emission = ON_Color::Black;
	mat.m_ambient = ON_Color::Black;
	mat.m_shine = 0.0;
	mat.m_transparency = 0.0;
	mat.m_reflectivity = 0.0;
	mat.m_index_of_refraction = 1.0;
	mat.m_transparent = ON_Color::Black;
	mat.m_reflection = ON_Color::Black;
}

IRhRdkContentUI* CSampleRdkMaterialCustomUIMaterial::CreateUI(IRhRdkContentEditor& e, const UUID& uuidUI, const CRhRdkContentArray& aInitialClients, void* pReserved)
{
	UNREFERENCED_PARAMETER(aInitialClients);
	UNREFERENCED_PARAMETER(pReserved);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const auto pDlg = new CSampleRdkMaterialCustomUIDlg(e.Id(), uuidUI, nullptr);
	pDlg->CreateDlg(nullptr);

	return pDlg;
}

CRhRdkVariant CSampleRdkMaterialCustomUIMaterial::GetParameter(const wchar_t* wszName) const
{
	if (0 == _wcsicmp(L"color", wszName))
	{
		return m_color.Value();
	}

	return __super::GetParameter(wszName);
}

bool CSampleRdkMaterialCustomUIMaterial::SetParameter(const wchar_t* wszName, const CRhRdkVariant& vValue)
{
	if (0 == _wcsicmp(L"color", wszName))
	{
		return m_color.SetValue(vValue);
	}

	return __super::SetParameter(wszName, vValue);
}

void* CSampleRdkMaterialCustomUIMaterial::GetShader(const UUID& uuidRenderEngine, void* pvData) const
{
	UNREFERENCED_PARAMETER(pvData);

	if (!IsCompatible(uuidRenderEngine))
		return nullptr;

	void* pShader = nullptr;

	// TODO: Get a pointer to the shader used to render this material.

	return pShader;
}

bool CSampleRdkMaterialCustomUIMaterial::IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& factory, const wchar_t* wszChildSlotName) const
{
	// TODO: Determine if pFactory produces content suitable as a child for a particular child slot.
	//       If so, return true, otherwise return false.
	UNREFERENCED_PARAMETER(factory);
	UNREFERENCED_PARAMETER(wszChildSlotName);

	return false;
}

unsigned int CSampleRdkMaterialCustomUIMaterial::BitFlags(void) const
{
	auto flags = __super::BitFlags();

	flags |=  bfFields;         // We're using fields (CRhRdkContentField).
	flags &= ~bfTextureSummary; // No texture summary required.

	return flags;
}
