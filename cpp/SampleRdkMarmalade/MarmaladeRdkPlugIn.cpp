
#include "stdafx.h"
#include "MarmaladeRdkPlugIn.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeRMP.h"
#include "MarmaladePointCloudRMP.h"
#include "MarmaladeActions.h"
#include "MarmaladeMaterial.h"
#include "MarmaladeNewMaterial.h"
#include "MarmaladeAutoMaterial.h"
#include "MarmaladeAutoUITexture.h"
#include "MarmaladeNewAutoUITexture.h"
#include "MarmaladeContentIOPlugIn.h"
#include "MarmaladeRenderSettingsSection.h"

CMarmaladeRdkPlugIn::CMarmaladeRdkPlugIn(void)
{
}

CMarmaladeRdkPlugIn::~CMarmaladeRdkPlugIn(void)
{
}

UUID CMarmaladeRdkPlugIn::PlugInId(void) const 
{ 
	return MarmaladePlugIn().PlugInID(); 
}

bool CMarmaladeRdkPlugIn::Initialize(void)
{
	return __super::Initialize();
}

void CMarmaladeRdkPlugIn::Uninitialize(void)
{
	__super::Uninitialize();
}

bool CMarmaladeRdkPlugIn::AllowChooseContent(const CRhRdkContent& content) const
{
	// Marmalade does not support blends; prevent the user from choosing blends
	// when Marmalade is the current renderer.

	if (content.TypeId() == uuidBlendMaterialType)
		return false;

	return true;
}

void CMarmaladeRdkPlugIn::RegisterExtensions(void) const
{
	AddExtension(new CMarmaladeMaterialFactory);
	AddExtension(new CMarmaladeNewMaterialFactory);
	AddExtension(new CMarmaladeAutoUITextureFactory);
	AddExtension(new CMarmaladeNewAutoUITextureFactory);

	// Add all the plug-in shaders to the factories.
	for (int i = 0; i < Shaders().Count(); i++)
	{
		const auto* pShader = Shaders().Shader(i);
		AddExtension(new CMarmaladeAutoMaterialFactory(pShader->Uuid()));
	}

	AddExtension(new CMarmaladeRMP);
	AddExtension(new CMarmaladePointCloudRMP);

	AddExtension(new CMarmaladeContentIOPlugIn);

	auto* pEPA = new CMarmaladeExtraPeelTask;
	AddExtension(pEPA);

	AddExtension(new CMarmaladeOrangeTask(*pEPA));
	AddExtension(new CMarmaladeLemonTask (*pEPA));
	AddExtension(new CMarmaladeLimeTask  (*pEPA));

	AddExtension(new CMarmaladeBreadAction);
	AddExtension(new CMarmaladeToastAction);
	AddExtension(new CMarmaladeSconeAction); // Note: Marmalade is not a traditional scone condiment.
}

#ifdef OBSOLETE
void CMarmaladeRdkPlugIn::AddCustomEditorActions(IRhRdkActions& actions, const IRhRdkContentEditor& editor) const
{
	// This function is obsolete because actions have been replaced by tasks. See RegisterExtensions().
}

void CMarmaladeRdkPlugIn::UpdateCustomEditorActions(IRhRdkActions& actions, const IRhRdkContentEditor& editor) const
{
	// This function is obsolete because actions have been replaced by tasks. See RegisterExtensions().
}

void CMarmaladeRdkPlugIn::AddCustomEditorMenu(IRhRdkMenu& menu, const IRhRdkContentEditor& editor) const
{
	// This function is obsolete because actions have been replaced by tasks. See RegisterExtensions().
}
#endif

void CMarmaladeRdkPlugIn::AbortRender(void)
{
	// Nothing to do in Marmalade.
}

inline static void Fill(COLORREF col, CRhinoDib& dib, int step)
{
	for (int y = 0; y < dib.Height(); y += step)
	{
		for (int x = 0; x < dib.Width(); x += step)
		{
			dib.SetPixel(x  , y  , col);
			dib.SetPixel(x+1, y  , col);
			dib.SetPixel(x  , y+1, col);
			dib.SetPixel(x+1, y+1, col);
		}
	}
}

bool CMarmaladeRdkPlugIn::CreatePreview(const ON_2iSize& sizeImage, RhRdkPreviewQuality quality,
	                      const IRhRdkPreviewSceneServer* pSceneServer, IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut)
{
	// This is how you might implement this function, but obviously, with a little better rendering quality.

	if (nullptr == pSceneServer)
		return false;

	const auto* pObject = pSceneServer->NextObject();
	if (nullptr == pObject)
		return false;

	const auto* pMaterial = pObject->Material();
	if (nullptr == pMaterial)
		return false;

	void* pvData = nullptr; // This could be your own private data object.

	void* pvShader = pMaterial->GetShader(MarmaladePlugIn().PlugInID(), pvData);
	if (nullptr == pvShader)
		return false;

	const auto* pShader = reinterpret_cast<CMarmaladeShader*>(pvShader);

	// Use your shader to render into dibOut.

	const auto& paramBlock = pShader->ParamBlock();

	const auto* pParam = paramBlock.FindParameter(L"color");
	if (nullptr == pParam)
		return false;

	CRhinoDib dib(sizeImage.cx, sizeImage.cy, 24);
	dib.FillSolid(RGB(0, 0, 0));

	const auto col = pParam->m_vValue.AsRdkColor().ColorRef();

	switch (quality)
	{
	case RhRdkPreviewQuality::Low:
		Fill(col, dib, 8);
		break;

	case RhRdkPreviewQuality::Medium:
		Fill(col, dib, 4);
		break;

	case RhRdkPreviewQuality::Full:
		dib.FillSolid(col);
		break;
	}

	delete pShader;

	// Pretend to take a long time to produce the image.
	Sleep(500);

	dibOut = dib;

	return true;
}

bool CMarmaladeRdkPlugIn::CreatePreview(const ON_2iSize&, const CRhRdkTexture&, CRhinoDib&)
{
	// Allow RDK to produce all texture previews.
	return false;
}

bool CMarmaladeRdkPlugIn::SupportsFeature(const UUID& uuidFeature) const
{
	// Determine which features of the RDK are exposed while Marmalade is the current renderer.

	if (uuidFeature == uuidFeatureCustomRenderMeshes)
		return false;

	if (uuidFeature == uuidFeatureDecals)
		return false;

	if (uuidFeature == uuidFeatureGroundPlane)
		return false;

//	if (uuidFeature == uuidFeatureSun)
//		return false;

	return true;
}

CRhRdkVariant CMarmaladeRdkPlugIn::GetParameter(const wchar_t* wszParam) const
{
	if (_wcsicmp(wszParam, L"SunSettingsCommandName") == 0)
	{
		return L"MarmaladeSunSettings"; // Not a real command just yet.
	}
	else
	if (_wcsicmp(wszParam, L"ExecuteSunSettingsCommand") == 0)
	{
		return RhinoApp().RunScript(CRhinoDoc::NullRuntimeSerialNumber, L"MarmaladeSunSettings");
	}

	return CRhRdkRenderPlugIn::GetParameter(wszParam);
}

void CMarmaladeRdkPlugIn::AddCustomRenderSettingsSections(RhRdkUiModalities m, ON_SimpleArray<IRhinoUiSection*>& aSections) const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	aSections.Append(new CMarmaladeRenderSettingsSection1(m));
	aSections.Append(new CMarmaladeRenderSettingsSection2(m));
	aSections.Append(new CMarmaladeRenderSettingsSection1_Detailed(m));
	aSections.Append(new CMarmaladeRenderSettingsSection2_Detailed(m));

	// You must call the base class last.
	CRhRdkRenderPlugIn::AddCustomRenderSettingsSections(m, aSections);
}
