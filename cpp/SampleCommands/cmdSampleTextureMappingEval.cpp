#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleTextureMappingEval command
//

class CCommandSampleTextureMappingEval : public CRhinoCommand
{
public:
	CCommandSampleTextureMappingEval() = default;
	~CCommandSampleTextureMappingEval() = default;
	UUID CommandUUID() override
	{
		// {6A3AA8E1-D554-465D-A246-18E45A4E6B2C}
		static const GUID SampleTextureMappingEvalCommand_UUID =
		{ 0x6a3aa8e1, 0xd554, 0x465d, { 0xa2, 0x46, 0x18, 0xe4, 0x5a, 0x4e, 0x6b, 0x2c } };
		return SampleTextureMappingEvalCommand_UUID;
	}
	const wchar_t* EnglishCommandName() override { return L"SampleTextureMappingEval"; }
	CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleTextureMappingEval object
static class CCommandSampleTextureMappingEval theSampleTextureMappingEvalCommand;

static CRhinoCommand::result GetPointOnMesh(const CRhinoCommandContext& context, CRhinoObjRef& objRef, ON_3dPoint& point, ON_3dVector& normal)
{
	// Ask for object
	CRhinoGetObject go;
	go.SetCommandPrompt(L"Select mesh");
	go.SetGeometryFilter(CRhinoGetObject::mesh_object);
	CRhinoGet::result res = go.GetObjects();
	if (res != CRhinoGet::result::object)
	{
		return CRhinoCommand::cancel;
	}
	if (go.ObjectCount() < 1)
	{
		return CRhinoCommand::cancel;
	}
	objRef = go.Object(0);

	// Create new mesh and compute normals
	const CRhinoMeshObject* pMeshObject = CRhinoMeshObject::Cast(objRef.Object());
	if (nullptr == pMeshObject)
	{
		return CRhinoCommand::cancel;
	}
	const ON_Mesh* pMesh = pMeshObject->Mesh();
	if (nullptr == pMesh)
	{
		return CRhinoCommand::cancel;
	}
	ON_Mesh mesh(*pMesh);
	mesh.ComputeFaceNormals();
	mesh.ComputeVertexNormals();

	// Ask for a point on the mesh
	CRhinoGetPoint gp;
	gp.SetCommandPrompt(L"Select point on the mesh");
	gp.Constrain(mesh);
	res = gp.GetPoint();
	if (res != CRhinoGet::result::point)
	{
		return CRhinoCommand::cancel;
	}
	point = gp.Point();
	normal = ON_3dVector::ZeroVector;

	// Find normal for the picked point if possible
	ON_MESH_POINT mp;
	if (mesh.GetClosestPoint(point, &mp))
	{
		if (mp.m_ci.m_type == ON_COMPONENT_INDEX::mesh_face && mesh.HasFaceNormals())
		{
			normal = mesh.m_FN[mp.m_ci.m_index];
		}
		else if (mp.m_ci.m_type == ON_COMPONENT_INDEX::mesh_vertex && mesh.HasVertexNormals())
		{
			normal = mesh.m_N[mp.m_ci.m_index];
		}
		else
		{
			normal = ON_3dVector::ZeroVector;
		}
	}

	return CRhinoCommand::success;
}

CRhinoCommand::result CCommandSampleTextureMappingEval::RunCommand(const CRhinoCommandContext& context)
{
	CRhinoObjRef objRef;
	ON_3dPoint point;
	ON_3dVector normal;
	CRhinoCommand::result res = GetPointOnMesh(context, objRef, point, normal);
	if (CRhinoCommand::success != res)
	{
		return res;
	}

	if (nullptr == objRef.Object())
	{
		return CRhinoCommand::cancel;
	}
	const CRhinoObject& object = *objRef.Object();

	if (nullptr == object.Document())
	{
		return CRhinoCommand::cancel;
	}
	const CRhinoDoc& doc = *object.Document();

	const UUID uuidCurrentRenderingPlugIn = RhinoApp().GetDefaultRenderApp();

	bool bMappingsFound = false;
	const ON_MappingRef* pMappingRef = object.Attributes().m_rendering_attributes.MappingRef(uuidCurrentRenderingPlugIn);
	if (nullptr != pMappingRef)
	{
		// Iterate all mapping channels
		for (int i = 0; i < pMappingRef->m_mapping_channels.Count(); i++)
		{
			const ON_MappingChannel& mc = pMappingRef->m_mapping_channels[i];
			const int mappingChannel = mc.m_mapping_channel_id;

			// Find texture mapping from the texture mapping table
			const int mappingTableIndex = doc.m_texture_mapping_table.FindTextureMapping(mc.m_mapping_id);
			if (mappingTableIndex >= 0)
			{
				const CRhinoTextureMapping& mapping = doc.m_texture_mapping_table[mappingTableIndex];

				bMappingsFound = true;
				RhinoApp().Print(L"Mapping channel %i\n", mappingChannel);

				// Get xforms for point and normal transforms
				ON_Xform mappingXformP, mappingXformN;
				mc.m_object_xform.GetMappingXforms(mappingXformP, mappingXformN);

				// Get texure coordinate. This coordinate is the one that needs to be passed to texture evaluator
				ON_3dPoint textureCoordinate;
				if (0 <= mapping.Evaluate(point, normal, &textureCoordinate, mappingXformP, mappingXformN))
				{
					RhinoApp().Print(L"  Evaluates to %f, %f, %f \n", textureCoordinate.x, textureCoordinate.y, textureCoordinate.z);
				}
			}
		}
	}

	if (!bMappingsFound)
	{
		RhinoApp().Print(L"No mapping channels\n");
	}


	// WCS mappings on RDK diffuse material
	const CRhRdkMaterial* pMaterial = object.ObjectRdkMaterial(ON_COMPONENT_INDEX());
	if (pMaterial)
	{
		// There's a material - let's see if there's a texture.
		const CRhRdkTexture* pTexture = dynamic_cast<const CRhRdkTexture*>(pMaterial->GetTextureForUsage(CRhRdkMaterial::ChildSlotUsage::Diffuse));
		if (pTexture)
		{
			ON_3dPoint textureCoordinate = ON_3dPoint::UnsetPoint;
			IRhRdkTextureEvaluator* pTextEvaluator = pTexture->NewTextureEvaluator(IRhRdkTextureEvaluator::CEvalFlags::efNormal);
			if (nullptr != pTextEvaluator)
			{
				if (pTexture->ProjectionMode() == CRhRdkTexture::ProjectionModes::Wcs)
				{
					// Wcs mapping maps world coordinate straight into texture coordinate
					textureCoordinate = point;
					RhinoApp().Print(L"RDK texture with WCS projection\n");
					RhinoApp().Print(L"  Evaluates to %f, %f, %f \n", textureCoordinate.x, textureCoordinate.y, textureCoordinate.z);
				}
				if (pTexture->ProjectionMode() == CRhRdkTexture::ProjectionModes::WcsBox)
				{
					// Wcs box mapping has it's own function to evaluate it
					textureCoordinate = CRhRdkTexture::WcsBoxMapping(point, normal);
					RhinoApp().Print(L"RDK texture with WCS box projection\n");
					RhinoApp().Print(L"  Evaluates to %f, %f, %f \n", textureCoordinate.x, textureCoordinate.y, textureCoordinate.z);
				}
				CRhRdkColor color;
				pTextEvaluator->GetColor(textureCoordinate, ON_3dVector::ZeroVector, ON_3dVector::ZeroVector, color);
				RhinoApp().Print(L"  Texture RGB color is %f, %f, %f \n", color.FRed(), color.FGreen(), color.FBlue());
				pTextEvaluator->DeleteThis();
			}
		}
	}

	return CRhinoCommand::success;
}

//
// END SampleTextureMappingEval command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
