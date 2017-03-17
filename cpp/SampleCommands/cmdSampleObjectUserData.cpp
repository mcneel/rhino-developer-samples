#include "StdAfx.h"
#include "SampleObjectUserData.h"

class CGetObjectWithSampleObjectUserData : public CRhinoGetObject
{
public:
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const
  {
    UNREFERENCED_PARAMETER(object);
    UNREFERENCED_PARAMETER(geometry);
    UNREFERENCED_PARAMETER(component_index);
    if (object && CSampleObjectUserData::HasSampleObjectUserData(object))
      return true;
    return false;
  }
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectUserDataAdd command
//

class CCommandSampleObjectUserDataAdd : public CRhinoCommand
{
public:
  CCommandSampleObjectUserDataAdd() {}
  ~CCommandSampleObjectUserDataAdd() {}
  UUID CommandUUID()
  {
    // {4F98D5D-FCB-4E96-91A8-1925D140D114}
    static const GUID SampleObjectUserDataAddCommand_UUID =
    { 0x4F98D5D, 0xFCB, 0x4E96, { 0x91, 0xA8, 0x19, 0x25, 0xD1, 0x40, 0xD1, 0x14 } };
    return SampleObjectUserDataAddCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleObjectUserDataAdd"; }
  const wchar_t* LocalCommandName() { return L"SampleObjectUserDataAdd"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleObjectUserDataAdd object
static class CCommandSampleObjectUserDataAdd theSampleObjectUserDataAddCommand;

CRhinoCommand::result CCommandSampleObjectUserDataAdd::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object");
  go.GetObjects(1, 1);
  if (go.CommandResult() != success)
    return go.CommandResult();

  const CRhinoObject* object = go.Object(0).Object();
  if (0 == object)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Pick point");
  gp.GetPoint();
  if (gp.CommandResult() != success)
    return gp.CommandResult();

  CRhinoGetString gs;
  gp.SetCommandPrompt(L"Description");
  gs.GetString();
  if (gs.CommandResult() != success)
    return gs.CommandResult();

  bool rc = CSampleObjectUserData::AddSampleObjectUserData(context.m_doc, object, gp.Point(), gs.String());
  RhinoApp().Print(L"%s\n", rc ? L"Succeeded!" : L"Failed!");

  return CRhinoCommand::success;
}

//
// END SampleObjectUserDataAdd command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectUserDataGet command
//

class CCommandSampleObjectUserDataGet : public CRhinoCommand
{
public:
  CCommandSampleObjectUserDataGet() {}
  ~CCommandSampleObjectUserDataGet() {}
  UUID CommandUUID()
  {
    // {91DC4B69-DD98-4560-8E79-28422FAFD525}
    static const GUID SampleObjectUserDataGetCommand_UUID =
    { 0x91DC4B69, 0xDD98, 0x4560, { 0x8E, 0x79, 0x28, 0x42, 0x2F, 0xAF, 0xD5, 0x25 } };
    return SampleObjectUserDataGetCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleObjectUserDataGet"; }
  const wchar_t* LocalCommandName() { return L"SampleObjectUserDataGet"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleObjectUserDataGet object
static class CCommandSampleObjectUserDataGet theSampleObjectUserDataGetCommand;

CRhinoCommand::result CCommandSampleObjectUserDataGet::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CGetObjectWithSampleObjectUserData go;
  go.SetCommandPrompt(L"Select object");
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != success)
    return go.CommandResult();

  const CRhinoObject* object = go.Object(0).Object();
  if (0 == object)
    return CRhinoCommand::failure;

  ON_3dPoint point;
  ON_wString string;
  bool rc = CSampleObjectUserData::GetSampleObjectUserData(object, point, string);
  if (rc)
  {
    ON_wString pointstr;
    RhinoFormatPoint(point, pointstr);
    RhinoApp().Print(L"point = %s, string = %s\n", pointstr, string);
  }
  else
  {
    RhinoApp().Print(L"Failed!\n");
  }

  return CRhinoCommand::success;
}

//
// END SampleObjectUserDataGet command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectUserDataRemove command
//

class CCommandSampleObjectUserDataRemove : public CRhinoCommand
{
public:
  CCommandSampleObjectUserDataRemove() {}
  ~CCommandSampleObjectUserDataRemove() {}
  UUID CommandUUID()
  {
    // {247E307B-DDB8-4E31-910F-3C4EC94B2089}
    static const GUID SampleObjectUserDataRemoveCommand_UUID =
    { 0x247E307B, 0xDDB8, 0x4E31, { 0x91, 0x0F, 0x3C, 0x4E, 0xC9, 0x4B, 0x20, 0x89 } };
    return SampleObjectUserDataRemoveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleObjectUserDataRemove"; }
  const wchar_t* LocalCommandName() { return L"SampleObjectUserDataRemove"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleObjectUserDataRemove object
static class CCommandSampleObjectUserDataRemove theSampleObjectUserDataRemoveCommand;

CRhinoCommand::result CCommandSampleObjectUserDataRemove::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object");
  go.GetObjects(1, 1);
  if (go.CommandResult() != success)
    return go.CommandResult();

  const CRhinoObject* object = go.Object(0).Object();
  if (0 == object)
    return CRhinoCommand::failure;

  bool rc = CSampleObjectUserData::RemoveSampleObjectUserData(context.m_doc, object);
  RhinoApp().Print(L"%s\n", rc ? L"Succeeded!" : L"Failed!");

  return CRhinoCommand::success;
}

//
// END SampleObjectUserDataRemove command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
