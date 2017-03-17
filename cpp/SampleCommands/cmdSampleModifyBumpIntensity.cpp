#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleModifyBumpIntensity command
//

class CCommandSampleModifyBumpIntensity : public CRhinoCommand
{
public:
  CCommandSampleModifyBumpIntensity() {}
  ~CCommandSampleModifyBumpIntensity() {}
  UUID CommandUUID()
  {
    // {AEB0DBEB-9904-4367-AB1A-53919D7EA37A}
    static const GUID SampleModifyBumpIntensityCommand_UUID =
    { 0xAEB0DBEB, 0x9904, 0x4367, { 0xAB, 0x1A, 0x53, 0x91, 0x9D, 0x7E, 0xA3, 0x7A } };
    return SampleModifyBumpIntensityCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleModifyBumpIntensity"; }
  const wchar_t* LocalCommandName() { return L"SampleModifyBumpIntensity"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleModifyBumpIntensity object
static class CCommandSampleModifyBumpIntensity theSampleModifyBumpIntensityCommand;

CRhinoCommand::result CCommandSampleModifyBumpIntensity::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object to modify bump intensity");
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& ref = go.Object(0);
  const CRhinoObject* obj = ref.Object();
  if (0 == obj)
    return CRhinoCommand::failure;

  ON_Material material = obj->ObjectMaterial();
  if (material.Index() < 0)
  {
    // I'm assuming the object already has a material. That is,
    // it is not just using the default material.
    RhinoApp().Print(L"Object does not have a material.\n");
    return CRhinoCommand::nothing;
  }

  int texture_index = material.FindTexture(0, ON_Texture::TYPE::bump_texture);
  if (texture_index < 0)
  {
    // I'm assuming the object's material already has a bump.
    RhinoApp().Print(L"Object does not have a bump texture.\n");
    return CRhinoCommand::nothing;
  }

  ON_Texture& texture = material.m_textures[texture_index];
  int blend_constant = ON_Round(texture.m_blend_constant_A * 100.0);

  CRhinoGetInteger gi;
  gi.SetCommandPrompt(L"New bump intensity");
  gi.SetDefaultInteger(blend_constant);
  gi.SetLowerLimit(0, false);
  gi.SetUpperLimit(100, false);
  gi.GetInteger();
  if (gi.CommandResult() != CRhinoCommand::success)
    return gi.CommandResult();

  texture.m_blend_constant_A = (double)gi.Number() / 100.0;
  if (texture.m_blend_constant_A < 1.0)
    texture.m_mode = ON_Texture::MODE::blend_texture;
  else
    texture.m_mode = ON_Texture::MODE::decal_texture;

  context.m_doc.m_material_table.ModifyMaterial(material, material.Index());
  context.m_doc.Redraw();

  return success;
}

//
// END SampleModifyBumpIntensity command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
