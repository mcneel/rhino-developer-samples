#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleEnableGrips command
//

#pragma region SampleEnableGrips command

class CCommandSampleEnableGrips : public CRhinoCommand
{
public:
  CCommandSampleEnableGrips();
  UUID CommandUUID() override
  {
    // {B8A7E946-AE2C-4AFE-A24A-3E32186B206E}
    static const GUID SampleEnableGripsCommand_UUID =
    { 0xB8A7E946, 0xAE2C, 0x4AFE, { 0xA2, 0x4A, 0x3E, 0x32, 0x18, 0x6B, 0x20, 0x6E } };
    return SampleEnableGripsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleEnableGrips"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleEnableGrips object
static class CCommandSampleEnableGrips theSampleEnableGripsCommand;

CCommandSampleEnableGrips::CCommandSampleEnableGrips()
{
  EnableUndo(false);
}

CRhinoCommand::result CCommandSampleEnableGrips::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects for grip display");
  go.EnableSubObjectSelect(false);
  go.EnableGroupSelect();
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const int object_count = go.ObjectCount();
  for (int i = 0; i < object_count; i++)
  {
    CRhinoObject* pObject = const_cast<CRhinoObject*>(go.Object(i).Object());
    if (pObject)
    {
      pObject->EnableGrips(true);
      if (nullptr == pObject->m_grips)
      {
        const CRhinoBrepObject* pBrepObject = CRhinoBrepObject::Cast(pObject);
        if (pBrepObject)
        {
          const ON_Brep* pBrep = pBrepObject->Brep();
          if (pBrep && pBrep->m_F.Count() > 1)
            RhinoApp().Print(L"Cannot turn on grips for polysurfaces.\n");
        }
        else
        {
          const CRhinoMeshObject* pMeshObject = CRhinoMeshObject::Cast(pObject);
          if (pMeshObject)
          {
            const ON_Mesh* pMesh = pMeshObject->Mesh();
            if (pMesh && pMesh->m_V.Count() > 0)
              RhinoApp().Print(L"Cannot turn on grips for meshes that have more than one million vertices.\n");
          }
          else
          {
            const CRhinoPointCloudObject* pPointCloudObject = CRhinoPointCloudObject::Cast(pObject);
            if (pPointCloudObject)
            {
              if (pPointCloudObject->PointCloud().PointCount() > 0)
                RhinoApp().Print(L"Cannot turn on grips for point clouds that have more than one million vertices.\n");
            }
          }
        }
      }
      else
      {
        pObject->Select(false);
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleEnableGrips command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
