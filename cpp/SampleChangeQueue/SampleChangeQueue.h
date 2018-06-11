#pragma once

class SampleChangeQueue : public RhRdk::Realtime::ChangeQueue
{
public:
	SampleChangeQueue(const CRhinoDoc& doc, const UUID& uuidPlugInId, const ON_3dmView& view, const CDisplayPipelineAttributes* pAttributes, bool bRespectDisplayAttributes, bool bNotifyChanges);

	/****** Implementing Apply* functions to collect data *****/
	void ApplyViewChange(const ON_3dmView& view) const override;

	void ApplyDynamicObjectTransforms(const ON_SimpleArray<const DynamicObject*>&) const override;
	void ApplyDynamicLightChanges(const ON_SimpleArray<const ON_Light*>&) const override;

	void ApplyMeshChanges(const ON_SimpleArray<const UUID*>& deleted, const ON_SimpleArray<const Mesh*>& added) const override;
	void ApplyMeshInstanceChanges(const ON_SimpleArray<ON__UINT32>& deleted, const ON_SimpleArray<const MeshInstance*>& added) const override;

	void ApplySunChanges(const ON_Light& sun) const override;
	void ApplySkylightChanges(const Skylight&) const override;
	void ApplyLightChanges(const ON_SimpleArray<const Light*>&) const override;
	void ApplyMaterialChanges(const ON_SimpleArray<const Material*>& md) const override;

	void ApplyRenderSettingsChanges(const ON_3dmRenderSettings& rs) const override;
	void ApplyEnvironmentChanges(IRhRdkCurrentEnvironment::Usage) const override;
	void ApplyGroundPlaneChanges(const GroundPlane&) const override;
	void ApplyLinearWorkflowChanges(const IRhRdkLinearWorkflow& lw) const override;

	void ApplyClippingPlaneChanges(const ON_SimpleArray<const UUID*>& deleted, const ON_SimpleArray<const ClippingPlane*>& added) const override;
	void ApplyDynamicClippingPlaneChanges(const ON_SimpleArray<const ClippingPlane*>& planes) const override;

	/* Implement notification functions to react to changes in the model*/

	void NotifyBeginUpdates(void) const override;
	void NotifyEndUpdates(void) const override;

	void NotifyDynamicUpdatesAreAvailable(void) const override;

	/* Return true if we want the original object */
	bool ProvideOriginalObject(void) const override;
	/* Let the changequeue know if we want baking, and for what*/
	eRhRdkBakingFunctions BakeFor(void) const override;
	/* If we want baking tell the changequeue at what size */
	int BakingSize(const CRhinoObject& object, const CRhRdkMaterial& material, ON_Texture::TYPE type) const override;

public:
	// print out accumulated stats
	void PrintStats() const;
	// call Flush(true) if we have changes; Return true if there were any signalled, false otherwise.
	bool HandleFlush();

private:
	class Impl {
	public:
		int m_iVertexCount = 0;
		int m_iMeshCount = 0;
		int m_iInstanceCount = 0;
		int m_iLightCount = 0;
		int m_iSunCount = 0;
		int m_iSkylightCount = 0;
		int m_iMaterialChangeCount = 0;

		bool m_bChangesReady = false;
	};
	Impl* m_pImpl;
};

