#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSubDDeleteFaces command
//

#pragma region SampleSubDDeleteFaces command

class CCommandSampleSubDDeleteFaces : public CRhinoCommand
{
public:
  CCommandSampleSubDDeleteFaces() = default;
  UUID CommandUUID() override
  {
    // {AFDBE815-ED7C-4586-BD8C-B89808FA4025}
    static const GUID SampleSubDDeleteFacesCommand_UUID =
    { 0xAFDBE815, 0xED7C, 0x4586, { 0xBD, 0x8C, 0xB8, 0x98, 0x08, 0xFA, 0x40, 0x25 } };
    return SampleSubDDeleteFacesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSubDDeleteFaces"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSubDDeleteFaces object
static class CCommandSampleSubDDeleteFaces theSampleSubDDeleteFacesCommand;

CRhinoCommand::result CCommandSampleSubDDeleteFaces::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetGeometryFilter(CRhinoObject::meshface_filter);
  go.SetCommandPrompt(L"Select faces to delete");
  go.GetObjects(1, 0);

  CRhinoCommand::result rc = go.CommandResult();
  if (rc != CRhinoCommand::success)
    return rc;

  const int count = go.ObjectCount();
  for (int i = 0; i < count; i++)
  {
    CRhinoObject* object = const_cast<CRhinoObject*>(go.Object(i).Object());
    if (object)
    {
      if (!object->IsVisible() || object->IsLocked())
        continue;

      ON_SimpleArray<ON_COMPONENT_INDEX> components;
      object->GetSelectedSubObjects(components);
      if (components.UnsignedCount() > 0)
      {
        CRhinoSubDObject* rhino_subd_object = CRhinoSubDObject::Cast(object);
        if (0 == rhino_subd_object)
          break;

        const ON_SubDRef subd_ref = rhino_subd_object->SubDRef();

        ON_SubDRef new_subd_ref(subd_ref);
        ON_SubD& new_subd = new_subd_ref.UniqueSubD();
        if (new_subd.IsEmpty())
          break;

        // Make SubD subobject deletion not clear locked or hidden status
        ON_ComponentStatus s;
        s.SetStates(ON_ComponentStatus::SelectedPersistent);
        s.SetStates(ON_ComponentStatus::Highlighted);
        s.SetStates(ON_ComponentStatus::Damaged);
        new_subd.ClearComponentStates(s);

        new_subd.DissolveOrDelete(components);

        if (0 == new_subd.FaceCount())
        {
          context.m_doc.DeleteObject(object);
          object = nullptr;
        }
        else if (
             new_subd.FaceCount() < subd_ref.SubD().FaceCount()
          || new_subd.EdgeCount() < subd_ref.SubD().EdgeCount()
          || new_subd.VertexCount() < subd_ref.SubD().VertexCount()
          )
        {
          CRhinoSubDObject* new_rhino_subd_object = new CRhinoSubDObject();
          new_rhino_subd_object->SetSubD(new_subd_ref);
          if (false == context.m_doc.ReplaceObject(object, new_rhino_subd_object))
            delete new_rhino_subd_object;
        }
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSubDDeleteFaces command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
