#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleUserText command
//

#pragma region SampleUserText command

class CCommandSampleUserText : public CRhinoCommand
{
public:
  CCommandSampleUserText() {}
  ~CCommandSampleUserText() {}
  UUID CommandUUID()
  {
    // {D518AB2A-D364-4325-9C25-121A0EFB1585}
    static const GUID SampleUserTextCommand_UUID =
    { 0xD518AB2A, 0xD364, 0x4325, { 0x9C, 0x25, 0x12, 0x1A, 0x0E, 0xFB, 0x15, 0x85 } };
    return SampleUserTextCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleUserText"; }
  const wchar_t* LocalCommandName() const { return L"SampleUserText"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
};

// The one and only CCommandSampleUserText object
static class CCommandSampleUserText theSampleUserTextCommand;

CRhinoCommand::result CCommandSampleUserText::RunCommand( const CRhinoCommandContext& context )
{
  bool bAttachToGeometry = false;

	CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects");
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 0);
  if (CRhinoCommand::success != go.CommandResult())
    return go.CommandResult();
  
  CRhinoCommandOptionName AttachToOption(L"AttachTo", L"AttachTo");
  CRhinoCommandOptionValue AttachToGeometry(L"Object", L"Object");
  CRhinoCommandOptionValue AttachToAttributes(L"Attributes", L"Attributes");

  ON_wString key, value;

  CRhinoGetString gs;
  gs.AddCommandOptionToggle(AttachToOption, AttachToAttributes, AttachToGeometry, bAttachToGeometry, &bAttachToGeometry);
  for(;;)
  {
    if (key.IsEmpty())
      gs.SetCommandPrompt(L"Text key");
    else
      gs.SetCommandPrompt(L"Text");

    CRhinoGet::result res = gs.GetString();
    if (CRhinoGet::option == res)
      continue;
    else if (CRhinoGet::string != res)
      return gs.CommandResult();

    ON_wString str = gs.String();
    str.TrimLeftAndRight();
    if (str.IsEmpty())
      return CRhinoCommand::nothing;

    if (key.IsEmpty())
    {
      key = str;
      continue;
    }

    value = str;
    break;
  }

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& object_ref = go.Object(i);
    const CRhinoObject* object = object_ref.Object();
    if (0 == object)
      return CRhinoCommand::failure;

    if (bAttachToGeometry)
    {
      if (ON::light_object == object->ObjectType())
      {
        const CRhinoLight* light_obj = CRhinoLight::Cast(object);
        if (light_obj)
        {
          ON_Light light = light_obj->Light();
          light.SetUserString(key, value);
          context.m_doc.m_light_table.ModifyLight(light, light_obj->LightIndex());
        }
      }
      else
      {
        CRhinoObject* new_object = object->DuplicateRhinoObject();
        if (0 != new_object)
        {
          ON_Geometry* geometry = const_cast<ON_Geometry*>(new_object->Geometry());
          if (0 != geometry)
          {
            geometry->SetUserString(key, value);
            if (!context.m_doc.ReplaceObject(object_ref, new_object, true))
              delete new_object; // Don't leak...
          }
          else
            delete new_object; // Don't leak...
        }
      }
    }
    else
    {
      CRhinoObjectAttributes new_attributes = object->Attributes();
      new_attributes.SetUserString(key, value);
      context.m_doc.ModifyObjectAttributes(object_ref, new_attributes);
    }
  }

  context.m_doc.Redraw();

	return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleUserText command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
