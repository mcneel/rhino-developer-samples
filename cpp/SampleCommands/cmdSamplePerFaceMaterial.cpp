
#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePerFaceMaterial command
//

#pragma region SamplePerFaceMaterial command

class CCommandSamplePerFaceMaterial : public CRhinoCommand
{
public:
  CCommandSamplePerFaceMaterial() = default;

  virtual UUID CommandUUID() override { static const UUID uuid = { 0x5B02BE51, 0x25C9, 0x4C83, { 0x95, 0xC6, 0xE5, 0x56, 0x2E, 0x59, 0x9C, 0x31 } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SamplePerFaceMaterial"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSamplePerFaceMaterial object
static class CCommandSamplePerFaceMaterial theSamplePerFaceMaterialCommand;

CRhinoCommand::result CCommandSamplePerFaceMaterial::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  const CRhRdkMaterial* pMaterial = nullptr;

  int count = 0;
  while (true)
  {
    CRhinoGetObject go;
    go.SetCommandPrompt(L"Select object with the rendering material you want to assign");
    if (count > 0)
    {
      go.EnablePreSelect(false);
      go.EnablePostSelect(true);
    }

    go.GetObjects(1, 1);
    if (CRhinoCommand::success != go.CommandResult())
      return go.CommandResult();

    const auto& objRef = go.Object(0);

    const auto* pObject = objRef.Object();
    if (nullptr == pObject)
      return CRhinoCommand::failure;

    pMaterial = pObject->ObjectRdkMaterial(ON_COMPONENT_INDEX());
    if (nullptr != pMaterial)
      break;

    RhinoApp().Print(L"The selected object does not have a rendering material.\n");
    count++;
  }

  RhinoApp().Print(L"The rendering material's name is '%s'\n", static_cast<const wchar_t*>(pMaterial->DisplayName()));

  while (true)
  {
    RhinoApp().Print(L"You can hold down Shift and Control to select a sub-face\n");

    CRhinoGetObject go;
    go.SetCommandPrompt(L"Select the object or sub-face that you would like to use the rendering material");
    go.AcceptNothing();
    go.EnableSubObjectSelect(true);
    go.EnablePreSelect(false);
    go.EnablePostSelect(true);

    // Specify a geometry filter that excludes extrusion objects. Otherwise sub-face picking on extrusions won't work.
    // meshface_filter is needed for mesh and SubD face picking. We ran out of spare component indices when SubD's were
    // added which is why meshes and SubDs share the same component indices.
    go.SetGeometryFilter(ON::surface_object | ON::brep_object | ON::subd_object | ON::meshface_filter);

    go.GetObjects(1, 1);
    if (CRhinoCommand::success != go.CommandResult())
      return go.CommandResult();

    if (CRhinoGet::nothing == go.Result())
      break;

    const auto& objRef = go.Object(0);
    pMaterial->AssignTo(objRef);

    doc->Redraw();
  }

  return success;
}

#pragma endregion

//
// END SamplePerFaceMaterial command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
