#include "StdAfx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSetUserText command
//

class CCommandSampleSetUserText : public CRhinoCommand
{
public:
  CCommandSampleSetUserText() {}
  ~CCommandSampleSetUserText() {}
  UUID CommandUUID()
  {
    // {EBBF656E-CE46-4CD3-ABD9-9CA008FEFBD8}
    static const GUID SampleSetUserTextCommand_UUID =
    { 0xEBBF656E, 0xCE46, 0x4CD3, { 0xAB, 0xD9, 0x9C, 0xA0, 0x08, 0xFE, 0xFB, 0xD8 } };
    return SampleSetUserTextCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSetUserText"; }
  const wchar_t* LocalCommandName() { return L"SampleSetUserText"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSetUserText object
static class CCommandSampleSetUserText theSampleSetUserTextCommand;

CRhinoCommand::result CCommandSampleSetUserText::RunCommand(const CRhinoCommandContext& context)
{
  bool bAttachToGeometry = false;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects");
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 0);
  if (CRhinoCommand::success != go.CommandResult())
    return go.CommandResult();

  CRhinoCommandOptionName AttachToOption = RHCMDOPTNAME(L"AttachTo");
  CRhinoCommandOptionValue AttachToGeometry = RHCMDOPTVALUE(L"Object");
  CRhinoCommandOptionValue AttachToAttributes = RHCMDOPTVALUE(L"Attributes");

  ON_wString key;
  for (;;)
  {
    CRhinoGetString gid;
    gid.SetCommandPrompt(L"Text key");
    gid.AddCommandOptionToggle(AttachToOption, AttachToAttributes, AttachToGeometry, bAttachToGeometry, &bAttachToGeometry);
    gid.GetString();
    if (CRhinoCommand::success != gid.CommandResult())
      return gid.CommandResult();
    if (CRhinoGet::string != gid.Result())
      continue;
    key = gid.String();
    break;
  }

  ON_wString string_value;
  for (;;)
  {
    CRhinoGetString gs;
    gs.SetCommandPrompt(L"Text");
    gs.AddCommandOptionToggle(AttachToOption, AttachToAttributes, AttachToGeometry, bAttachToGeometry, &bAttachToGeometry);
    gs.GetString();
    if (CRhinoCommand::success != gs.CommandResult())
      return gs.CommandResult();
    if (CRhinoGet::string != gs.Result())
      continue;
    string_value = gs.String();
    break;
  }

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& object_ref = go.Object(i);
    const CRhinoObject* object = object_ref.Object();
    if (0 == object)
      continue;

    if (bAttachToGeometry)
    {
      CRhinoObject* object_copy = object->DuplicateRhinoObject();
      if (object_copy)
      {
        ON_Geometry* geometry = const_cast<ON_Geometry*>(object_copy->Geometry());
        if (geometry)
        {
          geometry->SetUserString(key, string_value);
          bool rc = context.m_doc.ReplaceObject(object_ref, object_copy);
          if (!rc)
            delete object_copy; // don't leak...
        }
      }
    }
    else
    {
      CRhinoObjectAttributes attrib = object->Attributes();
      attrib.SetUserString(key, string_value);
      context.m_doc.ModifyObjectAttributes(object_ref, attrib, TRUE);
    }
  }

  return CRhinoCommand::success;
}

//
// END SampleSetUserText command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetUserText command
//

class CCommandSampleGetUserText : public CRhinoCommand
{
public:
  CCommandSampleGetUserText() {}
  ~CCommandSampleGetUserText() {}
  UUID CommandUUID()
  {
    // {DE95D543-BF3B-42D1-B147-94BA19C9484D}
    static const GUID SampleGetUserTextCommand_UUID =
    { 0xDE95D543, 0xBF3B, 0x42D1, { 0xB1, 0x47, 0x94, 0xBA, 0x19, 0xC9, 0x48, 0x4D } };
    return SampleGetUserTextCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGetUserText"; }
  const wchar_t* LocalCommandName() { return L"SampleGetUserText"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleGetUserText object
static class CCommandSampleGetUserText theSampleGetUserTextCommand;

CRhinoCommand::result CCommandSampleGetUserText::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object");
  go.GetObjects(1, 1);
  if (CRhinoCommand::success != go.CommandResult())
    return go.CommandResult();

  const CRhinoObject* object = go.Object(0).Object();
  if (0 == object)
    return CRhinoCommand::failure;

  const ON_Geometry* geometry = object->Geometry();
  if (0 == geometry)
    return CRhinoCommand::failure;

  ON_ClassArray<ON_UserString> user_geo_strings;
  ON_ClassArray<ON_UserString> user_att_strings;

  if (geometry->GetUserStrings(user_geo_strings))
  {
    int i, count = user_geo_strings.Count();
    RhinoApp().Print(L"%d object user strings.\n", count);
    for (i = 0; i < count; i++)
    {
      const wchar_t* key = user_geo_strings[i].m_key;
      const wchar_t* value = user_geo_strings[i].m_string_value;
      if (0 == key)
        key = L"";
      if (0 == value)
        value = L"";
      RhinoApp().Print(L"  <%s> %s.\n", key, value);
    }
  }

  if (object->Attributes().GetUserStrings(user_att_strings))
  {
    int i, count = user_att_strings.Count();
    RhinoApp().Print(L"%d attributes user strings.\n", count);
    for (i = 0; i < count; i++)
    {
      const wchar_t* key = user_att_strings[i].m_key;
      const wchar_t* value = user_att_strings[i].m_string_value;
      if (0 == key)
        key = L"";
      if (0 == value)
        value = L"";
      RhinoApp().Print(L"  <%s> %s.\n", key, value);
    }
  }

  return CRhinoCommand::success;
}

//
// END SampleGetUserText command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSetDocumentUserText command
//

class CCommandSampleSetDocumentUserText : public CRhinoCommand
{
public:
  CCommandSampleSetDocumentUserText() {}
  ~CCommandSampleSetDocumentUserText() {}
  UUID CommandUUID()
  {
    // {587DE9DF-11E4-41B5-B98C-06817DAC1BFB}
    static const GUID SampleSetDocumentUserTextCommand_UUID =
    { 0x587DE9DF, 0x11E4, 0x41B5, { 0xB9, 0x8C, 0x06, 0x81, 0x7D, 0xAC, 0x1B, 0xFB } };
    return SampleSetDocumentUserTextCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSetDocumentUserText"; }
  const wchar_t* LocalCommandName() { return L"SampleSetDocumentUserText"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSetDocumentUserText object
static class CCommandSampleSetDocumentUserText theSampleSetDocumentUserTextCommand;

CRhinoCommand::result CCommandSampleSetDocumentUserText::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Text key");
  gs.GetString();
  if (CRhinoCommand::success != gs.CommandResult())
    return gs.CommandResult();

  ON_wString key = gs.String();

  gs.SetCommandPrompt(L"Text");
  gs.GetString();
  if (CRhinoCommand::success != gs.CommandResult())
    return gs.CommandResult();

  ON_wString string_value = gs.String();

  context.m_doc.SetUserString(key, string_value);

  return CRhinoCommand::success;
}

//
// END SampleSetDocumentUserText command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetDocumentUserText command
//

class CCommandSampleGetDocumentUserText : public CRhinoCommand
{
public:
  CCommandSampleGetDocumentUserText() {}
  ~CCommandSampleGetDocumentUserText() {}
  UUID CommandUUID()
  {
    // {BFE6A00C-EBA7-474C-A9D3-BF7EAE6901F5}
    static const GUID SampleGetDocumentUserTextCommand_UUID =
    { 0xBFE6A00C, 0xEBA7, 0x474C, { 0xA9, 0xD3, 0xBF, 0x7E, 0xAE, 0x69, 0x01, 0xF5 } };
    return SampleGetDocumentUserTextCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGetDocumentUserText"; }
  const wchar_t* LocalCommandName() { return L"SampleGetDocumentUserText"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleGetDocumentUserText object
static class CCommandSampleGetDocumentUserText theSampleGetDocumentUserTextCommand;

CRhinoCommand::result CCommandSampleGetDocumentUserText::RunCommand(const CRhinoCommandContext& context)
{
  ON_ClassArray<ON_UserString> user_strings;

  if (context.m_doc.GetUserStrings(user_strings))
  {
    int i, count = user_strings.Count();
    RhinoApp().Print(L"%d document user strings.\n", count);
    for (i = 0; i < count; i++)
    {
      const wchar_t* key = user_strings[i].m_key;
      const wchar_t* value = user_strings[i].m_string_value;
      if (0 == key)
        key = L"";
      if (0 == value)
        value = L"";
      RhinoApp().Print(L"  <%s> %s.\n", key, value);
    }
  }

  return CRhinoCommand::success;
}

//
// END SampleGetDocumentUserText command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
