#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleNewFloatingViewport command
//

#pragma region SampleNewFloatingViewport command

class CCommandSampleNewFloatingViewport : public CRhinoCommand
{
public:
  CCommandSampleNewFloatingViewport() {}
  ~CCommandSampleNewFloatingViewport() {}
  UUID CommandUUID()
  {
    // {55850077-8755-4054-AA92-6A925889A2A3}
    static const GUID SampleNewFloatingViewportCommand_UUID =
    { 0x55850077, 0x8755, 0x4054, { 0xAA, 0x92, 0x6A, 0x92, 0x58, 0x89, 0xA2, 0xA3 } };
    return SampleNewFloatingViewportCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleNewFloatingViewport"; }
  const wchar_t* LocalCommandName() const { return L"SampleNewFloatingViewport"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleNewFloatingViewport object
static class CCommandSampleNewFloatingViewport theSampleNewFloatingViewportCommand;

CRhinoCommand::result CCommandSampleNewFloatingViewport::RunCommand(const CRhinoCommandContext& context)
{
  ON_SimpleArray<ON_UUID> viewport_ids;
  ON_SimpleArray<CRhinoView*> view_list;
  CRhinoView* view = 0;
  int i = 0;

  // Build a list of (current) viewport ids
  context.m_doc.GetViewList(view_list, true, false);
  for (i = 0; i < view_list.Count(); i++)
  {
    view = view_list[i];
    if (view)
      viewport_ids.Append(view->ActiveViewportID());
  }
  view_list.Empty();

  // Create a new view
  context.m_doc.NewView(ON_3dmView(), true);

  // Find the viewport (id) that was just created
  context.m_doc.GetViewList(view_list, true, false);
  for (i = 0; i < view_list.Count(); i++)
  {
    view = view_list[i];
    if (view)
    {
      int rc = viewport_ids.Search(view->ActiveViewportID());
      if (rc < 0)
        break;
      else
        view = 0;
    }
  }

  // Make any necessary updates
  if (view)
  {
    ON_3dmView v = view->ActiveViewport().View();
    v.m_name = L"New View";
    view->ActiveViewport().SetView(v);
    view->FloatRhinoView(true);
    view->Redraw();
  }

  return CRhinoCommand::success;  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleNewFloatingViewport command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
