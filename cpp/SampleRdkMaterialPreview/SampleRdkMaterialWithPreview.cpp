
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

class CPreviewGeometry : public CRhRdkPreviewGeometry
{
public:
	virtual UUID TypeId(void) const { static const UUID uuid = { 0xff3dce7b, 0xb305, 0x421a, { 0xb1, 0x76, 0xb4, 0xee, 0xb7, 0xf4, 0xf9, 0x31 } }; return uuid; }
	virtual const wchar_t* XmlTag(void) const { return L"my-geometry"; }
	virtual CRhRdkPreviewSceneElement* Duplicate(void) const { return new CPreviewGeometry; }
	virtual void SetUpPreview(IRhRdkPreviewSceneServerEx& ss, const CRhRdkContent& c, bool bCopy) const;
};

void CPreviewGeometry::SetUpPreview(IRhRdkPreviewSceneServerEx& ss, const CRhRdkContent& c, bool bCopy) const
{
	auto* pMat = dynamic_cast<const CRhRdkMaterial*>(&c);
	if (nullptr == pMat)
		return;

	// Add a sphere to the preview scene.
	auto* pObject = ss.AddObject(IRhRdkPreviewSceneServer::geomSphere, *pMat, bCopy);

	// Add a torus to the preview scene.
	pObject = ss.AddObject(IRhRdkPreviewSceneServer::geomTorus, *pMat, bCopy);

	// In order to set the location, cast to IObjectEx. This was added after the SDK freeze.
	auto* pObjectEx = dynamic_cast<IRhRdkPreviewSceneServerEx::IObjectEx*>(pObject);
	if (nullptr != pObjectEx)
	{
		ON_3dPoint loc(-10.0, 17.0, 0.0);
		pObjectEx->SetLocation(loc);
	}

	// Add a cube to the preview scene.
	pObject = ss.AddObject(IRhRdkPreviewSceneServer::geomCuboid, *pMat, bCopy);

	// In order to set the location, cast to IObjectEx. This was added after the SDK freeze.
	pObjectEx = dynamic_cast<IRhRdkPreviewSceneServerEx::IObjectEx*>(pObject);
	if (nullptr != pObjectEx)
	{
		ON_3dPoint loc(-10.0, -17.0, 0.0);
		pObjectEx->SetLocation(loc);
	}
}

class CPreviewBackground : public CRhRdkPreviewBackground
{
public:
	virtual UUID TypeId(void) const override { static const UUID uuid = { 0x418f54ba, 0x7c96, 0x4638, { 0xb3, 0xd9, 0x3e, 0x6a, 0xea, 0x11, 0x40, 0x86 } }; return uuid; }
	virtual const wchar_t* XmlTag(void) const override { return L"my-background"; }
	virtual CRhRdkPreviewSceneElement* Duplicate(void) const override { return new CPreviewBackground; }
	virtual void SetUpPreview(IRhRdkPreviewSceneServerEx& ss, const UUID uuidRdkDoc) const override;
};

void CPreviewBackground::SetUpPreview(IRhRdkPreviewSceneServerEx& ss, const UUID uuidRdkDoc) const
{
	auto* pRdkDoc = CRhRdkDocument::Get(uuidRdkDoc);
	if (nullptr == pRdkDoc)
		return;

	auto* pEnv = ::RhRdkNewBasicEnvironment(pRdkDoc->RhinoDoc());
	pEnv->Initialize();
	pEnv->SetParameter(FS_ENV_BACKGROUND_COLOR, CRhRdkColor(80, 20, 130));

	const bool bCopyEnvironment = false;
	ss.SetEnvironment(pEnv, bCopyEnvironment, uuidRdkDoc);
}

class CPreviewLighting : public CRhRdkPreviewLighting
{
public:
	virtual UUID TypeId(void) const override { static const UUID uuid = { 0x80d847ac, 0x20e7, 0x417d, { 0x92, 0x33, 0xb1, 0x36, 0x3d, 0x5c, 0x47, 0xd3 } }; return uuid; }
	virtual const wchar_t* XmlTag(void) const override { return L"my-lighting"; }
	virtual CRhRdkPreviewSceneElement* Duplicate(void) const override { return new CPreviewLighting; }
	virtual void SetUpPreview(IRhRdkPreviewSceneServerEx& ss) const override;
};

//#define SIMPLE_LIGHTING

void CPreviewLighting::SetUpPreview(IRhRdkPreviewSceneServerEx& ss) const
{
	ON_Light light;

	light.SetStyle(ON::world_directional_light);
	light.SetIntensity(1.2);

#ifdef SIMPLE_LIGHTING
	light.SetDirection(-ON_3dVector::XAxis);
	ss.AddLight(light);
#else
	const auto az = ON_PI * 0.375;
	const auto ax = ON_PI * -0.25;

	ON_3dVector v1(ON_3dVector::XAxis);
	v1.Rotate( az, ON_3dVector::ZAxis);
	v1.Rotate( ax, ON_3dVector::XAxis);
	light.SetDirection(v1);
	light.SetDiffuse(ON_Color(255, 0, 0));
	ss.AddLight(light);

	ON_3dVector v2(ON_3dVector::XAxis);
	v2.Rotate( az, ON_3dVector::ZAxis);
	light.SetDirection(v2);
	light.SetDiffuse(ON_Color(0, 255, 0));
	ss.AddLight(light);

	ON_3dVector v3(ON_3dVector::XAxis);
	v3.Rotate( az, ON_3dVector::ZAxis);
	v3.Rotate(-ax, ON_3dVector::XAxis);
	light.SetDirection(v3);
	light.SetDiffuse(ON_Color(0, 0, 255));
	ss.AddLight(light);

	ON_3dVector v4(ON_3dVector::XAxis);
	v4.Rotate(ON_PI*-0.65, ON_3dVector::ZAxis);
	light.SetDirection(v4);
	light.SetDiffuse(ON_Color(255, 255, 255));
	ss.AddLight(light);
#endif

	ss.SetSkylighting(false);
}

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

	// This creates the preview.
	CRhinoDib dib;
	ON_2iSize size(r.Width(), r.Height());

	const auto* pContent = aContent[0];

	CPreviewGeometry g;
	CPreviewBackground b;
	CPreviewLighting l;
	CRhRdkSSData data(&g, &b, &l, CRhRdkSSData::Usage::Synchronous);
	auto* pSS = pContent->NewPreviewSceneServer(data);

	// If you want the RDK to get use default scene server by calling the content's NewPreviewSceneServer()
	// method, you can pass null instead of pSS. Here, we create our own custom scene server.

	if (pContent->CreatePreview(*pPlugIn, size, RhRdkPreviewQuality::Quick, pSS, nullptr, dib))
	{
		m_dib = dib;

		Invalidate();
	}

	delete pSS;
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
