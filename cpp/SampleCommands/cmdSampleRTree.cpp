#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRTree command
//

#pragma region SampleRTree command

class CSampleSearchResults
{
public:
  static void resultCallback(void* a_context, INT_PTR a_idA, INT_PTR a_idB);
  const ON_SimpleArray<ON_2dex>& Fids() const;

private:
  ON_SimpleArray<ON_2dex> m_fids;
};

void CSampleSearchResults::resultCallback(void* a_context, INT_PTR a_idA, INT_PTR a_idB)
{
  CSampleSearchResults* pThis = (CSampleSearchResults*)a_context;
  ON_2dex&d = pThis->m_fids.AppendNew();
  d.i = (int)a_idA;
  d.j = (int)a_idB;
}

const ON_SimpleArray<ON_2dex>& CSampleSearchResults::Fids() const
{
  return m_fids;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleRTree : public CRhinoCommand
{
public:
  CCommandSampleRTree() = default;
  UUID CommandUUID() override
  {
    // {A0941681-9B6D-48A7-B34F-E09751D65D64}
    static const GUID SampleRTreeCommand_UUID =
    { 0xA0941681, 0x9B6D, 0x48A7, { 0xB3, 0x4F, 0xE0, 0x97, 0x51, 0xD6, 0x5D, 0x64 } };
    return SampleRTreeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleRTree"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleRTree object
static class CCommandSampleRTree theSampleRTreeCommand;

CRhinoCommand::result CCommandSampleRTree::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Pick meshes");
  go.SetGeometryFilter(ON::mesh_object);
  go.GetObjects(2, 2);
  if (CRhinoCommand::success != go.CommandResult())
    return go.CommandResult();

  const ON_Mesh* meshA = go.Object(0).Mesh();
  if (0 == meshA)
    return CRhinoCommand::failure;

  const ON_Mesh* meshB = go.Object(1).Mesh();
  if (0 == meshB)
    return CRhinoCommand::failure;

  ON_RTree rtreeA, rtreeB;
  if (!rtreeA.CreateMeshFaceTree(meshA))
    return CRhinoCommand::failure;

  if (!rtreeB.CreateMeshFaceTree(meshB))
    return CRhinoCommand::failure;

  CSampleSearchResults results;
  if (!ON_RTree::Search(rtreeA, rtreeB, 0.5, CSampleSearchResults::resultCallback, &results))
    return CRhinoCommand::failure;
  
  const ON_SimpleArray<ON_2dex>& fids = results.Fids();
  for (int mesh_index = 0; mesh_index < 2; mesh_index++)
  {
    const ON_Mesh* mesh = (0 == mesh_index) ? meshA : meshB;
    ON_Mesh* pMesh = new ON_Mesh(fids.Count(), 4 * fids.Count(), true, false);
    ON_MeshFace f;
    for (int i = 0; i < fids.Count(); i++)
    {
      const int* fvi = mesh->m_F[(0 == mesh_index) ? fids[i].i : fids[i].j].vi;
      f.vi[0] = pMesh->m_V.Count();
      f.vi[1] = f.vi[0] + 1;
      f.vi[2] = f.vi[1] + 1;
      pMesh->m_V.Append(mesh->m_V[fvi[0]]);
      pMesh->m_V.Append(mesh->m_V[fvi[1]]);
      pMesh->m_V.Append(mesh->m_V[fvi[2]]);
      pMesh->m_N.Append(mesh->m_N[fvi[0]]);
      pMesh->m_N.Append(mesh->m_N[fvi[1]]);
      pMesh->m_N.Append(mesh->m_N[fvi[2]]);
      if (fvi[2] != fvi[3])
      {
        f.vi[3] = f.vi[2] + 1;
        pMesh->m_V.Append(mesh->m_V[fvi[3]]);
        pMesh->m_N.Append(mesh->m_N[fvi[3]]);
      }
      else
      {
        f.vi[3] = f.vi[2];
      }
      pMesh->m_F.Append(f);
    }

    pMesh->ComputeFaceNormals();
    CRhinoMeshObject* mesh_object = new CRhinoMeshObject();
    mesh_object->SetMesh(pMesh);
    context.m_doc.AddObject(mesh_object);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRTree command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
