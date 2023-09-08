#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetCurvesWithValues command
//

#pragma region SampleGetCurvesWithValues command

class CCommandSampleGetCurvesWithValues : public CRhinoCommand
{
public:
  CCommandSampleGetCurvesWithValues();
  UUID CommandUUID() override
  {
    // {C1C39219-860B-40BA-B217-88AD17CCFDEB}
    static const GUID SampleGetCurvesWithValuesCommand_UUID =
    { 0xC1C39219, 0x860B, 0x40BA, { 0xB2, 0x17, 0x88, 0xAD, 0x17, 0xCC, 0xFD, 0xEB } };
    return SampleGetCurvesWithValuesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleGetCurvesWithValues"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  double m_default_value;
};

// The one and only CCommandSampleGetCurvesWithValues object
static class CCommandSampleGetCurvesWithValues theSampleGetCurvesWithValuesCommand;

CCommandSampleGetCurvesWithValues::CCommandSampleGetCurvesWithValues()
  : m_default_value(1.0)
{
}

CRhinoCommand::result CCommandSampleGetCurvesWithValues::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_ClassArray<CRhinoObjRef> objects;
  ON_SimpleArray<double> values;

  double old_value = m_default_value;

  CRhinoGetObject go;
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableReferenceObjectSelect(false);
  go.EnableUnselectObjectsOnExit(false);
  go.AcceptNothing();
  go.AcceptNumber();
  go.EnableHighlight(false);

  for (;;)
  {
    if (objects.Count() > 0)
    {
      go.EnableDeselectAllBeforePostSelect(false);
      go.SetCommandPrompt(L"Select curves. Press Enter when done");
    }
    else
    {
      go.SetCommandPrompt(L"Select curves");
    }

    if (m_default_value == 0.0)
      m_default_value = (old_value > 0.0) ? old_value : 1.0;
    old_value = m_default_value;

    go.ClearCommandOptions();
    go.AddCommandOptionNumber(RHCMDOPTNAME(L"NextValue"), &m_default_value, L"Next value", false, 0.0);

    CRhinoGet::result res = go.GetObjects(1, -1);
    go.EnablePreSelect(FALSE);

    bool bCtrlDown = ::GetKeyState(VK_CONTROL) < 0;
    bool bShiftDown = ::GetKeyState(VK_SHIFT) < 0;

    if (res == CRhinoGet::nothing && objects.Count() > 0)
      break;

    if (res == CRhinoGet::option)
      continue;

    if (res == CRhinoGet::number)
    {
      if (fabs(go.Number()) > 0.0)
        m_default_value = fabs(go.Number());
      continue;
    }

    if (res == CRhinoGet::cancel)
      return CRhinoCommand::cancel;

    if (res != CRhinoGet::object)
      return CRhinoCommand::failure;

    if (go.ObjectCount() < 1)
      return CRhinoCommand::failure;

    bool bAdd = true;
    if (bCtrlDown || bShiftDown)
    {
      bAdd = false;

      if (bShiftDown)
      {
        int sel_count = 0;
        for (int i = 0; i < go.ObjectCount(); i++)
        {
          for (int j = 0; j < objects.Count(); j++)
          {
            if (objects[j] == go.Object(i))
              sel_count++;
          }
        }
        if (sel_count != go.ObjectCount())
          bAdd = true;
      }
    }

    if (!bAdd && bCtrlDown)
    {
      // remove selected edges from the set
      for (int i = 0; i < go.ObjectCount(); i++)
      {
        go.Object(i).Object()->Select(false, true);
        go.Object(i).Object()->Highlight(false);
        for (int j = objects.Count() - 1; j >= 0; j--)
        {
          if (objects[j] == go.Object(i))
          {
            objects.Remove(j);
            values.Remove(j);
          }
        }
      }
    }
    else
    {
      for (int i = 0; i < go.ObjectCount(); i++)
      {
        go.Object(i).Object()->Select(false, true);
        go.Object(i).Object()->Highlight(true);
        objects.Append(go.Object(i));
        values.Append(m_default_value);
      }
    }
    doc->Redraw();

    continue;
  }

  for (int i = 0; i < objects.Count(); i++)
  {
    ON_wString str;
    ON_UuidToString(objects[i].ObjectUuid(), str);
    RhinoApp().Print(L"%s = %g\n", static_cast<const wchar_t*>(str), values[i]);
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleGetCurvesWithValues command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
