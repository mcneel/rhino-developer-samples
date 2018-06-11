#include "stdafx.h"
#include "SampleChangeQueue.h"

SampleChangeQueue::SampleChangeQueue
(const CRhinoDoc& doc,
	const UUID& uuidPlugInId,
	const ON_3dmView& view,
	const CDisplayPipelineAttributes* pAttributes,
	bool bRespectDisplayAttributes,
	bool bNotifyChanges)
	: RhRdk::Realtime::ChangeQueue(doc, uuidPlugInId, view, pAttributes, bRespectDisplayAttributes, bNotifyChanges)
{
	m_pImpl = new SampleChangeQueue::Impl();
}

void SampleChangeQueue::ApplyViewChange(const ON_3dmView & view) const
{
	// received a view change
	// this is where you can collect the information about the view in Rhino
	// to which this changequeue instance is bound, if you need it.
}

void SampleChangeQueue::ApplyDynamicObjectTransforms(const ON_SimpleArray<const DynamicObject*>&) const
{

}

void SampleChangeQueue::ApplyDynamicLightChanges(const ON_SimpleArray<const ON_Light*>&) const
{
}

void SampleChangeQueue::ApplyMeshChanges(const ON_SimpleArray<const UUID*>& deleted, const ON_SimpleArray<const Mesh*>& added) const
{
	for (int i = 0; i < added.Count(); i++) {
		const RhRdk::Realtime::ChangeQueue::Mesh* m = added[i];

		// UuidId is the same for MeshInstance::MeshId()
		// meaning that this same ChangeQueue::Mesh with its
		// various parts can be used in multiple instances later on
		const ON_UUID& obid = m->UuidId();

		// access original object: m->Object();
		// a mesh can consist of multiple parts
		ON_SimpleArray<ON_Mesh*> meshes = m->Meshes();
		int iMeshesCount = meshes.Count();
		m_pImpl->m_iMeshCount += iMeshesCount;
		for (int im = 0; im < iMeshesCount; im++) {
			// note that the 'im' index corresponds to the MeshIndex
			// you get later from MeshInstance::MeshIndex()
			ON_Mesh* innerMesh = meshes[im];
			m_pImpl->m_iVertexCount += innerMesh->VertexCount();
		}
	}
}

void SampleChangeQueue::ApplyMeshInstanceChanges(const ON_SimpleArray<ON__UINT32>& deleted, const ON_SimpleArray<const MeshInstance*>& added) const
{
	m_pImpl->m_iInstanceCount += deleted.Count() + added.Count();
	for (int aidx = 0; aidx < added.Count(); aidx++) {
		const MeshInstance* mi = added[aidx];
		// now you can access the material id, retrieve a render material with it:
		const CRhRdkMaterial* rm = MaterialFromId(mi->MaterialId());
		// know what mesh parts at what indices 'belong' to an object
		const ON_UUID& obid = mi->MeshId();
		// the mesh index of the part in ChangeQueue::Mesh::Meshes()
		const int idx = mi->MeshIndex();
		// the mesh instance id can be used to identify the object.
		const int instanceidx = mi->InstanceId();

		// the tuple (instanceidx, idx) essentially defines a mesh uniquely within
		// an object container
	}
}

void SampleChangeQueue::ApplySunChanges(const ON_Light& sun) const
{
	// a change to the sun was recorded
	m_pImpl->m_iSunCount += 1;
}

void SampleChangeQueue::ApplySkylightChanges(const Skylight& sky) const
{
	// a change to the skylight was recorded
	m_pImpl->m_iSkylightCount += 1;
}

void SampleChangeQueue::ApplyLightChanges(const ON_SimpleArray<const Light*>& lightChanges) const
{
	m_pImpl->m_iLightCount += lightChanges.Count();
	for (int i = 0; i < lightChanges.Count(); i++) {
		const ChangeQueue::Light* light = lightChanges[i];
		
		// this tells us whether a light was added, deleted or otherwise modified
		const CRhinoEventWatcher::light_event event = light->Event();
		// this is the actual light data as ON_Light
		const ON_Light& lightData = light->LightData();
	}
}

void SampleChangeQueue::ApplyMaterialChanges(const ON_SimpleArray<const Material*>& md) const
{
	//
	for (int i = 0; i < md.Count(); i++) {
		const ChangeQueue::Material* mat = md[i];
		// the new material id
		const int matid = mat->MaterialId();
		// get the render material for the id
		const CRhRdkMaterial* rmat = MaterialFromId(matid);
		
		// This rmat is to be assigned to mesh identified by
		// the tuple (meshid, meshindex)
		const int meshid = mat->MeshInstanceId();
		const int idx = mat->MeshIndex();
		// now you can find your mesh instance in your own data
		// structure and assing this new material to it.
	}
}

void SampleChangeQueue::ApplyRenderSettingsChanges(const ON_3dmRenderSettings & rs) const
{
}

void SampleChangeQueue::ApplyEnvironmentChanges(IRhRdkCurrentEnvironment::Usage) const
{
}

void SampleChangeQueue::ApplyGroundPlaneChanges(const GroundPlane& gp) const
{
	// a change to the groundplane was recorded
}

void SampleChangeQueue::ApplyLinearWorkflowChanges(const IRhRdkLinearWorkflow & lw) const
{
}

void SampleChangeQueue::ApplyClippingPlaneChanges(const ON_SimpleArray<const UUID*>& deleted, const ON_SimpleArray<const ClippingPlane*>& added) const
{
}

void SampleChangeQueue::ApplyDynamicClippingPlaneChanges(const ON_SimpleArray<const ClippingPlane*>& planes) const
{
}

void SampleChangeQueue::NotifyBeginUpdates(void) const
{
	// The changequeue system is collecting changes, when done the
	// NotifyEndUpdates is called.
}

void SampleChangeQueue::NotifyEndUpdates(void) const
{
	// Updates have been collected, you can now flush these
	// to your implementation by doing a flush with bApplyChanges = true
	m_pImpl->m_bChangesReady = true;
}

void SampleChangeQueue::NotifyDynamicUpdatesAreAvailable(void) const
{
	// The changequeue is signalling a dynamic update is available. This
	// can happen for i.e. dragging of an object.
}

bool SampleChangeQueue::ProvideOriginalObject(void) const
{
	// Tell we want a copy of the original object (attributes). This we can access in ApplyMeshChanges.
	return true;
}

eRhRdkBakingFunctions SampleChangeQueue::BakeFor(void) const
{
	// right now we don't want to be bake anything. See the
	// eRhRdkBakingFunctions enumeration for all possible
	// bakeable data. Particularly useful for procedurals.
	return eRhRdkBakingFunctions::kNone;
}

int SampleChangeQueue::BakingSize(const CRhinoObject & object, const CRhRdkMaterial & material, ON_Texture::TYPE type) const
{
	// not baking, so just returnin 0 here. If you do bake it is probably
	// a good idea to bake big enough for good resolution procedurals.
	return 0;
}

void SampleChangeQueue::PrintStats() const
{
	RhinoApp().Print(L"Accumulated stats:\n");
	RhinoApp().Print(L"meshes: %d\n", m_pImpl->m_iMeshCount);
	RhinoApp().Print(L"vertices: %d\n", m_pImpl->m_iVertexCount);
	RhinoApp().Print(L"instances: %d\n", m_pImpl->m_iInstanceCount);
	RhinoApp().Print(L"lights: %d\n", m_pImpl->m_iLightCount);
}

bool SampleChangeQueue::HandleFlush()
{
	if (m_pImpl && m_pImpl->m_bChangesReady) {
		Flush(true);
		m_pImpl->m_bChangesReady = false;
		return true;
	}
	return false;
}
