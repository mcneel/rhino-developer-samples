#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleReduceMesh2 command
//

#pragma region SampleReduceMesh2 command

class CCommandSampleReduceMesh2 : public CRhinoCommand
{
public:
  CCommandSampleReduceMesh2();
  ~CCommandSampleReduceMesh2() {}
  UUID CommandUUID()
  {
    // {B3706E7E-28BE-4114-8382-062AF42F69DF}
    static const GUID SampleReduceMesh2Command_UUID =
    { 0xB3706E7E, 0x28BE, 0x4114, { 0x83, 0x82, 0x06, 0x2A, 0xF4, 0x2F, 0x69, 0xDF } };
    return SampleReduceMesh2Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleReduceMesh2"; }
  const wchar_t* LocalCommandName() const { return L"SampleReduceMesh2"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context);

private:
  bool GetCommandOptions(int total_face_count, double& reduce_percentage, bool& planar_only, int& accuracy);

private:
  double m_reduce_percentage;
  int m_accuracy;
  bool m_planar_only;
};

// The one and only CCommandSampleReduceMesh2 object
static class CCommandSampleReduceMesh2 theSampleReduceMesh2Command;

CCommandSampleReduceMesh2::CCommandSampleReduceMesh2()
  : m_reduce_percentage(0.0)
  , m_accuracy(7)
  , m_planar_only(false)
{
}

CRhinoCommand::result CCommandSampleReduceMesh2::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select meshes");
  go.SetGeometryFilter(CRhinoGetObject::mesh_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  int total_face_count = 0;
  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Mesh* mesh = go.Object(i).Mesh();
    if (0 == mesh)
      return CRhinoCommand::failure;

    total_face_count += mesh->TriangleCount() + 2 * mesh->QuadCount();
  }

  bool rc = GetCommandOptions(total_face_count, m_reduce_percentage, m_planar_only, m_accuracy);
  if (!rc)
    return CRhinoCommand::cancel;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const ON_Mesh* mesh = go.Object(i).Mesh();
    if (0 == mesh)
      return CRhinoCommand::failure;

    ON_Mesh new_mesh(*mesh);
    new_mesh.CullDegenerateFaces();
    new_mesh.ConvertQuadsToTriangles();

    const double sharp_new_face_count = (double)new_mesh.FaceCount() * (100.0 - m_reduce_percentage) / 100.0;
    const int new_face_count = (int)max(1, floor(sharp_new_face_count + 0.5));

    rc = RhinoReduceMesh(new_mesh, new_face_count, !m_planar_only, m_accuracy, false);

    if (rc && new_mesh.IsValid())
    {
      new_mesh.ConvertTrianglesToQuads(5.0*ON_PI / 180.0, 2.0);
      context.m_doc.ReplaceObject(go.Object(i), new_mesh);
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

bool CCommandSampleReduceMesh2::GetCommandOptions(int total_face_count, double& reduce_percentage, bool& planar_only, int& accuracy)
{
  double face_count = total_face_count;

  while (true)
  {
    CRhinoGetOption go;
    go.SetCommandPrompt(L"Choose mesh reduction option");
    const int opt_count = go.AddCommandOptionNumber(RHCMDOPTNAME(L"FaceCount"), &face_count, 0, TRUE, 0.0, total_face_count);
    const int opt_percent = go.AddCommandOptionNumber(RHCMDOPTNAME(L"ReductionPercentage"), &reduce_percentage, 0, FALSE, 0.0, 100.0);
    const int opt_planar = go.AddCommandOption(RHCMDOPTNAME(L"PlanarOnly"));
    const int opt_accuracy = go.AddCommandOptionInteger(RHCMDOPTNAME(L"Accuracy"), &accuracy, L"Accuracy (1=fast, 10=accurate)", 1.0, 10.0);
    go.AcceptNothing(TRUE);

    CRhinoGet::result res = go.GetOption();

    if (res == CRhinoGet::cancel)
      return false;

    if (res == CRhinoGet::option)
    {
      const CRhinoCommandOption* option = go.Option();
      if (0 == option)
        return false;

      const int option_index = option->m_option_index;
      if (opt_planar == option_index)
        planar_only = true;
      else if (opt_count == option_index)
        reduce_percentage = 100.0 * (1.0 - face_count / (double)total_face_count);
      else if (opt_percent == option_index)
        face_count = (int)(0.5 + (1.0 - reduce_percentage / 100.0) * (double)total_face_count);

      continue;
    }

    break;
  }

  return true;
}

#pragma endregion

//
// END SampleReduceMesh2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
