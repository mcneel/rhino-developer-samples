
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

	const ON_Xform translationXform = ON_Xform::TranslationTransformation(0.0, 0.0, user_data->Amount());

	//We're just going to move the meshes - so make a copy of the previous meshes...
	auto out = previousMeshes->MakeCopy();

	if (!out)
		return nullptr;

	//...and then add the translation transform to all of the instances.
	for (auto& pInstance : *out)
	{
		if (pInstance)
		{
			pInstance->Transform(translationXform);
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
	//This is super quick,. so no need to report any progress
	return nullptr;
}

void* CSampleCRMP::EVF(const wchar_t*, void*)
{
	return nullptr;
}
