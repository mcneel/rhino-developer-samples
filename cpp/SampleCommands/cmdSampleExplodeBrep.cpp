#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExplodeBrep command
//

#pragma region SampleExplodeBrep command

class CCommandSampleExplodeBrep : public CRhinoCommand
{
public:
  CCommandSampleExplodeBrep() {}
  ~CCommandSampleExplodeBrep() {}
  UUID CommandUUID()
  {
    // {9475C820-1FD7-4B7A-94C5-B39E4A6B646D}
    static const GUID SampleExplodeBrepCommand_UUID =
    { 0x9475C820, 0x1FD7, 0x4B7A, { 0x94, 0xC5, 0xB3, 0x9E, 0x4A, 0x6B, 0x64, 0x6D } };
    return SampleExplodeBrepCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExplodeBrep"; }
  const wchar_t* LocalCommandName() const { return L"SampleExplodeBrep"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExplodeBrep object
static class CCommandSampleExplodeBrep theSampleExplodeBrepCommand;

CRhinoCommand::result CCommandSampleExplodeBrep::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select polysurfaces to explode");
  go.SetGeometryFilter(CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 0);
  CRhinoCommand::result rc = go.CommandResult();
  if (rc != CRhinoCommand::success)
    return go.CommandResult();

  CRhinoKeepKinkySurfaces kinky_helper;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& object_ref = go.Object(i);
    const CRhinoObject* object = object_ref.Object();
    const ON_Brep* brep = object_ref.Brep();
    if (0 == object || 0 == brep || 1 == brep->m_F.Count())
      continue;

    ON_SimpleArray<CRhinoObject*> sub_objects(brep->m_F.Count());
    if (object->GetSubObjects(sub_objects, 0))
    {
      for (int j = 0; j < sub_objects.Count(); j++)
        context.m_doc.AddObject(sub_objects[j]);

      context.m_doc.DeleteObject(object_ref);
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleExplodeBrep command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
