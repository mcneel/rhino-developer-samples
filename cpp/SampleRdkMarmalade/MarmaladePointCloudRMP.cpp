
#include "stdafx.h"
#include "MarmaladePointCloudRMP.h"

CMarmaladePointCloudRMP::CMarmaladePointCloudRMP(void)
{
}

CMarmaladePointCloudRMP::~CMarmaladePointCloudRMP(void)
{
}

UUID CMarmaladePointCloudRMP::PlugInId(void) const
{
	return MarmaladePlugIn().PlugInID();
}

UUID CMarmaladePointCloudRMP::ProviderId(void) const
{
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	static const UUID uuid =
	{
		// {399353C8-45B8-494b-B447-804786E82492}
		0x399353c8, 0x45b8, 0x494b, { 0xb4, 0x47, 0x80, 0x47, 0x86, 0xe8, 0x24, 0x92 }
	};
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!
	// !!!! DO NOT REUSE THIS UUID !!!!

	return uuid;
}

ON_wString CMarmaladePointCloudRMP::Name(void) const
{
	 return L"Point Cloud";
}

bool CMarmaladePointCloudRMP::WillBuildCustomMesh(const ON_Viewport& vp, 
												  const CRhinoObject* pObject, 
												  const UUID& uuidRequestingPlugIn, 
												  IRhRdkCustomRenderMeshManager::eMeshType type) const
{
	if (NULL == pObject)
		return false;

	static CRhinoPointCloudObject pc;

	if (!pObject->IsKindOf(pc.ClassId()))
		return false;

	return true;
}

ON_BoundingBox CMarmaladePointCloudRMP::BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const UUID& uuidRequestingPlugIn, IRhRdkCustomRenderMeshManager::eMeshType type) const
{
	return ::RMPBoundingBoxImpl(*this, vp, pObject, uuidRequestingPlugIn, type);
}

bool CMarmaladePointCloudRMP::BuildCustomMeshes(const ON_Viewport& vp, 
												const UUID& uuidRequestingPlugIn, 
												IRhRdkCustomRenderMeshes& mesh, 
												IRhRdkCustomRenderMeshManager::eMeshType type) const
{
	if (!WillBuildCustomMesh(vp, mesh.Object(), uuidRequestingPlugIn, type))
		return false;

	// Guaranteed a point cloud by this point.
	const CRhinoObject* pObject = mesh.Object();
	
	const ON_PointCloud* pPC = static_cast<const ON_PointCloud*>(pObject->Geometry());

	const int iPointCount = pPC->PointCount();

	for (int i = 0; i < iPointCount; i++)
	{
		ON_COMPONENT_INDEX ci;
		ci.m_type = ON_COMPONENT_INDEX::pointcloud_point;
		ci.m_index = i;

		ON_3dPoint pt = pPC->Point(ci);

		ON_Sphere sphere;
		sphere.plane = ON_Plane(pt, ON_3dVector::UnitVector(2));
		sphere.radius = 0.1;
		mesh.Add(sphere, NULL);
	}

	return true;
}
