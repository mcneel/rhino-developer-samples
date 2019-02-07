#pragma once
class CustomChangeQueue : public RhRdk::Realtime::ChangeQueue
{
public:
	CustomChangeQueue(const CRhinoDoc& doc, const ON_3dmView& view);

	eRhRdkBakingFunctions BakeFor(void) const override;
	//Default returns 2048
	int BakingSize(const CRhinoObject& object, const CRhRdkMaterial& material, ON_Texture::TYPE channel) const override;
	bool ProvideOriginalObject(void) const override;

	void ApplyViewChange(const ON_3dmView& view) const override;
	void ApplyMeshChanges(const ON_SimpleArray<const UUID*>& deleted, const ON_SimpleArray<const Mesh*>& addedOrChanged) const override;
	void ApplyMeshInstanceChanges(const ON_SimpleArray<ON__UINT32>& deleted, const ON_SimpleArray<const MeshInstance*>& addedOrChanged) const override;
	void ApplySunChanges(const ON_Light& sun) const override;
	void ApplySkylightChanges(const Skylight& skylight) const override;
	void ApplyLightChanges(const ON_SimpleArray<const Light*>&) const override;
	void ApplyEnvironmentChanges(IRhRdkCurrentEnvironment::Usage usage) const override;
	void ApplyGroundPlaneChanges(const GroundPlane&) const override;
};

