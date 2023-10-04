
// ANDY: Search for [ANDYLOOK] -- 3 places.

#include "stdafx.h"
#include "SampleCRMP.h"
#include "SampleCustomRenderMeshPlugIn.h"
#include "SampleCustomRenderMeshUserData.h"

CSampleCRMP::CSampleCRMP(void)
{
}

CSampleCRMP::~CSampleCRMP(void)
{
}

UUID CSampleCRMP::PlugInId(void) const
{
	return SampleCustomRenderMeshPlugIn().PlugInID();
}

UUID CSampleCRMP::ProviderId(void) const
{
	static const UUID uuid =
	{
		0x5f3b9be4, 0x4663, 0x450c,{ 0x86, 0xde, 0x9, 0x86, 0x95, 0xa0, 0x10, 0x1e }
	};

	return uuid;
}

std::vector<ObjectId> CSampleCRMP::NonObjectIds(void) const
{
	return std::vector<ObjectId>();
}

UUID CSampleCRMP::InterfaceId(void) const
{
	return IRenderMeshProvider::InterfaceId();
}

ON_wString CSampleCRMP::Name(void) const
{
	const static ON_wString wszName = L"SampleCRMP";
	return wszName;
}

CRhRdkVariant CSampleCRMP::GetParameter(const CRhinoDoc& doc, const ObjectId& object,
										const wchar_t* wszParamName) const
{
	const auto* pObject = doc.LookupObject(object);
	if (nullptr == pObject)
		return CRhRdkVariant::Null();

	// TODO: Return your value for this parameter if possible.
	return CRhRdkVariant(); // ...
}

void CSampleCRMP::SetParameter(const CRhinoDoc& doc, const ObjectId& object,
                               const wchar_t* wszParamName, const CRhRdkVariant& value)
{
	auto* pObject = doc.LookupObject(object);
	if (nullptr == pObject)
		return;

	// TODO: Set your value for this parameter if possible.
}

bool CSampleCRMP::HasCustomRenderMeshes(ON::mesh_type mt, const ON_Viewport& vp, const CRhinoDoc& doc,
                  const ObjectId& objectId, ON__UINT32& flags, const CRhinoPlugIn* requestingPlugIn,
                  const CDisplayPipelineAttributes* pAttributes) const
{
	const auto* pObject = doc.LookupObject(objectId);
	if (nullptr == pObject)
		return false;

	// Read sample custom render mesh user data from the object attributes.
	auto* ud = pObject->Attributes().GetUserData(SampleCustomRenderMeshUserDataUuid);
	const auto* pUserData = CSampleCustomRenderMeshUserData::Cast(ud);

	// If object doesn't have this type of user data then it doesn't have custom render mesh.
	if (pUserData == nullptr)
		return false;

	return true;
}

std::shared_ptr<const IRenderMeshes> CSampleCRMP::RenderMeshes(
                ON::mesh_type mp, const ON_Viewport& vp, const CRhinoDoc& doc, const ObjectId& objectId,
                const CRhRdkObjectAncestry& ancestry, ON__UINT32& flags,
                std::shared_ptr<const IRenderMeshes> previousMeshes, const CRhinoPlugIn* requestingPlugIn,
                const CDisplayPipelineAttributes* pAttributes) const
{
	if (!previousMeshes)
		return nullptr;

	const auto* object = doc.LookupObject(objectId);
	if (object == nullptr)
		return nullptr;

	if (!HasCustomRenderMeshes(mp, vp, doc, objectId, flags, requestingPlugIn, pAttributes))
		return nullptr;

	// Read sample custom render mesh user data from the object attributes
	// If object doesn't have this type of user data then it doesn't have custom render meshes.
	auto* ud = object->Attributes().GetUserData(SampleCustomRenderMeshUserDataUuid);
	const auto* user_data = CSampleCustomRenderMeshUserData::Cast(ud);
	if (user_data == nullptr)
		return false;

	auto runningHash = previousMeshes->Hash();
	const double amount = user_data->Amount();
	runningHash = ON_CRC32(runningHash, sizeof(amount), &amount);

	// Check the cache.
	if (IManager::Flags::DisableCaching != (flags & IManager::Flags::DisableCaching))
	{
		const auto* pTrackingRecord = m_tracking.Tracker(doc).Record(objectId);
		if (nullptr != pTrackingRecord)
		{
			if (pTrackingRecord->Hash() == runningHash)
			{
				return pTrackingRecord->Primitives();
			}
		}
	}

	// [ANDYLOOK] I don't know what I'm supposed to do here.
	//
	// This is what Thickening does:
	//
	//ON_SimpleArray<int> aSubObjectIndices;
	//ON_Mesh renderMeshUnion;
	//
	//int rmi = 0;
	//int i = 0;
	//for (const auto& rm : *previousMeshes)
	//{
	//	ON_Mesh mesh(*rm->Geometry().Mesh());
	//
	//	mesh.ConvertQuadsToTriangles();
	//
	//	renderMeshUnion.Append(mesh);
	//
	//	aSubObjectIndices.SetCapacity(renderMeshUnion.FaceCount());
	//	aSubObjectIndices.SetCount(renderMeshUnion.FaceCount());
	//
	//	const int iFaceCount = mesh.FaceCount();
	//
	//	for (int fi = 0; fi < iFaceCount; fi++, i++)
	//	{
	//		aSubObjectIndices[i] = rmi;
	//	}
	//
	//	rmi++;
	//}

	auto out = std::shared_ptr<IRenderMeshes>(IRenderMeshes::New(doc, objectId, ProviderId(), runningHash));

	ON_Xform translationXform = ON_Xform::TranslationTransformation(0.0, 0.0, user_data->Amount());

	ON_SimpleArray<const ON_Mesh*> renderMeshes;
	object->GetMeshes(ON::render_mesh, renderMeshes);

	auto it = previousMeshes->begin();

	for (int i = 0; i < renderMeshes.Count(); i++)
	{
		const auto* rm = renderMeshes[i];
		if (rm == nullptr)
			continue;

		auto custom_mesh = std::make_shared<ON_Mesh>(*rm);
		if (custom_mesh)
		{
			custom_mesh->Transform(translationXform); // Is this needed?

			// [ANDYLOOK]
			// This was here before I tried to change it but I can't figure out what I should change it to.
			//
			//crmInOut.SetUseObjectsMappingChannels(true);
			//crmInOut.SetInstanceTransform(ON_Xform::IdentityTransformation);
			//crmInOut.SetAutoDeleteMeshesOn();
			//crmInOut.SetAutoDeleteMaterialsOn();

			std::shared_ptr<IRenderMeshes::IInstance> pPrevious = nullptr; // Is this needed?
			if (std::end(*previousMeshes) != it && nullptr != *it)
			{
				const auto pPrevious = *it; // Is this right?
				out->AddInstance(IRenderMeshes::IInstance::New(custom_mesh, *pPrevious));
				it++;
			}
			else
			{
				out->AddInstance(IRenderMeshes::IInstance::New(custom_mesh, *object));
			}
		}
	}

	// Add to the cache.
	if (IManager::Flags::DisableCaching != (flags & IManager::Flags::DisableCaching))
	{
		m_tracking.Tracker(doc).Set(out);
	}

	return out;
}

std::unique_ptr<IRenderMeshProvider::IProgress> CSampleCRMP::Progress(
                const CRhinoDoc&, const std::vector<ObjectId>* pOptionalObjects) const
{
	// TODO: [ANDYLOOK]
	return nullptr;
}

void* CSampleCRMP::EVF(const wchar_t*, void*)
{
	return nullptr;
}
