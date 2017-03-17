#include "stdafx.h"
#include "SampleClippingPlaneUserData.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDisableAllClippingPlanes command
//

#pragma region SampleDisableAllClippingPlanes command

class CCommandSampleDisableAllClippingPlanes : public CRhinoCommand
{
public:
  CCommandSampleDisableAllClippingPlanes() {}
  ~CCommandSampleDisableAllClippingPlanes() {}
  UUID CommandUUID()
  {
    // {23E01DCF-FC76-4930-A9BE-545AFF2ACE2B}
    static const GUID SampleDisableAllClippingPlanesCommand_UUID =
    { 0x23E01DCF, 0xFC76, 0x4930, { 0xA9, 0xBE, 0x54, 0x5A, 0xFF, 0x2A, 0xCE, 0x2B } };
    return SampleDisableAllClippingPlanesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDisableAllClippingPlanes"; }
  const wchar_t* LocalCommandName() const { return L"SampleDisableAllClippingPlanes"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDisableAllClippingPlanes object
static class CCommandSampleDisableAllClippingPlanes theSampleDisableAllClippingPlanesCommand;

CRhinoCommand::result CCommandSampleDisableAllClippingPlanes::RunCommand(const CRhinoCommandContext& context)
{
  ON_SimpleArray<UUID> viewport_ids;
  const int viewport_count = CSampleClippingPlaneUserData::StandardViewportIds(context.m_doc, viewport_ids);
  if (0 == viewport_count)
    return CRhinoCommand::nothing;

  CRhinoObjectIterator it(CRhinoObjectIterator::undeleted_objects, CRhinoObjectIterator::active_objects);
  it.SetObjectFilter(ON::clipplane_object);

  int num_disabled = 0;

  const CRhinoObject* obj = 0;
  for (obj = it.First(); obj; obj = it.Next())
  {
    const CRhinoClippingPlaneObject* clip_obj = CRhinoClippingPlaneObject::Cast(obj);
    if (clip_obj)
    {
      ON_SimpleArray<UUID> clipped_ids;
      for (int i = 0; i < viewport_count; i++)
      {
        if (clip_obj->ClipsViewport(viewport_ids[i]))
          clipped_ids.Append(viewport_ids[i]);
      }

      if (0 == clipped_ids.Count())
        continue;

      if (CSampleClippingPlaneUserData::AddUserData(context.m_doc, clip_obj, clipped_ids))
        num_disabled++;
    }
  }

  if (0 == num_disabled)
    RhinoApp().Print(L"0 clipping planes disabled.\n");
  else
  {
    if (1 == num_disabled)
      RhinoApp().Print(L"1 clipping plane disabled.\n");
    else
      RhinoApp().Print(L"%d clipping planes disabled.\n", num_disabled);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDisableAllClippingPlanes command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
