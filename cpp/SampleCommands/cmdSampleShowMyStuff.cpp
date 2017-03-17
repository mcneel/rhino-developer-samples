#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleShowMyStuff command
//

class CSampleShowMyStuffConduit : public CRhinoDisplayConduit
{
public:
  CSampleShowMyStuffConduit();
  void SetDisplayObjects(UUID viewport_id, const ON_SimpleArray<ON_UUID>& object_ids);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel);

private:
  ON_SimpleArray<ON_UUID> m_object_ids;
  ON_UUID m_viewport_id;
};

CSampleShowMyStuffConduit::CSampleShowMyStuffConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_DRAWOBJECT)
{
  m_object_ids.SetCapacity(16);
  m_viewport_id = ON_nil_uuid;
}

void CSampleShowMyStuffConduit::SetDisplayObjects(UUID viewport_id, const ON_SimpleArray<ON_UUID>& object_ids)
{
  m_viewport_id = viewport_id;
  m_object_ids.Empty();
  m_object_ids.Append(object_ids.Count(), object_ids.Array());
  m_object_ids.HeapSort(ON_UuidCompare);
}

bool CSampleShowMyStuffConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  UNREFERENCED_PARAMETER(bTerminateChannel);

  // If drawing objects
  if (nActiveChannel == CSupportChannels::SC_DRAWOBJECT)
  {
    // See if the viewport being drawn is the one I don't want my objects to appear in
    if (0 == ON_UuidCompare(m_viewport_id, dp.GetRhinoVP()->ViewportId()))
    {
      // Get the object being drawn
      const CRhinoObject* object = m_pChannelAttrs->m_pObject;
      if (object)
      {
        // See if its in my list of "don't draw objects"
        if (-1 == m_object_ids.BinarySearch(&object->Attributes().m_uuid, ON_UuidCompare))
          // Don't draw it....
          m_pChannelAttrs->m_bDrawObject = false;
      }
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleShowMyStuff : public CRhinoCommand
{
public:
  CCommandSampleShowMyStuff() {}
  ~CCommandSampleShowMyStuff() {}
  UUID CommandUUID()
  {
    // {AAF0F89A-EDF2-444C-8454-594D4D15DE49}
    static const GUID SampleShowMyStuffCommand_UUID =
    { 0xAAF0F89A, 0xEDF2, 0x444C, { 0x84, 0x54, 0x59, 0x4D, 0x4D, 0x15, 0xDE, 0x49 } };
    return SampleShowMyStuffCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleShowMyStuff"; }
  const wchar_t* LocalCommandName() { return L"SampleShowMyStuff"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleShowMyStuff object
static class CCommandSampleShowMyStuff theSampleShowMyStuffCommand;

CRhinoCommand::result CCommandSampleShowMyStuff::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to show");
  go.GetObjects(1, 0);
  if (go.CommandResult() != success)
    return go.CommandResult();

  const int object_count = go.ObjectCount();
  ON_SimpleArray<ON_UUID> object_ids(object_count);

  int i;
  for (i = 0; i < object_count; i++)
  {
    const CRhinoObject* object = go.Object(i).Object();
    if (0 == object)
      continue;

    object_ids.Append(object->Attributes().m_uuid);
  }

  if (0 == object_ids.Count())
    return nothing;

  ON_UUID viewport_id = ON_nil_uuid;
  CRhinoView* view = go.View();
  if (view)
    viewport_id = view->ActiveViewport().ViewportId();

  CSampleShowMyStuffConduit conduit;
  conduit.SetDisplayObjects(viewport_id, object_ids);
  conduit.Enable(context.m_doc.RuntimeSerialNumber());

  context.m_doc.Redraw(CRhinoView::regenerate_display_hint);

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleShowMyStuff command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
