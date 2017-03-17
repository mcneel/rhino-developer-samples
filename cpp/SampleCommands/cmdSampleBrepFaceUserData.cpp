#include "stdafx.h"
#include "SampleObjectUserData.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleBrepFaceUserData command
//

#pragma region SampleBrepFaceUserData command

class CCommandSampleBrepFaceUserData : public CRhinoCommand
{
public:
  CCommandSampleBrepFaceUserData() {}
  ~CCommandSampleBrepFaceUserData() {}
  UUID CommandUUID()
  {
    // {A16EC009-E898-4775-A379-5024D51E5B3E}
    static const GUID SampleBrepFaceUserDataCommand_UUID =
    { 0xA16EC009, 0xE898, 0x4775, { 0xA3, 0x79, 0x50, 0x24, 0xD5, 0x1E, 0x5B, 0x3E } };
    return SampleBrepFaceUserDataCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleBrepFaceUserData"; }
  const wchar_t* LocalCommandName() const { return L"SampleBrepFaceUserData"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleBrepFaceUserData object
static class CCommandSampleBrepFaceUserData theSampleBrepFaceUserDataCommand;

CRhinoCommand::result CCommandSampleBrepFaceUserData::RunCommand(const CRhinoCommandContext& context)
{
  // Allow for picking of either a surface or a brep face.
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface to attach data");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(TRUE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Get first object reference. Note, this reference
  // reprsents the picked surface or face, not the
  // top level brep.
  const CRhinoObjRef& objref = go.Object(0);

  // Get face on brep that was picked
  const ON_BrepFace* face = objref.Face();
  if (0 == face)
    return CRhinoCommand::failure;

  // Get the faces underlying surface
  const ON_Surface* srf = face->SurfaceOf();
  if (0 == srf)
    return CRhinoCommand::failure;

  // Query surface for user data
  CSampleObjectUserData* ud = CSampleObjectUserData::Cast(srf->GetUserData(CSampleObjectUserData::Id()));
  if (0 != ud)
  {
    RhinoApp().Print(L"Data already attached.\n");
    return CRhinoCommand::nothing;
  }

  // Get the top level object
  const CRhinoBrepObject* obj = CRhinoBrepObject::Cast(objref.Object());
  if (0 == obj)
    return CRhinoCommand::failure;

  // Duplicate the top level object.
  CRhinoBrepObject* dupe_obj = CRhinoBrepObject::Cast(obj->DuplicateRhinoObject());
  if (!dupe_obj)
    return CRhinoCommand::failure;

  // Get the brep
  ON_Brep* dupe_brep = const_cast<ON_Brep*>(dupe_obj->Brep());
  if (0 == dupe_brep)
    return CRhinoCommand::failure;

  // Get the surface
  ON_Surface* dupe_srf = dupe_brep->m_S[face->SurfaceIndexOf()];
  if (0 == dupe_srf)
    return CRhinoCommand::failure;

  // New up the user data
  ud = new CSampleObjectUserData();
  if (0 == ud)
  {
    delete dupe_obj;
    return CRhinoCommand::failure;
  }

  // TODO: Initialize user data object here
  ud->m_point = ON_3dPoint(1.0, 2.0, 3.0);
  ud->m_string = L"Sample Brep Face User Data";

  // Attach user data to surface, not face. 
  if (!dupe_srf->AttachUserData(ud))
  {
    delete ud;
    delete dupe_obj;
    return CRhinoCommand::failure;
  }

  // Note, the memory allocated for ud is now owned by dupe_srf. 
  // So, dupe_srf is deleted, ud will also be deleted.

  // Replace object. Note, we cannot reuse objref for it references
  // the picked face, not the top level brep. 
  // Note, dupe_obj is now owned by Rhino, so we are not 
  // responsible for deleting it.
  if (!context.m_doc.ReplaceObject(CRhinoObjRef(obj), dupe_obj))
  {
    delete dupe_obj;
    return CRhinoCommand::failure;
  }

  // Done deal
  RhinoApp().Print(L"Data attached successfully.\n");
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleBrepFaceUserData command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetBrepFaceUserData command
//

#pragma region SampleGetBrepFaceUserData command

class CCommandSampleGetBrepFaceUserData : public CRhinoCommand
{
public:
  CCommandSampleGetBrepFaceUserData() {}
  ~CCommandSampleGetBrepFaceUserData() {}
  UUID CommandUUID()
  {
    // {5B941346-E07F-4E03-85BD-3515853AA4EE}
    static const GUID SampleGetBrepFaceUserDataCommand_UUID =
    { 0x5B941346, 0xE07F, 0x4E03, { 0x85, 0xBD, 0x35, 0x15, 0x85, 0x3A, 0xA4, 0xEE } };
    return SampleGetBrepFaceUserDataCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGetBrepFaceUserData"; }
  const wchar_t* LocalCommandName() const { return L"SampleGetBrepFaceUserData"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleGetBrepFaceUserData object
static class CCommandSampleGetBrepFaceUserData theSampleGetBrepFaceUserDataCommand;

CRhinoCommand::result CCommandSampleGetBrepFaceUserData::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  // Allow for picking of either a surface or a brep face.
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface to attach data");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(TRUE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Get first object reference. Note, this reference
  // reprsents the picked surface or face, not the
  // top level brep.
  const CRhinoObjRef& objref = go.Object(0);

  // Get face on brep that was picked
  const ON_BrepFace* face = objref.Face();
  if (0 == face)
    return CRhinoCommand::failure;

  // Get the faces underlying surface
  const ON_Surface* srf = face->SurfaceOf();
  if (0 == srf)
    return CRhinoCommand::failure;

  // Query surface for user data
  CSampleObjectUserData* ud = CSampleObjectUserData::Cast(srf->GetUserData(CSampleObjectUserData::Id()));
  if (0 == ud)
  {
    RhinoApp().Print(L"No Data attached.\n");
    return CRhinoCommand::nothing;
  }

  ON_wString pointstr;
  RhinoFormatPoint(ud->m_point, pointstr);
  RhinoApp().Print(L"point = %s, string = %s\n", pointstr, ud->m_string);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleGetBrepFaceUserData command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
