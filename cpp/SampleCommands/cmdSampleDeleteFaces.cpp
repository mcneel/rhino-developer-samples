#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDeleteFaces command
//

#pragma region SampleDeleteFaces command

class CCommandSampleDeleteFaces : public CRhinoCommand
{
public:
  CCommandSampleDeleteFaces() = default;
  UUID CommandUUID() override
  {
    // {D223571C-BCBA-4664-B786-D9EAD4B97AF9}
    static const GUID SampleDeleteFacesCommand_UUID =
    { 0xD223571C, 0xBCBA, 0x4664, { 0xB7, 0x86, 0xD9, 0xEA, 0xD4, 0xB9, 0x7A, 0xF9 } };
    return SampleDeleteFacesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDeleteFaces"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  int SeparateBrep(ON_Brep& brep, ON_SimpleArray<ON_Brep*>& out_breps);
};

// The one and only CCommandSampleDeleteFaces object
static class CCommandSampleDeleteFaces theSampleDeleteFacesCommand;

CRhinoCommand::result CCommandSampleDeleteFaces::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select faces to delete");
  go.SetGeometryFilter(CRhinoObject::surface_object);
  go.AcceptNothing();
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  ON_SimpleArray<unsigned int> done_sn;

  const int object_count = go.ObjectCount();
  for (int i = 0; i < object_count; i++)
  {
    const CRhinoObject* object = go.Object(i).Object();
    if (nullptr == object)
      return CRhinoCommand::failure;

    // See if we've already processed the top-level object
    if (done_sn.Search(object->RuntimeSerialNumber()) > -1)
      continue;

    if (!object->IsVisible() || object->IsLocked())
      continue;

    // Get all the selected subobjects at once
    ON_SimpleArray<ON_COMPONENT_INDEX> components;
    const int component_count = object->GetSelectedSubObjects(components);
    if (0 == component_count)
      continue;

    ON_SimpleArray<int> indices;
    for (int j = 0; j < component_count; j++)
    {
      const ON_COMPONENT_INDEX& ci = components[j];
      if (ci.m_type == ON_COMPONENT_INDEX::brep_face)
        indices.Append(ci.m_index);
    }

    if (0 == indices.Count())
      continue;

    const CRhinoBrepObject* brep_object = CRhinoBrepObject::Cast(object);
    if (nullptr == brep_object)
      return CRhinoCommand::failure;

    const ON_Brep* brep = brep_object->Brep();
    if (nullptr == brep)
      return CRhinoCommand::failure;

    if (indices.Count() == brep->m_F.Count())
    {
      // All faces selected, delete the whole thing...
      context.m_doc.DeleteObject(object);
      done_sn.Append(object->RuntimeSerialNumber());
      continue;
    }

    // Invert face indices array
    ON_SimpleArray<bool> mark(brep->m_F.Count());
    for (int j = 0; j < brep->m_F.Count(); j++)
      mark.Append(true);
    for (int j = 0; j < indices.Count(); j++)
      mark[indices[j]] = false;
    indices.SetCount(0);
    for (int j = 0; j < mark.Count(); j++)
    {
      if (mark[j])
        indices.Append(j);
    }

    ON_Brep* brep_copy = brep->DuplicateFaces(indices.Count(), indices.Array(), true);
    if (brep_copy)
    {
      // If the brep is disjoint after deleting faces, 
      // separate the result into multiple breps.
      ON_SimpleArray<ON_Brep*> brep_pieces;
      if (!SeparateBrep(*brep_copy, brep_pieces))
      {
        CRhinoBrepObject* new_obj = new CRhinoBrepObject(object->Attributes());
        if (new_obj)
        {
          new_obj->SetBrep(brep_copy);
          brep_copy = nullptr;
          if (context.m_doc.ReplaceObject(object, new_obj))
            done_sn.Append(object->RuntimeSerialNumber());
          else
            delete new_obj;
        }
      }
      else
      {
        CRhinoObjectAttributes attr(object->Attributes());
        for (int j = 0; j < brep_pieces.Count(); j++)
        {
          if (brep_pieces[j])
          {
            CRhinoBrepObject* new_obj = new CRhinoBrepObject(attr);
            if (new_obj)
            {
              new_obj->SetBrep(brep_pieces[j]);
              brep_pieces[j] = nullptr;
              if (0 == j)
              {
                if (context.m_doc.ReplaceObject(object, new_obj))
                  done_sn.Append(object->RuntimeSerialNumber());
                else
                  delete new_obj;
              }
              else
              {
                if (!context.m_doc.AddObject(new_obj))
                  delete new_obj;
              }
            }
            if (brep_pieces[j])
              delete brep_pieces[j]; // Don't leak...
          }
        }
      }

      if (brep_copy)
        delete brep_copy; // Don't leak...
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

int CCommandSampleDeleteFaces::SeparateBrep(ON_Brep& brep, ON_SimpleArray<ON_Brep*>& out_breps)
{
  int piece_count = 0;
  const int connected_count = brep.LabelConnectedComponents();
  if (connected_count > 1)
  {
    ON_SimpleArray<int> fids(brep.m_F.Count());
    for (int i = 1; i <= connected_count; i++)
    {
      fids.Empty();
      for (int j = 0; j < brep.m_F.Count(); j++)
      {
        if (brep.m_F[j].m_face_user.i == i)
          fids.Append(j);
      }
      if (fids.Count() == 0)
        continue;
      ON_Brep*& pBrep = out_breps.AppendNew();
      pBrep = brep.DuplicateFaces(fids.Count(), fids.Array(), false);
      if (!pBrep)
        out_breps.Remove();
      else
        piece_count++;
    }
  }
  return piece_count;
}

#pragma endregion

//
// END SampleDeleteFaces command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
