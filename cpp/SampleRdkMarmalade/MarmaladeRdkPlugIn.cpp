
#include "stdafx.h"
#include "MarmaladeRdkPlugIn.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeRMP.h"
#include "MarmaladePointCloudRMP.h"
#include "MarmaladeActions.h"
#include "MarmaladeMaterial.h"
#include "MarmaladeAutoMaterial.h"
#include "MarmaladeAutoUITexture.h"
#include "MarmaladeContentIOPlugIn.h"

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

void CMarmaladeRdkPlugIn::EnableNonModalWindows(bool bEnable) const
{
	// Marmalade doesn't have any non modal windows (like a non-model options dialog) so just do nothing.
}

void CMarmaladeRdkPlugIn::RegisterContent(IRhRdkContentFactories& factories) const
{
	factories.Add(new CMarmaladeMaterialFactory);
	factories.Add(new CMarmaladeAutoUITextureFactory);

	// Add all the plug-in shaders to the factories.
	for (int i = 0; i < Shaders().Count(); i++)
	{
		const CMarmaladeShader* pShader = Shaders().Shader(i);
		factories.Add(new CMarmaladeAutoMaterialFactory(pShader->Uuid()));
	}

	__super::RegisterContent(factories);
}

void CMarmaladeRdkPlugIn::RegisterCustomRenderMeshProviders(IRhRdkCustomRenderMeshManager& crmm) const
{
	crmm.Add(new CMarmaladeRMP);
	crmm.Add(new CMarmaladePointCloudRMP);

	__super::RegisterCustomRenderMeshProviders(crmm);
}

void CMarmaladeRdkPlugIn::RegisterContentIOPlugIns(IRhRdkContentIOPlugIns& ciop) const
{
	ciop.Add(new CMarmaladeContentIOPlugIn);

	__super::RegisterContentIOPlugIns(ciop);
}

void CMarmaladeRdkPlugIn::AddCustomEditorActions(IRhRdkActions& actions, const IRhRdkContentEditor& editor) const
{
	// Add custom actions for use by custom menu.

	if (!IsMarmaladeCurrentRenderer())
		return;

	const UUID uuid = PlugInId();

	CMarmaladeExtraPeelAction* pEPA = new CMarmaladeExtraPeelAction(uuid);
	actions.Add(pEPA);

	actions.Add(new CMarmaladeOrangeAction(uuid, *pEPA));
	actions.Add(new CMarmaladeLemonAction (uuid, *pEPA));
	actions.Add(new CMarmaladeLimeAction  (uuid, *pEPA));

	actions.Add(new CMarmaladeBreadAction(uuid));
	actions.Add(new CMarmaladeToastAction(uuid));
	actions.Add(new CMarmaladeSconeAction(uuid)); // Note: Marmalade is not a traditional scone condiment.
}

void CMarmaladeRdkPlugIn::UpdateCustomEditorActions(IRhRdkActions& actions, const IRhRdkContentEditor& editor) const
{
	if (!IsMarmaladeCurrentRenderer())
		return;

	// Update [checked and] enabled state of actions.
	for (int i = 0; i < actions.Count(); i++)
	{
		// Enable actions for the Material Editor only.
		const bool bEnable = (0 == wcscmp(editor.Kind(), RDK_KIND_MATERIAL));
		actions.Action(i)->SetEnabled(bEnable);
	}
}

void CMarmaladeRdkPlugIn::AddCustomEditorMenu(IRhRdkMenu& menu, const IRhRdkContentEditor& editor) const
{
	// Add custom menu.

	if (!IsMarmaladeCurrentRenderer())
		return;

	IRhRdkMenu& subMenu = menu.AddSubMenu(L"&Marmalade");
	subMenu.AddItem(CMarmaladeOrangeAction::Ident());
	subMenu.AddItem(CMarmaladeLemonAction::Ident());
	subMenu.AddItem(CMarmaladeLimeAction::Ident());
	subMenu.AddSeparator();
	subMenu.AddItem(CMarmaladeExtraPeelAction::Ident());
	subMenu.AddSeparator();
	subMenu.AddItem(CMarmaladeBreadAction::Ident());
	subMenu.AddItem(CMarmaladeToastAction::Ident());
	subMenu.AddItem(CMarmaladeSconeAction::Ident());
}

bool CMarmaladeRdkPlugIn::IsMarmaladeCurrentRenderer(void) const
{
	if (RhinoApp().GetDefaultRenderApp() == PlugInId())
		return true;

	return false;
}

void CMarmaladeRdkPlugIn::AbortRender(void)
{
	// Nothing to do in Marmalade.
}

HBITMAP CMarmaladeRdkPlugIn::CreatePreview(const CSize& sizeImage, eRhRdkRenderQuality quality,
                                           const IRhRdkPreviewSceneServer* pSceneServer)
{
	// This function does nothing so that the simulated material is shown by the RDK preview renderer.

	if (NULL == pSceneServer)
		return NULL;

	const IRhRdkPreviewSceneServer::IObject* pObject = pSceneServer->NextObject();

	const CRhRdkMaterial* pMaterial = pObject->Material();
	if (NULL == pMaterial)
		return NULL;

	void* pvData = NULL; // Could be your own private data object.

	HBITMAP hBitmap = NULL;

	void* pvShader = pMaterial->GetShader(MarmaladePlugIn().PlugInID(), pvData);
	if (NULL != pvShader)
	{
		CMarmaladeShader* pShader = reinterpret_cast<CMarmaladeShader*>(pvShader);

		// Use your shader to render into a bitmap.

		delete pShader;
	}

	return hBitmap;



	// This is how you might implement this function - but obviously, with a little better rendering quality.

	CRhinoDib dib(sizeImage.cx, sizeImage.cy, 24);

	switch (quality)
	{
	case rcmQualLow:
		dib.FillSolid(RGB(255, 140, 0));
		break;

	case rcmQualMedium:
		dib.FillSolid(RGB(255, 255, 0));
		break;

	case rcmQualFull:
		dib.FillSolid(RGB(0, 255, 0));
		break;
	}

	// Pretend to take a long time to produce the image.
	Sleep(1000);

	return dib.CopyHBitmap();
}

HBITMAP CMarmaladeRdkPlugIn::CreatePreview(const CSize& sizeImage, const CRhRdkTexture& texture)
{
	// Allow RDK to produce all texture previews.
	return NULL;
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
		return RhinoApp().RunScript(L"MarmaladeSunSettings");
	}

	return CRhRdkRenderPlugIn::GetParameter(wszParam);
}

#include "MarmaladeOptionsSection.h"
class CMarmaladeViewDockBarManager : public CRhRdkViewDockBarCustomSectionManager
{
	bool AddSections(ON_SimpleArray<CRhRdkViewDockBarCustomSection*>& aSections) const
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		aSections.Append(new CMarmaladeViewDockBarSection);
		return true;
	}

	void DeleteThis()				{ delete this; }
	UUID PlugInID(void) const		{		return MarmaladePlugIn().PlugInID();	}
};

void CMarmaladeRdkPlugIn::RegisterCustomPlugIns(void) const
{
	AddCustomPlugIn(new CMarmaladeViewDockBarManager);
}
