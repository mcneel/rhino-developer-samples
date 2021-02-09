
#include "stdafx.h"
#include "SampleRdkMaterial.h"
#include "SampleRdkMaterialAutoUIRdkPlugIn.h"

/** This sample material demonstrates how to use fields in different ways
	in conjunction with the RDK's Automatic UI system.

	Fields can be static or dynamic. Static fields are the most commonly
	used and are simply members of the content class. They are initialized
	in the custom content's constructor. Dynamic fields can be added at
	run-time (usually by loading them from a file).

	This sample also demonstrates how to create a material with an automatic UI
	whose (dynamic) fields can be loaded from a file. If the material is merely
	created (not loaded), only its static fields will exist. If a .sample file
	is loaded as a material, the static fields will be supplemented by dynamic
	fields created by the content I/O plug-in that parses the file.

*/

using F = CRhRdkContentField::Filter;

static CRhRdkVariant nullVariant;

CSampleRdkMaterial::CSampleRdkMaterial()
	:
	// Construct static fields. A .sample file must never contain an entry with the
	// same name as a static field's internal name. If you only want to load
	// dynamic fields, you can delete these static fields.
	m_diffuse_color (*this, L"diffuse-color",  L"Color",          L"Color",          F::All, F::All),
	m_ambient_color (*this, L"ambient-color",  L"Ambient color",  L"Ambient color",  F::All, F::All),
	m_glossiness    (*this, L"glossiness",     L"Glossiness",     L"Glossiness",     F::All, F::All),
	m_description   (*this, L"description",    L"Description",    L"Description",    F::All, F::All),

	// This field demonstrates how to hide the 'on' check box and 'amount' edit box.
	// This is accomplished by passing null variants for vTextureOn and vTextureAmount.
	m_emission_color(*this, L"emission-color", L"Emission color", L"Emission color", F::All, F::All, 0,
	                 false, nullVariant, nullVariant)
{
	// Initialize static fields. This is necessary even if the default value is
	// zero or an empty string, because it sets the field type as well as the value.

	// Subclassed fields need a static cast to access operator =
	static_cast<CRhRdkContentField&>(m_diffuse_color) = CRhRdkColor(22, 130, 255);
	static_cast<CRhRdkContentField&>(m_emission_color) = CRhRdkColor(0, 0, 0);

	m_ambient_color = CRhRdkColor(64, 64, 64);

	m_glossiness = 0.5f;
	m_glossiness.SetLimits(0.0f, 1.0f);

	m_description = L"";
}

UUID CSampleRdkMaterial::RenderEngineId(void) const
{
	return CSampleRdkMaterialAutoUIRdkPlugIn::RenderEngineId();
}

UUID CSampleRdkMaterial::PlugInId(void) const
{
	return CSampleRdkMaterialAutoUIRdkPlugIn::RdkPlugInId();
}

unsigned int CSampleRdkMaterial::BitFlags(void) const
{
	auto flags = __super::BitFlags();

	flags |=  bfFields;         // We're using fields (CRhRdkContentField).
	flags &= ~bfTextureSummary; // No texture summary required.

	return flags;
}

void CSampleRdkMaterial::AddDynamicField(const wchar_t* wszName, const CRhRdkVariant vValue)
{
	// This is called when a .sample file is loaded.

	// Add a dynamic field to the material. The material takes ownership of the field
	// and the returned pointer should not be stored.
	new CRhRdkDynamicContentField(*this, wszName, nullptr, nullptr, vValue, F::All, F::All);

#ifdef _DEBUG
	OutputDebugString(wszName);
	OutputDebugString(L"=");
	OutputDebugString(vValue.AsString());
	OutputDebugString(L" : ");

	ON_wString s;
	vValue.Format(s);
	OutputDebugString(s);
	OutputDebugString(L"\n");
#endif
}

UUID CSampleRdkMaterial::UuidType(void) // Static.
{
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	//
	// TODO: If you base your plug-in on this code, you MUST change this UUID.
	//       All render content type ids MUST be universally unique.
	//
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!

	static const UUID uuid =
	{
		0x5A5A5A5A, 0x5A5A, 0x5A5A, { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x01 }
	};

	return uuid;
}

UUID CSampleRdkMaterial::TypeId(void) const
{
	return UuidType();
}

ON_wString CSampleRdkMaterial::TypeName(void) const
{
	return L"SampleRdkMaterialAutoUI";
}

ON_wString CSampleRdkMaterial::TypeDescription(void) const
{
	return "Demo material for SampleRdkMaterialAutoUI";
}

ON_wString CSampleRdkMaterial::InternalName(void) const
{
	return L"sample-rdk-material";
}

void CSampleRdkMaterial::SimulateMaterial(OUT ON_Material& mat, CRhRdkTexture::TextureGeneration tg, int iSimulatedTextureSize, const CRhinoObject* pObject) const
{
	CRhRdkMaterial::SimulateMaterial(mat, tg, iSimulatedTextureSize, pObject);

	// Set up the ON_Material based on your material state so that it looks as much
	// as possible like your material will look when it is rendered.

	mat.m_diffuse  = m_diffuse_color;
	mat.m_emission = m_emission_color;
	mat.m_ambient  = m_ambient_color;

	const double dGlossFinish = m_glossiness;
	mat.m_shine = (dGlossFinish < 0.005) ? 0.0 : (dGlossFinish * ON_Material::MaxShine);

	mat.m_transparency        = 0.0;
	mat.m_reflectivity        = 0.0;
	mat.m_index_of_refraction = 1.0;
	mat.m_transparent         = ON_Color(0, 0, 0);
	mat.m_reflection          = ON_Color(0, 0, 0);
}

bool CSampleRdkMaterial::IsFactoryProductAcceptableAsChild(const CRhRdkContentFactory& f, const wchar_t* wszChildSlotName) const
{
	// Determine if factory 'f' produces content suitable as a child for a particular child slot.
	// If so, return true, otherwise return false.
	// Note: The child-slot-name is the same as the field (AKA parameter) name for our textured fields,
	// because we don't override ChildSlotNameFromParamName().

	if (0 == _wcsicmp(L"diffuse-color", wszChildSlotName))
	{
		// The diffuse-color child slot is limited to bitmap textures.
		if (f.ContentTypeId() == uuidBitmapTextureType)
			return true;

		if (f.ContentTypeId() == uuidSimpleBitmapTextureType)
			return true;
	}
	else
	if (0 == _wcsicmp(L"emission-color", wszChildSlotName))
	{
		// The emission-color child slot is limited to any textures.
		if (f.IsKind(Kinds::Texture))
			return true;
	}
	else
	if (0 == _wcsicmp(L"material", wszChildSlotName))
	{
		// The environment child slot is limited to any materials.
		if (f.IsKind(Kinds::Material))
			return true;
	}

	return false;
}

void CSampleRdkMaterial::AddUISections(IRhRdkExpandableContentUI& ui)
{
	// Add an automatic section to the UI.
	const wchar_t* wsz1 = L"Sample RDK material settings";
	AddAutomaticUISection(ui, wsz1, wsz1, 0);

	// You can also add a regular section to the UI.
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//ui.AddSection(new CMySection);

	__super::AddUISections(ui);
}

CRhRdkContent::ParamSerialMethod CSampleRdkMaterial::ParameterSerializationMethod(void) const
{
	return ParamSerialMethod::GetSet;
}

void CSampleRdkMaterial::AddAutoParameters(IRhRdkParamBlock& paramBlock, int sectionId) const
{
	Fields().AddValuesToParamBlock(paramBlock, sectionId);
}

void CSampleRdkMaterial::GetAutoParameters(const IRhRdkParamBlock& paramBlock, int sectionId)
{
	Fields().GetValuesFromParamBlock(paramBlock, sectionId);
}
