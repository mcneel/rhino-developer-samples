#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleNetworkSrf command
//

#pragma region SampleNetworkSrf command

class CCommandSampleNetworkSrf : public CRhinoCommand
{
public:
  CCommandSampleNetworkSrf() = default;
  UUID CommandUUID() override
  {
    // {E5108DB6-B005-44FB-9243-05D6FBD7F5ED}
    static const GUID SampleNetworkSrfCommand_UUID =
    { 0xE5108DB6, 0xB005, 0x44FB,{ 0x92, 0x43, 0x05, 0xD6, 0xFB, 0xD7, 0xF5, 0xED } };
    return SampleNetworkSrfCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleNetworkSrf"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleNetworkSrf object
static class CCommandSampleNetworkSrf theSampleNetworkSrfCommand;

CRhinoCommand::result CCommandSampleNetworkSrf::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_SimpleArray<const ON_Curve*> ucurves, vcurves;
  int ucont[2] = { 1, 1 }; // 0 = Loose, 1 = Position, 2 = Tangency, 3 = Curvature
  int vcont[2] = { 1, 1 }; // 0 = Loose, 1 = Position, 2 = Tangency, 3 = Curvature

  CRhinoCommandOptionValue list[4] = { RHCMDOPTVALUE(L"Loose"), RHCMDOPTVALUE(L"Position"), RHCMDOPTVALUE(L"Tangency"), RHCMDOPTVALUE(L"Curvature") };

  CRhinoGetObject go;
  go.EnablePreSelect(false);
  go.SetGeometryFilter(CRhinoObject::curve_object);
  go.SetCommandPrompt(L"Select U curves in order, one by one. Press Enter when done");
  go.EnableDeselectAllBeforePostSelect(false);

  for (;;)
  {
    go.ClearCommandOptions();

    int u0 = go.AddCommandOptionList(RHCMDOPTNAME(L"UStartContinuity"), 4, list, ucont[0]);
    int u1 = go.AddCommandOptionList(RHCMDOPTNAME(L"UEndContinuity"), 4, list, ucont[1]);
    int as = -1;
    if (ucurves.Count() < 1)
      as = go.AddCommandOption(RHCMDOPTNAME(L"AutoSort"));

    CRhinoGet::result res = go.GetObjects(1, 1);

    if (res == CRhinoGet::option && go.Option())
    {
      if (go.Option()->m_option_index == as)
      {
        go.ClearCommandOptions();
        go.SetCommandPrompt(L"Select curves to autosort and build a network surface");
        go.AddCommandOptionList(RHCMDOPTNAME(L"Continuity"), 4, list, ucont[0]);
        for (;;)
        {
          go.GetObjects(3, 0);
          if (go.Result() == CRhinoGet::option && go.Option())
          {
            ucont[0] = go.Option()->m_list_option_current;
            continue;
          }
          break;
        }

        for (int i = 0; i < go.ObjectCount(); i++)
          ucurves.Append(go.Object(i).Curve());

        ON_NurbsSurface ns;
        if (0 == RhinoNetworkSurface(ucurves, ucont[0], ns))
        {
          doc->AddSurfaceObject(ns);
          doc->Redraw();
        }

        return CRhinoCommand::success;
      }

      if (go.Option()->m_option_index == u0)
        ucont[0] = go.Option()->m_list_option_current;
      else if (go.Option()->m_option_index == u1)
        ucont[1] = go.Option()->m_list_option_current;
      continue;
    }

    if (res != CRhinoGet::object)
      break;

    ucurves.Append(go.Object(0).Curve());
  }

  go.ClearCommandOptions();
  go.SetCommandPrompt(L"Select V curves in order, one by one. Press Enter when done");
  int v0 = go.AddCommandOptionList(RHCMDOPTNAME(L"VStartContinuity"), 4, list, vcont[0]);
  int v1 = go.AddCommandOptionList(RHCMDOPTNAME(L"VEndContinuity"), 4, list, vcont[1]);

  for (;;)
  {
    CRhinoGet::result res = go.GetObjects(1, 1);
    if (res == CRhinoGet::option && go.Option())
    {
      if (go.Option()->m_option_index == v0)
        vcont[0] = go.Option()->m_list_option_current;
      else if (go.Option()->m_option_index == v1)
        vcont[1] = go.Option()->m_list_option_current;
      continue;
    }

    if (res != CRhinoGet::object)
      break;

    vcurves.Append(go.Object(0).Curve());
  }

  ON_NurbsSurface ns;
  if (0 == RhinoNetworkSurface(ucurves, ucont, vcurves, vcont, ns))
  {
    doc->AddSurfaceObject(ns);
    doc->Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleNetworkSrf command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
