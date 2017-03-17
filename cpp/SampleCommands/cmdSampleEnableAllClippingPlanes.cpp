#include "stdafx.h"
#include "SampleClippingPlaneUserData.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleEnableAllClippingPlanes command
//

#pragma region SampleEnableAllClippingPlanes command

class CCommandSampleEnableAllClippingPlanes : public CRhinoCommand
{
public:
  CCommandSampleEnableAllClippingPlanes() {}
  ~CCommandSampleEnableAllClippingPlanes() {}
  UUID CommandUUID()
  {
    // {A8A4E5B7-8D0-49E3-933E-1DA58132FB56}
    static const GUID SampleEnableAllClippingPlanesCommand_UUID =
    { 0xA8A4E5B7, 0x8D0, 0x49E3, { 0x93, 0x3E, 0x1D, 0xA5, 0x81, 0x32, 0xFB, 0x56 } };
    return SampleEnableAllClippingPlanesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleEnableAllClippingPlanes"; }
  const wchar_t* LocalCommandName() const { return L"SampleEnableAllClippingPlanes"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleEnableAllClippingPlanes object
static class CCommandSampleEnableAllClippingPlanes theSampleEnableAllClippingPlanesCommand;

CRhinoCommand::result CCommandSampleEnableAllClippingPlanes::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoObjectIterator it(CRhinoObjectIterator::undeleted_objects, CRhinoObjectIterator::active_objects);
  it.SetObjectFilter(ON::clipplane_object);

  int num_enabled = 0;

  const CRhinoObject* obj = 0;
  for (obj = it.First(); obj; obj = it.Next())
  {
    const CRhinoClippingPlaneObject* clip_obj = CRhinoClippingPlaneObject::Cast(obj);
    if (clip_obj)
    {
      if (CSampleClippingPlaneUserData::RemoveUserData(context.m_doc, clip_obj))
        num_enabled++;
    }
  }

  if (0 == num_enabled)
    RhinoApp().Print(L"0 clipping planes enabled.\n");
  else
  {
    if (1 == num_enabled)
      RhinoApp().Print(L"1 clipping plane enabled.\n");
    else
      RhinoApp().Print(L"%d clipping planes enabled.\n", num_enabled);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleEnableAllClippingPlanes command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
