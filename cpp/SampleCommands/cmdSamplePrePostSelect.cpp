#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePrePostSelect command
//

class CCommandSamplePrePostSelect : public CRhinoCommand
{
public:
  CCommandSamplePrePostSelect();
  ~CCommandSamplePrePostSelect() {}
  UUID CommandUUID()
  {
    // {1A9E21B0-6E2-440E-A397-9D4BCD87320E}
    static const GUID SamplePrePostSelectCommand_UUID =
    { 0x1A9E21B0, 0x6E2, 0x440E, { 0xA3, 0x97, 0x9D, 0x4B, 0xCD, 0x87, 0x32, 0x0E } };
    return SamplePrePostSelectCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePrePostSelect"; }
  const wchar_t* LocalCommandName() { return L"SamplePrePostSelect"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  double m_dValue;
  int m_nValue;
};

// The one and only CCommandSamplePrePostSelect object
static class CCommandSamplePrePostSelect theSamplePrePostSelectCommand;

CCommandSamplePrePostSelect::CCommandSamplePrePostSelect()
{
  m_dValue = 1.0;
  m_nValue = 1;
}

CRhinoCommand::result CCommandSamplePrePostSelect::RunCommand(const CRhinoCommandContext& context)
{
  double dValue = m_dValue;
  int nValue = m_nValue;

  CRhinoGetObject go;
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableGroupSelect(TRUE);
  go.EnableSubObjectSelect(FALSE);

  /*int d_option_index =*/ go.AddCommandOptionNumber(
    RHCMDOPTNAME(L"Double"), &dValue, L"Double value", FALSE, 1.0, 99.9);

  /*int n_option_index =*/ go.AddCommandOptionInteger(
    RHCMDOPTNAME(L"Integer"), &nValue, L"Integer value", 1, 99);

  bool bHavePreselectedObjects = false;

  for (;; )
  {
    CRhinoGet::result res = go.GetObjects(1, 0);

    if (res == CRhinoGet::option)
    {
      go.EnablePreSelect(FALSE);
      go.EnableAlreadySelectedObjectSelect(true);
      go.EnableClearObjectsOnEntry(false);
      go.EnableDeselectAllBeforePostSelect(false);
      go.EnableUnselectObjectsOnExit(false);
      continue;
    }

    else if (res != CRhinoGet::object)
      return CRhinoCommand::cancel;

    if (go.ObjectsWerePreSelected())
    {
      bHavePreselectedObjects = true;
      go.EnablePreSelect(FALSE);
      go.EnableAlreadySelectedObjectSelect(true);
      go.EnableClearObjectsOnEntry(false);
      go.EnableDeselectAllBeforePostSelect(false);
      go.EnableUnselectObjectsOnExit(false);
      continue;
    }

    break;
  }

  if (bHavePreselectedObjects)
  {
    // Normally, pre-selected objects will remain selected, when a
    // command finishes, and post-selected objects will be unselected.
    // This this way of picking, it is possible to have a combination
    // of pre-selected and post-selected. So, to make sure everything
    // "looks the same", lets unselect everything before finishing
    // the command.
    for (int i = 0; i < go.ObjectCount(); i++)
    {
      const CRhinoObject* object = go.Object(i).Object();
      if (0 != object)
        object->Select(false);
    }
    context.m_doc.Redraw();
  }

  int object_count = go.ObjectCount();
  m_dValue = dValue;
  m_nValue = nValue;

  RhinoApp().Print(L"Select object count = %d\n", object_count);
  RhinoApp().Print(L"Value of double = %f\n", m_dValue);
  RhinoApp().Print(L"Value of integer = %d\n", m_nValue);

  return CRhinoCommand::success;
}

//
// END SamplePrePostSelect command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
