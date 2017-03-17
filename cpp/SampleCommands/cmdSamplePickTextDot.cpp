#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePickTextDot command
//

class CGetTextDotObject : public CRhinoGetObject
{
  bool CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const;
};

bool CGetTextDotObject::CustomGeometryFilter(const CRhinoObject* object, const ON_Geometry* geometry, ON_COMPONENT_INDEX component_index) const
{
  UNREFERENCED_PARAMETER(object);
  UNREFERENCED_PARAMETER(component_index);
  bool rc = false;
  if (geometry)
  {
    const ON_TextDot* p = ON_TextDot::Cast(geometry);
    if (p)
      rc = true;
  }
  return rc;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSamplePickTextDot : public CRhinoCommand
{
public:
  CCommandSamplePickTextDot() {}
  ~CCommandSamplePickTextDot() {}
  UUID CommandUUID()
  {
    // {24FFE2B4-E176-4D41-BCB0-B4E8E7C25D48}
    static const GUID SamplePickTextDotCommand_UUID =
    { 0x24FFE2B4, 0xE176, 0x4D41, { 0xBC, 0xB0, 0xB4, 0xE8, 0xE7, 0xC2, 0x5D, 0x48 } };
    return SamplePickTextDotCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePickTextDot"; }
  const wchar_t* LocalCommandName() { return L"SamplePickTextDot"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSamplePickTextDot object
static class CCommandSamplePickTextDot theSamplePickTextDotCommand;

CRhinoCommand::result CCommandSamplePickTextDot::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CGetTextDotObject go;
  go.SetCommandPrompt(L"Select text dots");
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const int object_count = go.ObjectCount();
  int i;
  for (i = 0; i < object_count; i++)
  {
    const ON_TextDot* p = ON_TextDot::Cast(go.Object(i).Geometry());
    if (p)
    {
      ON_wString sPoint;
      RhinoFormatPoint(p->CenterPoint(), sPoint);
      RhinoApp().Print(L"TextDot%d: point = (%s), text = \"%s\"\n", i, sPoint, p->PrimaryText());
    }
  }

  return CRhinoCommand::success;
}

//
// END SamplePickTextDot command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
