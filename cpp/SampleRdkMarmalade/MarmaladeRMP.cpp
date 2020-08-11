
#include "stdafx.h"
#include "MarmaladeRMP.h"
#include "MarmaladePlugIn.h"
#include "MarmaladeRdkPlugIn.h"

CMarmaladeRMP::CMarmaladeRMP(void)
{
}

CMarmaladeRMP::~CMarmaladeRMP(void)
{
}

UUID CMarmaladeRMP::PlugInId(void) const
{
	return MarmaladePlugIn().PlugInID();
}

UUID CMarmaladeRMP::ProviderId(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {E394777B-AA92-419e-B7DE-2EB822B4E03C}
		0xe394777b, 0xaa92, 0x419e, { 0xb7, 0xde, 0x2e, 0xb8, 0x22, 0xb4, 0xe0, 0x3c }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

ON_wString CMarmaladeRMP::Name(void) const
{
	 return L"Fake Flat People";
}

bool CMarmaladeRMP::WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc&,
                                        const UUID&, const CDisplayPipelineAttributes*) const
{
	if (nullptr == pObject)
		return false;

	ON_wString s;
	if (!pObject->Attributes().GetUserString(L"TestRMP", s))
		return false;

	return true;
}

ON_BoundingBox CMarmaladeRMP::BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
                                          const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const
{
	return ::RMPBoundingBoxImpl(*this, vp, pObject, doc, uuidRequestingPlugIn, pAttributes);
}

static void SetTextureSlot(CRhRdkBasicMaterial* pBasicMaterial, const ON_Material& mat,
                           CRhRdkMaterial::ChildSlotUsage usage, ON_Texture::TYPE texType)
{
	int iIndex = mat.FindTexture(nullptr, texType);
	if (iIndex >= 0)
	{
		const ON_Texture& tex = mat.m_textures[iIndex];
		CRhRdkBasicMaterial::CTextureSlot slot = pBasicMaterial->TextureSlot(usage);
		slot.SetOn(tex.m_bOn);
		slot.SetFilterOn((tex.m_magfilter == ON_Texture::FILTER::linear_filter) && (tex.m_minfilter == ON_Texture::FILTER::linear_filter));
		slot.SetAmount((tex.m_mode == ON_Texture::MODE::decal_texture) ? 1.0 : tex.m_blend_constant_A);
		pBasicMaterial->SetTextureSlot(usage, slot);
	}
}

static inline ON_3fPoint To3f(const ON_3dPoint& p)
{
	return ON_3fPoint((float)p.x, (float)p.y, (float)p.z);
}

bool CMarmaladeRMP::BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, const CRhinoDoc& doc,
                                      IRhRdkCustomRenderMeshes& crmInOut, const CDisplayPipelineAttributes* pAttributes,
                                      bool bWillBuildCustomMeshCheck) const
{
	const auto* pObject = crmInOut.Object();
	if (nullptr == pObject)
		return false;

	if (bWillBuildCustomMeshCheck && !WillBuildCustomMesh(vp, pObject, doc, uuidRequestingPlugIn, pAttributes))
		return false;

	const auto* pPoint = CRhinoPointObject::Cast(pObject);
	if (nullptr == pPoint)
		return false;

	crmInOut.SetAutoDeleteMeshesOn();
	crmInOut.SetAutoDeleteMaterialsOn();

	const ON_3dPoint pt = (ON_3dPoint)pPoint->Point();
	const ON_3dVector vNormal = vp.CameraDirection();
	const ON_3dVector vY = vp.CameraUp();
	const ON_3dVector vX = ON_CrossProduct(vNormal, vY);
	const ON_Plane plane(pt, vX, vY);

	CRhinoMeshDensity md;
	md.m_x = md.m_y = md.m_z = 1;
	ON_Mesh* pMesh = RhinoMeshPlane(plane, ON_Interval(0.0, 10.0), ON_Interval(0.0, 20.0), md);

	auto* pBasicMaterial = new CRhRdkBasicMaterial;

	wchar_t buf[MAX_PATH+1];
	::GetWindowsDirectory(buf, MAX_PATH);
	ON_wString sFilename = buf;
	sFilename += L"\\Coffee Bean.bmp";

	CRhinoDib dib;
	dib.ReadFromFile(doc.RuntimeSerialNumber(), sFilename);
	auto* pTexture = ::RhRdkNewDibTexture(&dib, &doc);
	if (nullptr != pTexture)
	{
		pBasicMaterial->SetChild(pTexture, RDK_BASIC_MAT_BITMAP_TEXTURE);
	}

	const auto usage = CRhRdkMaterial::ChildSlotUsage::Diffuse;

	auto ts = pBasicMaterial->TextureSlot(usage);
	ts.SetOn(true);
	pBasicMaterial->SetTextureSlot(usage, ts);

	crmInOut.Add(pMesh, pBasicMaterial);

	return true;
}
