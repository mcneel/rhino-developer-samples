#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSurfacePoints command
//

class CCommandSampleSurfacePoints : public CRhinoCommand
{
public:
  CCommandSampleSurfacePoints() {}
  ~CCommandSampleSurfacePoints() {}
  UUID CommandUUID()
  {
    // {1166B5D9-91EB-4393-B816-A62C8818DD13}
    static const GUID SampleSurfacePointsCommand_UUID =
    { 0x1166B5D9, 0x91EB, 0x4393, { 0xB8, 0x16, 0xA6, 0x2C, 0x88, 0x18, 0xDD, 0x13 } };
    return SampleSurfacePointsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSurfacePoints"; }
  const wchar_t* LocalCommandName() { return L"SampleSurfacePoints"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSurfacePoints object
static class CCommandSampleSurfacePoints theSampleSurfacePointsCommand;

CRhinoCommand::result CCommandSampleSurfacePoints::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect();
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_BrepFace* face = go.Object(0).Face();
  if (0 == face)
    return CRhinoCommand::failure;

  const ON_Brep* brep = face->Brep();
  if (0 == brep)
    return CRhinoCommand::failure;

  ON_SimpleArray<int> vertices;
  int i = 0;

  const ON_BrepLoop* loop = face->OuterLoop();
  if (loop)
  {
    for (i = 0; i < loop->TrimCount(); i++)
    {
      const ON_BrepTrim* trim = loop->Trim(i);
      if (trim)
      {
        const ON_BrepVertex* vertex = trim->Vertex(0);
        if (vertex)
          vertices.Append(vertex->m_vertex_index);
        vertex = trim->Vertex(1);
        if (vertex)
          vertices.Append(vertex->m_vertex_index);
      }
    }
  }

  if (0 == vertices.Count())
    return CRhinoCommand::failure;

  // sort
  vertices.QuickSort(&ON_CompareIncreasing<int>);

  // cull
  int vi = -1;
  for (i = vertices.Count() - 1; i >= 0; i--)
  {
    if (vertices[i] == vi)
      vertices.Remove(i);
    else
      vi = vertices[i];
  }

  // Add
  for (i = 0; i < vertices.Count(); i++)
    context.m_doc.AddPointObject(brep->m_V[vertices[i]].Point());

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleSurfacePoints command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
