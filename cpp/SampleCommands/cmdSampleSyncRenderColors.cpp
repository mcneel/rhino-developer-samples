#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSyncRenderColors command
//

class CCommandSampleSyncRenderColors : public CRhinoCommand
{
public:
  CCommandSampleSyncRenderColors() = default;
  ~CCommandSampleSyncRenderColors() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x6CB51310, 0xC634, 0x4DE0, { 0x85, 0xE0, 0x02, 0xC4, 0x4A, 0x8E, 0x54, 0xB2 } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleSyncRenderColors"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;

private:
  bool SynchronizeDiffuseColorWithDisplayColor(CRhinoDoc& doc, const CRhinoObject* obj);
};

// The one and only CCommandSampleSyncRenderColors object
static class CCommandSampleSyncRenderColors theSampleSyncRenderColorsCommand;

CRhinoCommand::result CCommandSampleSyncRenderColors::RunCommand(const CRhinoCommandContext& context)
{
  auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

  int num_modified = 0;

  if (context.IsInteractive())
  {
    CRhinoObjectIterator it(CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_objects);
    it.IncludeLights(false);

    for (auto* obj = it.First(); nullptr != obj; obj = it.Next())
    {
      if (SynchronizeDiffuseColorWithDisplayColor(context.m_doc, obj))
        num_modified++;
    }
  }
  else
  {
    CRhinoGetObject go;
    go.SetCommandPrompt(L"Select objects");
    go.EnablePreSelect(TRUE);
    go.EnableGroupSelect(TRUE);
    go.EnableSubObjectSelect(FALSE);
    go.GetObjects(1, 0);
    if (go.CommandResult() != success)
      return go.CommandResult();

    const int count = go.ObjectCount();
    for (int i = 0; i < count; i++)
    {
      const auto* obj = go.Object(i).Object();
      if (SynchronizeDiffuseColorWithDisplayColor(*pDoc, obj))
        num_modified++;
    }
  }

  if (num_modified > 0)
    pDoc->Regen();

  return success;
}

static CRhRdkBasicMaterial* CreateBasicMaterial(CRhinoDoc& doc, const ON_Color& color)
{
  // Create a Basic Material.
  ON_Material mat;
  mat.SetDiffuse(color);
  auto* pBM = ::RhRdkNewBasicMaterial(mat, &doc);
  if (nullptr != pBM)
  {
    // Make the attach operation undoable.
    CRhRdkContentUndo cu(doc);
    cu.AddContent(*pBM, nullptr);

    // Attach the Basic Material to the document.
    auto& contents = doc.Contents().BeginChange(RhRdkChangeContext::Program);
    contents.Attach(*pBM);
    contents.EndChange();
  }

  return pBM;
}

static void SetMaterialColor(const CRhRdkMaterial& material, const ON_Color& color)
{
  auto& m = material.BeginChange(RhRdkChangeContext::UI);
  m.SetParameter(FS_MAT_DIFFUSE, color);
  m.EndChange();
}

bool CCommandSampleSyncRenderColors::SynchronizeDiffuseColorWithDisplayColor(CRhinoDoc& doc, const CRhinoObject* obj)
{
  if (nullptr == obj)
    return false;

  const auto color = obj->Attributes().DrawColor();

  const auto color_source = obj->Attributes().ColorSource();
  if (color_source == ON::color_from_layer)
  {
    const auto& layer = obj->ObjectLayer();
    const auto* pMaterial = layer.LayerRdkMaterial();
    if ((nullptr == pMaterial) || pMaterial->IsDefaultInstance())
    {
      // Create a new Basic Material and assign it to the layer.
      pMaterial = CreateBasicMaterial(doc, color);
      if (nullptr != pMaterial)
      {
        CRhRdkObjectDataAccess da(&layer);
        da.SetMaterialInstanceId(pMaterial->InstanceId());
      }
    }
    else
    {
      // Material already exists; just set the diffuse color.
      SetMaterialColor(*pMaterial, color);
    }
  }
  else
  if (color_source == ON::color_from_object)
  {
    const auto* pMaterial = obj->ObjectRdkMaterial(ON_COMPONENT_INDEX());
    if ((nullptr == pMaterial) || pMaterial->IsDefaultInstance())
    {
      // Create a new Basic Material and assign it to the object.
      pMaterial = CreateBasicMaterial(doc, color);
      if (nullptr != pMaterial)
      {
        CRhRdkObjectDataAccess da(obj);
        da.SetMaterialInstanceId(pMaterial->InstanceId());
      }
    }
    else
    {
      // Material already exists; just set the diffuse color.
      SetMaterialColor(*pMaterial, color);
    }
  }

  return true;
}

//
// END SampleSyncRenderColors command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
