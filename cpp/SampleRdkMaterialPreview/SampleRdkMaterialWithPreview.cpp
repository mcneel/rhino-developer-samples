
#include "stdafx.h"
#include "SampleRdkMaterialWithPreview.h"
#include "SampleRdkMaterialPreviewPlugIn.h"
#include "SampleRdkMaterialPreviewRdkPlugIn.h"

/** \class CSampleRdkMaterialWithPreview

	This sample material demonstrates how to create a material that renders its own preview.

*/

using F = CRhRdkContentField::Filter;

CSampleRdkMaterialWithPreview::CSampleRdkMaterialWithPreview()
	:
	m_diffuse_color(*this, DIFFUSE_PARAM_NAME, L"Color", L"Color", F::All, F::All)
{
	m_diffuse_color.SetValue(CRhRdkColor(255, 255, 255));
}

bool CSampleRdkMaterialWithPreview::Initialize(void)
{
	if (!__super::Initialize())
		return false;

	return true;
}

UUID CSampleRdkMaterialWithPreview::RenderEngineId(void) const
{
	return ::SampleRdkMaterialPreviewPlugIn().PlugInID();
}

UUID CSampleRdkMaterialWithPreview::PlugInId(void) const
{
	return ::SampleRdkMaterialPreviewPlugIn().PlugInID();
}

unsigned int CSampleRdkMaterialWithPreview::BitFlags(void) const
{
	auto flags = __super::BitFlags();

	flags |=  bfFields;         // We're using fields (CRhRdkContentField).
	flags &= ~bfTextureSummary; // No texture summary required.

	return flags;
}

UUID CSampleRdkMaterialWithPreview::UuidType(void) // Static.
{
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	//
	// TODO: If you base your plug-in on this code, you MUST change this UUID.
	//       All render content type ids MUST be universally unique.
	//
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!

	static const UUID uuid =
	{
		0x59595959, 0x5959, 0x5959, { 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x06 }
	};

	return uuid;
}

UUID CSampleRdkMaterialWithPreview::TypeId(void) const
{
	return UuidType();
}

ON_wString CSampleRdkMaterialWithPreview::TypeName(void) const
{
	return L"SampleRdkMaterialWithPreview";
}

ON_wString CSampleRdkMaterialWithPreview::TypeDescription(void) const
{
	return "Demo material for SampleRdkMaterialPreview";
}

ON_wString CSampleRdkMaterialWithPreview::InternalName(void) const
{
	return L"sample-rdk-material-for-preview";
}

void CSampleRdkMaterialWithPreview::AddUISections(IRhRdkExpandableContentUI& ui)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Add an automatic section to the UI.
	const wchar_t* wsz1 = L"Sample RDK material settings";
	AddAutomaticUISection(ui, wsz1, wsz1, 0);

	// Add a custom section that also draws the preview. This demonstrates how to call
	// CreatePreview() from your UI.
	ui.AddSection(new CCustomContentSection);

	__super::AddUISections(ui);
}

void CSampleRdkMaterialWithPreview::SimulateMaterial(ON_Material& matOut, CRhRdkTexture::TextureGeneration tg, int iSimulatedTextureSize, const CRhinoObject* pObject) const
{
	__super::SimulateMaterial(matOut, tg, iSimulatedTextureSize, pObject);

	CRhRdkColor col = m_diffuse_color;
	matOut.SetDiffuse(col.OnColor());
}

CRhRdkContent::ParamSerialMethod CSampleRdkMaterialWithPreview::ParameterSerializationMethod(void) const
{
	return ParamSerialMethod::GetSet;
}

void CSampleRdkMaterialWithPreview::AddAutoParameters(IRhRdkParamBlock& paramBlock, int sectionId) const
{
	Fields().AddValuesToParamBlock(paramBlock, sectionId);
}

void CSampleRdkMaterialWithPreview::GetAutoParameters(const IRhRdkParamBlock& paramBlock, int sectionId)
{
	Fields().GetValuesFromParamBlock(paramBlock, sectionId);
}

bool CSampleRdkMaterialWithPreview::CreatePreview(CRhRdkRenderPlugIn& plugIn, const ON_2iSize& sizeImage, RhRdkPreviewQuality qual,
                                                  const IRhRdkPreviewSceneServer* pSceneServer,
                                                  IRhRdkPreviewCallbacks* pNotify, CRhinoDib& dibOut) const
{
	// We just call the base class, but the purpose of this override is to allow the material to render
	// its preview in a different way. You can create 'dibOut' any way you like.
	return CRhRdkMaterial::CreatePreview(plugIn, sizeImage, qual, pSceneServer, pNotify, dibOut);
}

BEGIN_MESSAGE_MAP(CCustomContentSection, CRhRdkMaterialUISection_MFC)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CCustomContentSection::CCustomContentSection()
	:
	CRhRdkMaterialUISection_MFC(IDD)
{
}

void CCustomContentSection::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_placeholder);
}

BOOL CCustomContentSection::OnInitDialog()
{
	__super::OnInitDialog();

	DisplayData();

	return FALSE; // Prevent focus being set to a control; this would interfere with the RDK UI.
}

#if 0

class CPreviewGeometry : public CRhRdkPreviewGeometry
{
public:
	virtual UUID TypeId(void) const;
	virtual const wchar_t* XmlTag(void) const { return SS_PG_SPHERE; }
	virtual CRhRdkPreviewSceneElement* Duplicate(void) const { return new CPreviewGeometry; }
	virtual void SetUpPreview(CRhRdkPreviewSceneServer& ss, const CRhRdkContent& c, bool bCopy) const;
};

UUID CPreviewGeometry::TypeId(void) const
{
	static const UUID uuid = 
	{
		0xff3dce7b, 0xb305, 0x421a, { 0xb1, 0x76, 0xb4, 0xee, 0xb7, 0xf4, 0xf9, 0x31 }
	};

	return uuid;
}

void CPreviewGeometry::SetUpPreview(CRhRdkPreviewSceneServer& ss, const CRhRdkContent& c, bool bCopy) const
{
	ss.AddObject(CRhRdkPreviewSceneServer::geomSphere, c, bCopy);
}

class CPreviewBackground : public CRhRdkPreviewBackground
{
public:
	virtual UUID TypeId(void) const override;
	virtual const wchar_t* XmlTag(void) const override { return SS_PB_NONE; }
	virtual CRhRdkPreviewSceneElement* Duplicate(void) const override { return new CPreviewBackground; }
	virtual void SetUpPreview(CRhRdkPreviewSceneServer& ss, const UUID uuidRdkDoc) const override;
};

class CPreviewLighting : public CRhRdkPreviewLighting
{
public:
	virtual UUID TypeId(void) const override;
	virtual const wchar_t* XmlTag(void) const override { return SS_PL_SIMPLE; }
	virtual CRhRdkPreviewSceneElement* Duplicate(void) const override { return new CPreviewLighting; }
	virtual void SetUpPreview(CRhRdkPreviewSceneServer& ss) const override;
};

#endif

void CCustomContentSection::DisplayData(void)
{
	__super::DisplayData();

	const auto con = Controller();
	if (!con)
		return;

	auto* pPlugIn = RhRdkFindCurrentRenderPlugIn();
	if (nullptr == pPlugIn)
		return;

	// This demonstrates how to generate a preview of the selected content. We only attempt this if
	// there is exactly one content selected in the editor.
	CRhRdkEditableContentArray aContent(*con, false);
	if (1 != aContent.Count())
		return;

	// We will create the preview in m_dib and draw it in OnPaint().
	m_dib.DestroyDib();

	// The placeholder is a picture object that just gives the rectangle for the preview within the section.
	CRect r;
	m_placeholder.GetWindowRect(r);

	// This creates the preview. For this simple demo, we pass a null scene server because we don't want to
	// be concerned with the complexity of custom scene servers here. Passing null causes the RDK to get a
	// default scene server by calling the content's NewPreviewSceneServer() method.
	// We also pass a null callback and render only one full-quality preview.
	CRhinoDib dib;
	ON_2iSize size(r.Width(), r.Height());

	const auto* pContent = aContent[0];

//	CPreviewGeometry g;
//	CPreviewBackground b;
//	CPreviewLighting l;
//	CRhRdkSSData data(&g, &b, &l, CRhRdkSSData::Usage::Synchronous);
//	auto* pSS = pContent->NewPreviewSceneServer(data);

	if (pContent->CreatePreview(*pPlugIn, size, RhRdkPreviewQuality::Quick, nullptr, nullptr, dib))
	{
		m_dib = dib;

		Invalidate();
	}
}

void CCustomContentSection::OnPaint()
{
	CPaintDC dc(this);

	if (m_dib.IsAllocated())
	{
		// Draw the preview dib at the position specified by the placeholder.
		CRect r;
		m_placeholder.GetWindowRect(r);
		ScreenToClient(r);
		m_dib.Draw(dc.m_hDC, ON_4iRect(r.left, r.top, r.right, r.bottom));
	}
}

ON_wString CCustomContentSection::Caption(bool bAlwaysEnglish) const
{
	return L"Custom Section";
}

UUID CCustomContentSection::Uuid(void) const
{
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	//
	// TODO: If you base your plug-in on this code, you MUST change this UUID.
	//
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!
	// TODO: !!!!!!!!!!! DO NOT REUSE THIS UUID !!!!!!!!!!!!

	static const UUID uuid =
	{
		0xb2c3ae20, 0xbded, 0x40ac, { 0xa8, 0x91, 0xd, 0x7, 0x3b, 0xe8, 0xcb, 0x74 }
	};

	return uuid;
}
