
#include "stdafx.h"
#include "SampleRdkMaterial.h"
#include "SampleRdkMaterialAutoUIRdkPlugIn.h"

UUID CSampleRdkMaterial::RenderEngineId(void) const
{
	return CSampleRdkMaterialAutoUIRdkPlugIn::RenderEngineId();
}

UUID CSampleRdkMaterial::PlugInId(void) const
{
	return CSampleRdkMaterialAutoUIRdkPlugIn::RdkPlugInId();
}

DWORD CSampleRdkMaterial::BitFlags(void) const
{
	return (__super::BitFlags() | bfFields | bfDynamicFields) & ~bfTextureSummary;
}

CSampleRdkMaterial::~CSampleRdkMaterial()
{
}

void CSampleRdkMaterial::AddField(const wchar_t* wszName, const CRhRdkVariant vValue)
{
	using F = CRhRdkContentField::Filter;
	auto pField = new CRhRdkContentField(*this, wszName, nullptr, nullptr, F::All, F::All, 0, true);
	*pField = vValue;

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

UUID CSampleRdkMaterial::TypeId(void) const
{
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	//
	// TODO: If you base your plug-in on this code, you MUST change this UUID.
	//
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!

	static const UUID uuid =
	{
		0x5A5A5A5A, 0x5A5A, 0x5A5A, { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x01 }
	};

	return uuid;
}

ON_wString CSampleRdkMaterial::TypeName(void) const
{
	return L"SampleRdkMaterialAutoUI";
}

ON_wString CSampleRdkMaterial::TypeDescription(void) const
{
	return "Sample material for SampleRdkMaterialAutoUI";
}

ON_wString CSampleRdkMaterial::InternalName(void) const
{
	return L"sample-rdk-material";
}

void CSampleRdkMaterial::SimulateMaterial(ON_Material& matOut, CRhRdkTexture::TextureGeneration tg, int iSimulatedTextureSize, const CRhinoObject* pObject) const
{
	CRhRdkMaterial::SimulateMaterial(matOut, tg, iSimulatedTextureSize, pObject);
}

void CSampleRdkMaterial::AddUISections(IRhRdkExpandableContentUI& ui)
{
	const wchar_t* wsz1 = L"Sample RDK material settings";
	AddAutomaticUISection(ui, wsz1, wsz1, 0);

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
