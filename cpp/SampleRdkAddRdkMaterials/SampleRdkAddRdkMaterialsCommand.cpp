
#include "stdafx.h"

static class CSampleRdkAddRdkMaterials : public CRhinoCommand
{
protected:
	virtual UUID CommandUUID() override { static const UUID uuid = { 0x43991fff, 0x967a, 0x44d8, { 0x86, 0x60, 0x2f, 0x66, 0xc7, 0x40, 0xf2, 0x12 } }; return uuid; }
	virtual const wchar_t* EnglishCommandName() override { return L"SampleRdkAddRdkMaterials"; }
	virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
}
theSampleRdkAddRdkMaterials;

CRhinoCommand::result CSampleRdkAddRdkMaterials::RunCommand(const CRhinoCommandContext& context)
{
	auto* pDoc = context.Document();
	if (nullptr == pDoc)
		return failure;

	// Create an ON_Material.
	ON_Material mat;
	ON_Color col(255, 0, 0);
	mat.SetDiffuse(col);
	mat.SetName(L"Sample");

	// Create a basic material using the ON_Material.
	auto* pMaterial = ::RhRdkNewBasicMaterial(mat, pDoc);
	if (nullptr == pMaterial)
		return failure;

	// Attach the basic material to the document.
	auto& contents = pDoc->Contents().BeginChange(RhRdkChangeContext::Program);
	contents.Attach(*pMaterial);
	contents.EndChange();

	// Set the ON_Material's material plug-in id to universal.
	mat.SetMaterialPlugInId(uuidUniversalRenderEngine);

	// Set the ON_Material's RDK material instance id to the RDK material.
	mat.SetRdkMaterialInstanceId(pMaterial->InstanceId());

	// Add the ON_Material to the material table.
	const int matIndex = pDoc->m_material_table.AddMaterial(mat);
	if (matIndex < 0)
		return failure;

	// Create a sphere object and assign the material to it.
	ON_3dmObjectAttributes attr;
	attr.m_material_index = matIndex;
	attr.SetMaterialSource(ON::material_from_object);

	const ON_Sphere sphere(ON_3dPoint::Origin, 8);
	ON_NurbsSurface ns;
	sphere.GetNurbForm(ns);
	pDoc->AddSurfaceObject(ns, &attr);

	// Do the same for a layer.
	ON_Layer layer;
	layer.m_material_index = matIndex;
	pDoc->m_layer_table.AddLayer(layer);

	pDoc->DeferredRedraw();

	return success;
}
