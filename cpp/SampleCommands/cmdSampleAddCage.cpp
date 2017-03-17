#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAddCage command
//

#pragma region SampleAddCage command

class CCommandSampleAddCage : public CRhinoCommand
{
public:
  CCommandSampleAddCage() {}
  ~CCommandSampleAddCage() {}
  UUID CommandUUID()
  {
    // {8AC799E1-353-45FD-8292-FA1B2E899557}
    static const GUID SampleAddCageCommand_UUID =
    { 0x8AC799E1, 0x353, 0x45FD, { 0x82, 0x92, 0xFA, 0x1B, 0x2E, 0x89, 0x95, 0x57 } };
    return SampleAddCageCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleAddCage"; }
  const wchar_t* LocalCommandName() const { return L"SampleAddCage"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleAddCage object
static class CCommandSampleAddCage theSampleAddCageCommand;

CRhinoCommand::result CCommandSampleAddCage::RunCommand(const CRhinoCommandContext& context)
{
  ON_3dPoint box_corners[8];
  CArgsRhinoGetBox args;

  CRhinoCommand::result rc = RhinoGetBox(args, box_corners);
  if (rc == CRhinoCommand::success)
  {
    int degree[3] = { 3,3,3 };   // defaults
    int cv_count[3] = { 4,4,4 }; // defaults

    ON_NurbsCage nurbs_cage;
    if (nurbs_cage.Create(
      box_corners,
      degree[0] + 1, degree[1] + 1, degree[2] + 1,
      cv_count[0], cv_count[1], cv_count[2])
      )
    {
      CRhinoCageObject* cage_object = new CRhinoCageObject();
      cage_object->SetCage(nurbs_cage);
      context.m_doc.AddObject(cage_object);
      context.m_doc.Redraw();
    }
  }

  return rc;
}

#pragma endregion

//
// END SampleAddCage command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
