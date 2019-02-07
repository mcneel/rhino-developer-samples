#include "stdafx.h"
#include "CustomChangeQueue.h"

// we are not posing as a render plug-in, nor are we interested in tracking changes to
// display attributes
CustomChangeQueue::CustomChangeQueue(const CRhinoDoc& doc, const ON_3dmView& view)
	: RhRdk::Realtime::ChangeQueue(doc, ON_nil_uuid, view, nullptr, false, false)
{
}

eRhRdkBakingFunctions CustomChangeQueue::BakeFor(void) const {
	// baking can be done for different types of mappings. This will
	// bake textures for all cases so you don't have to do that yourself.
	return eRhRdkBakingFunctions::kAll;
}

int CustomChangeQueue::BakingSize(const CRhinoObject& object, const CRhRdkMaterial& material, ON_Texture::TYPE channel) const
{
	// size of texture to generate - one side.
	return 512;
}
bool CustomChangeQueue::ProvideOriginalObject(void) const {
	// Return true from this function when you want to also have access to the original CRhinoObject
	// when receiving mesh data. This is useful for when you have custom UserData on objects
	// that you want access to.
	// If you don't have the need for the original objects just return false;
	return true;
}

void CustomChangeQueue::ApplyViewChange(const ON_3dmView& view) const {
	// This is the latest known transformation of the view used to set up
	// this CustomChangeQueue. On CreateWorld that would be the 'initial'
	// state. Use this information if you want to set up a camera in
	// another format.
}
void CustomChangeQueue::ApplyMeshChanges(const ON_SimpleArray<const UUID*>& deleted, const ON_SimpleArray<const Mesh*>& addedOrChanged) const {
	// We receive all mesh data. This is purely the mesh data. It is only in ApplyMeshInstanceChanges that we really find out what
	// objects exist in the scene. There meshes are bound to their instances.

	// For this sample code we will see only one ApplyMeshChanges call - the very first one from CreateWorld. This means there won't be
	// any deleted meshes yet only added meshes.
	RhinoApp().Print("ApplyMeshChanges: %u deletions, %u additions/changes\n", deleted.Count(), addedOrChanged.Count());
	for (int i = 0; i < addedOrChanged.Count(); i++) {
		const Mesh* m = addedOrChanged[i];
		// The ON_Mesh*'s for this mesh can be accessed with
		const ON_SimpleArray<ON_Mesh*>& onmeshes = m->Meshes();
		char muuidstr[37];
		char* muuidstrp = ON_UuidToString(m->UuidId(), muuidstr);
		char ouuidstr[37];
		char* ouuidstrp = ON_UuidToString(m->Object()->Id(), ouuidstr);
		RhinoApp().Print("\tmesh %s consists of %u meshes. Original object %s %s\n", muuidstrp, onmeshes.Count(), m->Object()->Name().Array(), ouuidstrp);
		for (int mi = 0; mi < onmeshes.Count(); mi++) {
			const ON_Mesh* amesh = onmeshes[mi];
			// pull out all vertices, faces, normals, etc using the ON_Mesh API.

			// NOTE: it is crucial to note that the mi index here is the mesh index that will be used in ApplyMeshInstanceChanges.
			// here multiple ON_Mesh* can exist for one object when for instance through subobject selection different parts have
			// been assigned different materials. Think a _Box added, then one side subselected and assigned a material while the
			// rest still has the default material.

			// NOTE2: in this stage your mesh data will not carry information about assigned materials - that is done in ApplyMeshInstances
			// so more information there.
		}
	}
}
void CustomChangeQueue::ApplyMeshInstanceChanges(const ON_SimpleArray<ON__UINT32>& deleted, const ON_SimpleArray<const MeshInstance*>& addedOrChanged) const {
	// Now that we have received all our mesh data we get all the instances that use the meshes. All these mesh instances are
	// processed from normal and block instance objects alike - so you shouldn't have to worry directly about what block instance 
	// a mesh is from.
	// Again, on the first call there won't be any deleted instances.

	// Note that the deleted array is an array with ON__UINT32, which is the identifier for mesh instances.

	RhinoApp().Print("ApplyMeshInstanceChanges %u deletions and %u additions/changes\n", deleted.Count(), addedOrChanged.Count());
	for (int i = 0; i < addedOrChanged.Count(); i++) {
		const MeshInstance* instance = addedOrChanged[i];
		// now we get an instance. It tells you its identifier InstanceId, the mesh UUID that is connected with this
		// the mesh index and the transform for this instance. Additionally you'll find the MaterialId for this instance.
		// Also for non-blockinstance objects you'll get here. There'll be then just one mesh instance using a MeshId. In that
		// case the transform is identity, since Rhino mesh data is always in world, except for block instances.

		// realization of a mesh(part) in the world
		ON__UINT32 iid = instance->InstanceId();
		ON_UUID mid = instance->MeshId();
		int meshindex = instance->MeshIndex();
		ON_Xform xform = instance->InstanceXform();
		ON_3dPoint p = ON_3dPoint::Origin;

		p.Transform(xform);

		// the material ID can be used to query the actual material from the ChangeQueue. In Rhino you can have the situation where
		// the same mesh has different materials. This happens especially with block instances. To illustrate: In Rhino add a _Box.
		// Set the box material to "Use Object Parent". Now create a _Block out of the box. Duplicate the instance that is placed in
		// the location of the box. Add two materials to the material editor and assign each block instance one.
		// It is here that you'll get for the same mesh (MeshId + MeshIndex) a different MaterialId.
		int materialId = instance->MaterialId();

		// Get the actual material that is used in Rhino for this mesh instance.
		// The materialId is the same as the render hash.
		const CRhRdkMaterial* rdkMaterial = MaterialFromId(materialId);

		char midstr[37];
		char* midstrp = ON_UuidToString(mid, midstr);
		RhinoApp().Print("\tMesh instance %u using mesh %s + %u at world (%f,%f,%f), using material %u\n", iid, midstrp, meshindex, p.x, p.y, p.z, materialId);
		RhinoApp().Print("\t\t%f %f %f %f\n", xform[0][0], xform[0][1], xform[0][2], xform[0][3]);
		RhinoApp().Print("\t\t%f %f %f %f\n", xform[1][0], xform[1][1], xform[1][2], xform[1][3]);
		RhinoApp().Print("\t\t%f %f %f %f\n", xform[2][0], xform[2][1], xform[2][2], xform[2][3]);
		RhinoApp().Print("\t\t%f %f %f %f\n", xform[3][0], xform[3][1], xform[3][2], xform[3][3]);
	}
	RhinoApp().Print("End of ApplyMeshInstances");
}
void CustomChangeQueue::ApplySunChanges(const ON_Light& sun) const {
	// The sun is an ON_Light, but is signalled separately. If no sun is enabled on CreateWorld this won't be called.
}
void CustomChangeQueue::ApplySkylightChanges(const Skylight& skylight) const {
	// Get the skylight information: whether a custom environment is set, whether the
	// skylight is on, and the wanted intensity for shadows
	RhinoApp().Print("The skylight is %s\n", skylight.On() ? "ON" : "OFF");
}
void CustomChangeQueue::ApplyLightChanges(const ON_SimpleArray<const Light*>& lights) const {
	// for each other light you'll find an entry in the lights array
}
void CustomChangeQueue::ApplyEnvironmentChanges(IRhRdkCurrentEnvironment::Usage usage) const {
	// environments will be signalled with this. First a call for Background, then for Skylight and
	// finally for Reflection
}
void CustomChangeQueue::ApplyGroundPlaneChanges(const GroundPlane&) const {
	// Rhino features a ground plane. This isn't a geometry by itself, so if you are
	// exporting somewhere that doesn't have a built-in ground plane feature you
	// could here create one using Rhino SDK functions.
}
