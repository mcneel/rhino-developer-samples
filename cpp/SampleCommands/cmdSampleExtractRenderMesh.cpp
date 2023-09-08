#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtractRenderMesh command
//

#pragma region SampleExtractRenderMesh command

class CCommandSampleExtractRenderMesh : public CRhinoCommand
{
public:
  CCommandSampleExtractRenderMesh() = default;
  UUID CommandUUID() override
  {
    // {12166A45-335D-4C6D-BF1D-2BEBD23518E2}
    static const GUID SampleExtractRenderMeshCommand_UUID =
    { 0x12166A45, 0x335D, 0x4C6D, { 0xBF, 0x1D, 0x2B, 0xEB, 0xD2, 0x35, 0x18, 0xE2 } };
    return SampleExtractRenderMeshCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleExtractRenderMesh"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleExtractRenderMesh object
static class CCommandSampleExtractRenderMesh theSampleExtractRenderMeshCommand;

static bool ExtractObjectMeshHelper_Recursive(ON::mesh_type mt, const ON_Viewport& vp, CRhinoDoc& doc, const CRhinoObject& object, const ON_Xform& xform)
{
  const auto* pBlockRef = CRhinoInstanceObject::Cast(&object);
  if (pBlockRef && !object.HasCustomRenderMeshes(mt, &vp))
  {
    //Recurse - this is a block definition.
    if (const auto* pDef = pBlockRef->InstanceDefinition())
    {
      const auto count = pDef->ObjectCount();

      for (int i = 0; i < count; i++)
      {
        const auto* pObject = pDef->Object(i);
        if (pObject)
        {
          ExtractObjectMeshHelper_Recursive(mt, vp, doc, *pObject, xform * pBlockRef->InstanceXform());
        }
      }
    }
    return true;
  }

  //Otherwise, actually get the meshes for this concrete object and make the mesh object.
  //First do the meshes - because if it makes something.
  auto object_primitives = object.RenderMeshes(mt, false, &vp);

  if (object_primitives)
  {
    ON_3dmObjectAttributes object_attribs;
    std::vector<std::shared_ptr<const ON_Mesh>> object_meshes;

    for (const auto& instance : *object_primitives)
    {
      if (instance)
      {
        const auto* pObject = doc.LookupObject(instance->Object());

        if (pObject)
        {
          if (pObject->IsReference() && pObject->ObjectLayer().IsReference())
          {
            doc.GetDefaultObjectAttributes(object_attribs);
          }
          else
          {
            object_attribs = pObject->Attributes();
          }

          object_attribs.RemoveFromAllGroups();
          object_attribs.m_uuid = ON_nil_uuid;
          object_attribs.PurgeUserData();

          const ON_Xform xform_this = instance->Xform() * xform;

          //Transform the mesh if it's not identity
          if (xform_this.IsIdentity())
          {
            object_meshes.push_back(instance->Geometry().Mesh());
          }
          else
          {
            auto pMesh = new ON_Mesh(*instance->Geometry().Mesh());
            pMesh->Transform(xform_this);
            object_meshes.push_back(std::shared_ptr<const ON_Mesh>(pMesh));
            object_attribs.Transform(pMesh, xform);
          }
        }
      }
    }

    ON_Mesh* new_mesh = new ON_Mesh;
    new_mesh->Append(object_meshes);

    if (!new_mesh->HasVertexNormals())
    {
      new_mesh->ComputeVertexNormals();
    }

    if (new_mesh->IsValid())
    {
      CRhinoMeshObject* mesh_object = new CRhinoMeshObject(object_attribs);
      mesh_object->SetMesh(new_mesh);
      doc.AddObject(mesh_object);
    }
    else
    {
      delete new_mesh;
    }
  }

  return true;
}

CRhinoCommand::result CCommandSampleExtractRenderMesh::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoGetObject go;
  go.EnableGroupSelect(TRUE);
  go.EnableSubObjectSelect(FALSE);
  go.SetCommandPrompt(L"Select objects to extract render mesh");
  go.GetObjects(1, 0);

  if (go.Result() != CRhinoGet::object)
    return CRhinoCommand::cancel;

  //For view dependent custom render primitives.
  const ON_Viewport vp = doc->ActiveView() ? doc->ActiveView()->ActiveViewport().VP() : ON_Viewport();

  auto rc = CRhinoCommand::success;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const auto* pTopLevelObject = go.Object(i).Object();
    if (pTopLevelObject)
    {
      if (!ExtractObjectMeshHelper_Recursive(ON::render_mesh, vp, *doc, *pTopLevelObject, ON_Xform::IdentityTransformation))
        rc = CRhinoCommand::failure;
    }
  }

  doc->Redraw();

  return rc;
}

#pragma endregion

//
// END SampleExtractRenderMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
