// cmdSampleCustomRenderMesh.cpp : command file
//

#include "StdAfx.h"
#include "SampleCustomRenderMeshPlugIn.h"
#include "SampleCustomRenderMeshUserData.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomRenderMesh command
//

#pragma region SampleCustomRenderMesh command

// Do NOT put the definition of class CCommandSampleCustomRenderMesh in a header
// file. There is only ONE instance of a CCommandSampleCustomRenderMesh class
// and that instance is the static theSampleCustomRenderMeshCommand that appears
// immediately below the class definition.

class CCommandSampleCustomRenderMesh : public CRhinoCommand
{
public:
  // The one and only instance of CCommandSampleCustomRenderMesh is created below.
  // No copy constructor or operator= is required.
  // Values of member variables persist for the duration of the application.

  // CCommandSampleCustomRenderMesh::CCommandSampleCustomRenderMesh()
  // is called exactly once when static theSampleCustomRenderMeshCommand is created.
  CCommandSampleCustomRenderMesh() = default;

  // CCommandSampleCustomRenderMesh::~CCommandSampleCustomRenderMesh()
  // is called exactly once when static theSampleCustomRenderMeshCommand is destroyed.
  // The destructor should not make any calls to the Rhino SDK. 
  // If your command has persistent settings, then override 
  // CRhinoCommand::SaveProfile and CRhinoCommand::LoadProfile.
  ~CCommandSampleCustomRenderMesh() = default;

  // Returns a unique UUID for this command.
  // If you try to use an id that is already being used, then
  // your command will not work. Use GUIDGEN.EXE to make unique UUID.
  UUID CommandUUID() override
  {
    // {ABC52CE5-6564-4A37-B97E-D886CEFF04BB}
    static const GUID SampleCustomRenderMeshCommand_UUID =
    { 0xABC52CE5, 0x6564, 0x4A37, { 0xB9, 0x7E, 0xD8, 0x86, 0xCE, 0xFF, 0x4, 0xBB } };
    return SampleCustomRenderMeshCommand_UUID;
  }

  // Returns the English command name.
  // If you want to provide a localized command name, then override 
  // CRhinoCommand::LocalCommandName.
  const wchar_t* EnglishCommandName() override { return L"SampleCustomRenderMesh"; }

  // Rhino calls RunCommand to run the command.
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleCustomRenderMesh object
// Do NOT create any other instance of a CCommandSampleCustomRenderMesh class.
static class CCommandSampleCustomRenderMesh theSampleCustomRenderMeshCommand;

static bool SetAmount(CRhinoDoc& doc, GUID objectId, double amount)
{
  const CRhinoObject* pObject = doc.LookupObject(objectId);
  if (pObject == nullptr)
    return false;

  CRhinoObjectAttributes newAttrs(pObject->Attributes());

  CSampleCustomRenderMeshUserData* pNewUserData = new CSampleCustomRenderMeshUserData();
  if (pNewUserData == nullptr)
    return false;

  pNewUserData->SetAmount(amount);

  ON_UserData* pOldUserData = newAttrs.GetUserData(SampleCustomRenderMeshUserDataUuid);
  if (pOldUserData != nullptr)
  {
    newAttrs.DetachUserData(pOldUserData);
  }


  if (!newAttrs.AttachUserData(pNewUserData))
  {
    delete pNewUserData;
    return false;
  }

  if (!doc.ModifyObjectAttributes(CRhinoObjRef(objectId), newAttrs))
    return false;

  return true;
}

static bool GetAmount(const CRhinoObject& object, double& amount)
{
  const CSampleCustomRenderMeshUserData* pUserData = CSampleCustomRenderMeshUserData::Cast(object.Attributes().GetUserData(SampleCustomRenderMeshUserDataUuid));
  if (pUserData == nullptr)
    return false;

  amount = pUserData->Amount();
  return true;
}

CRhinoCommand::result CCommandSampleCustomRenderMesh::RunCommand(const CRhinoCommandContext& context)
{
  if (context.Document() == nullptr)
    return CRhinoCommand::failure;
  CRhinoDoc& doc = *context.Document();

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select polysurface object");
  go.SetGeometryFilter(CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObject* pObject = go.Object(0).Object();
  if (0 == pObject)
    return CRhinoCommand::failure;

  double amount = 0.0;
  GetAmount(*pObject, amount);

  CRhinoGetNumber gn;
  gn.SetDefaultNumber(amount);
  int optAmount = gn.AddCommandOptionNumber(RHCMDOPTNAME(L"Amount"), &amount);
  while (true)
  {
    gn.SetDefaultNumber(amount);
    CRhinoGet::result gnr = gn.GetNumber();
    if (gnr == CRhinoGet::number)
    {
      amount = gn.Number();
      if (!SetAmount(doc, pObject->Id(), amount))
        return CRhinoCommand::failure;
      RhRdkCustomRenderMeshManager().OnRhinoObjectChanged(doc, pObject);
      return CRhinoCommand::success;
    }
    else if (gnr == CRhinoGet::result::option)
    {
      if (gn.OptionIndex() == optAmount)
      {
        if (!SetAmount(doc, pObject->Id(), amount))
          return CRhinoCommand::failure;
        RhRdkCustomRenderMeshManager().OnRhinoObjectChanged(doc, pObject);
      }
    }
    else if (gnr == CRhinoGet::result::nothing)
    {
      if (!SetAmount(doc, pObject->Id(), amount))
        return CRhinoCommand::failure;
      RhRdkCustomRenderMeshManager().OnRhinoObjectChanged(doc, pObject);
      return CRhinoCommand::success;
    }
    else
    {
      return gn.CommandResult();
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCustomRenderMesh command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
