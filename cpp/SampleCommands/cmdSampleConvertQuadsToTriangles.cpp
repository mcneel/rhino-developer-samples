#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleConvertQuadsToTriangles command
//

class CCommandSampleConvertQuadsToTriangles : public CRhinoCommand
{
public:
  CCommandSampleConvertQuadsToTriangles() { m_delete_input = false; }
  ~CCommandSampleConvertQuadsToTriangles() {}
  UUID CommandUUID()
  {
    // {F8B6CAC-6DC8-43F1-8215-ACCD07D004B2}
    static const GUID SampleConvertQuadsToTrianglesCommand_UUID =
    { 0xF8B6CAC, 0x6DC8, 0x43F1, { 0x82, 0x15, 0xAC, 0xCD, 0x07, 0xD0, 0x04, 0xB2 } };
    return SampleConvertQuadsToTrianglesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleConvertQuadsToTriangles"; }
  const wchar_t* LocalCommandName() { return L"SampleConvertQuadsToTriangles"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

  void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);
  void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);

private:
  bool m_delete_input;
};

// The one and only CCommandSampleConvertQuadsToTriangles object
static class CCommandSampleConvertQuadsToTriangles theSampleConvertQuadsToTrianglesCommand;

CRhinoCommand::result CCommandSampleConvertQuadsToTriangles::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select meshes to convert");
  go.SetGeometryFilter(CRhinoObject::mesh_object);
  if (go.GetObjects(1, 0) != CRhinoGet::object)
    return CRhinoCommand::cancel;

  CRhinoGetOption gs;
  gs.AcceptNothing();
  gs.SetCommandPrompt(L"Delete input?");
  gs.SetDefaultString((m_delete_input) ? L"Yes" : L"No");
  int n_index = gs.AddCommandOption(RHCMDOPTNAME(L"No"));
  /*int y_index =*/ gs.AddCommandOption(RHCMDOPTNAME(L"Yes"));
  switch (gs.GetOption())
  {
  case CRhinoGet::option:
    if (gs.Option()->m_option_index == n_index)
      m_delete_input = false;
    else
      m_delete_input = true;
    break;

  case CRhinoGet::string:
  case CRhinoGet::nothing:
    break;

  default:
    return CRhinoCommand::cancel;
  }

  int num_converted = 0;
  for (int i = 0; i < go.ObjectCount(); i++)
  {
    CRhinoObjRef objref = go.Object(i);
    if (const CRhinoObject* pObject = objref.Object())
    {
      if (const ON_Mesh* pMesh = ON_Mesh::Cast(pObject->Geometry()))
      {
        ON_Mesh mesh(*pMesh);
        if (mesh.QuadCount() <= 0)
          continue;

        if (!mesh.ConvertQuadsToTriangles())
          continue;

        const CRhinoMeshObject* mesh_object = NULL;
        if (m_delete_input)
          mesh_object = context.m_doc.ReplaceObject(objref, mesh);
        else
          mesh_object = context.m_doc.AddMeshObject(mesh);

        if (mesh_object)
          num_converted++;
      }
    }
  }

  if (num_converted > 0)
    context.m_doc.Redraw();

  RhinoApp().Print(L"%d meshes selected, %d meshes converted.\n", go.ObjectCount(), num_converted);

  return CRhinoCommand::success;
}

void CCommandSampleConvertQuadsToTriangles::LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  pc.LoadProfileBool(lpszSection, L"DeleteInput", &m_delete_input);
}

void CCommandSampleConvertQuadsToTriangles::SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  pc.SaveProfileBool(lpszSection, L"DeleteInput", m_delete_input);
}

//
// END SampleConvertQuadsToTriangles command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
