
#include "stdafx.h"
#include "SampleRdkMaterialWithChildren.h"
#include "SampleRdkMaterialWithChildrenRdkPlugIn.h"

/** \class CSampleRdkMaterialWithChildren

	This sample material demonstrates how to create a material that has a fixed, permanent child texture.
	See CSampleRdkMaterialWithChildren::Initialize() for more information.

*/

using F = CRhRdkContentField::Filter;

#define DIFFUSE_PARAM_NAME  L"diffuse-color"

CSampleRdkMaterialWithChildren::CSampleRdkMaterialWithChildren()
	:
	m_diffuse_color(*this, DIFFUSE_PARAM_NAME, L"Color", L"Color", F::All, F::All)
{
	m_diffuse_color.SetValue(CRhRdkColor(255, 255, 255));
}

bool CSampleRdkMaterialWithChildren::Initialize(void)
{
	if (!__super::Initialize())
		return false;

	// Get the child slot name from the diffuse color param name.
	// This is usually the same string. In this example, it is the same.
	const auto sChildSlotName = ChildSlotNameFromParamName(DIFFUSE_PARAM_NAME);

	// Create a stucco texture.
	auto* pChild = ::RhRdkContentFactoriesEx().NewContentFromTypeEx(uuidStuccoTextureType, DocumentAssoc());

	// Set the texture as locked. This prevents the user from changing or deleting it.
	pChild->SetLocked();

	// Set the texture as a child of this material in the diffuse color child slot.
	SetChild(pChild, sChildSlotName);

	// Set the child slot 'on'. This is shown on the UI as a check box.
	SetChildSlotOn(sChildSlotName, true);

	return true;
}

UUID CSampleRdkMaterialWithChildren::RenderEngineId(void) const
{
	return CSampleRdkMaterialWithChildrenRdkPlugIn::RenderEngineId();
}

UUID CSampleRdkMaterialWithChildren::PlugInId(void) const
{
	return CSampleRdkMaterialWithChildrenRdkPlugIn::RdkPlugInId();
}

unsigned int CSampleRdkMaterialWithChildren::BitFlags(void) const
{
	auto flags = __super::BitFlags();

	flags |=  bfFields;         // We're using fields (CRhRdkContentField).
	flags &= ~bfTextureSummary; // No texture summary required.

	return flags;
}

UUID CSampleRdkMaterialWithChildren::UuidType(void) // Static.
{
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	//
	// TODO: If you base your plug-in on this code, you MUST change this UUID.
	//       All render content type ids MUST be universally unique.
	//
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!

	static const UUID uuid =
	{
		0x5A5A5A5A, 0x5A5A, 0x5A5A, { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x02 }
	};

	return uuid;
}

UUID CSampleRdkMaterialWithChildren::TypeId(void) const
{
	return UuidType();
}

ON_wString CSampleRdkMaterialWithChildren::TypeName(void) const
{
	return L"SampleRdkMaterialWithChildren";
}

ON_wString CSampleRdkMaterialWithChildren::TypeDescription(void) const
{
	return "Demo material for SampleRdkMaterialWithChildren";
}

ON_wString CSampleRdkMaterialWithChildren::InternalName(void) const
{
	return L"sample-rdk-material-with-children";
}

void CSampleRdkMaterialWithChildren::AddUISections(IRhRdkExpandableContentUI& ui)
{
	// Add an automatic section to the UI.
	const wchar_t* wsz1 = L"Sample RDK material settings";
	AddAutomaticUISection(ui, wsz1, wsz1, 0);

	__super::AddUISections(ui);
}

void CSampleRdkMaterialWithChildren::SimulateMaterial(ON_Material& matOut, CRhRdkTexture::TextureGeneration tg, int iSimulatedTextureSize, const CRhinoObject* pObject) const
{
	__super::SimulateMaterial(matOut, tg, iSimulatedTextureSize, pObject);

	CRhRdkColor col = m_diffuse_color;
	matOut.SetDiffuse(col.OnColor());

	// Get the child slot name from the diffuse color param name.
	// This is usually the same string. In this example, it is the same.
	const auto sChildSlotName = ChildSlotNameFromParamName(DIFFUSE_PARAM_NAME);
	if (!ChildSlotOn(sChildSlotName))
		return;

	const auto* pTexture = dynamic_cast<const CRhRdkTexture*>(FindChild(sChildSlotName));
	if (nullptr != pTexture)
	{
		CRhRdkSimulatedTexture tex;
		pTexture->SimulateTexture(tex, tg, iSimulatedTextureSize, pObject);
		matOut.AddTexture(tex.OnTexture());
	}
}

CRhRdkContent::ParamSerialMethod CSampleRdkMaterialWithChildren::ParameterSerializationMethod(void) const
{
	return ParamSerialMethod::GetSet;
}

void CSampleRdkMaterialWithChildren::AddAutoParameters(IRhRdkParamBlock& paramBlock, int sectionId) const
{
	Fields().AddValuesToParamBlock(paramBlock, sectionId);
}

void CSampleRdkMaterialWithChildren::GetAutoParameters(const IRhRdkParamBlock& paramBlock, int sectionId)
{
	Fields().GetValuesFromParamBlock(paramBlock, sectionId);
}
