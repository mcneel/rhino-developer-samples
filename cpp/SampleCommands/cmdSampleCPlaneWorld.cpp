#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCPlaneWorld command
//

class CRhinoWorldConstructionPlanes
{
public:
  static ON_3dmConstructionPlane Back();
  static ON_3dmConstructionPlane Back(const ON_3dmConstructionPlane& cplane);

  static ON_3dmConstructionPlane Bottom();
  static ON_3dmConstructionPlane Bottom(const ON_3dmConstructionPlane& cplane);

  static ON_3dmConstructionPlane Front();
  static ON_3dmConstructionPlane Front(const ON_3dmConstructionPlane& cplane);

  static ON_3dmConstructionPlane Left();
  static ON_3dmConstructionPlane Left(const ON_3dmConstructionPlane& cplane);

  static ON_3dmConstructionPlane Right();
  static ON_3dmConstructionPlane Right(const ON_3dmConstructionPlane& cplane);

  static ON_3dmConstructionPlane Top();
  static ON_3dmConstructionPlane Top(const ON_3dmConstructionPlane& cplane);
};

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Back()
{
  return CRhinoWorldConstructionPlanes::Back(ON_3dmConstructionPlane());
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Back(const ON_3dmConstructionPlane& cplane)
{
  ON_3dmConstructionPlane cp(cplane);
  cp.m_plane.CreateFromFrame(ON_origin, -ON_xaxis, ON_zaxis);
  cp.m_name = L"Back";
  return cp;
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Bottom()
{
  return CRhinoWorldConstructionPlanes::Bottom(ON_3dmConstructionPlane());
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Bottom(const ON_3dmConstructionPlane& cplane)
{
  ON_3dmConstructionPlane cp(cplane);
  cp.m_plane.CreateFromFrame(ON_origin, ON_xaxis, -ON_yaxis);
  cp.m_name = L"Bottom";
  return cp;
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Front()
{
  return CRhinoWorldConstructionPlanes::Front(ON_3dmConstructionPlane());
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Front(const ON_3dmConstructionPlane& cplane)
{
  ON_3dmConstructionPlane cp(cplane);
  cp.m_plane.CreateFromFrame(ON_origin, ON_xaxis, ON_zaxis);
  cp.m_name = L"Front";
  return cp;
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Left()
{
  return CRhinoWorldConstructionPlanes::Left(ON_3dmConstructionPlane());
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Left(const ON_3dmConstructionPlane& cplane)
{
  ON_3dmConstructionPlane cp(cplane);
  cp.m_plane.CreateFromFrame(ON_origin, -ON_yaxis, ON_zaxis);
  cp.m_name = L"Left";
  return cp;
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Right()
{
  return CRhinoWorldConstructionPlanes::Right(ON_3dmConstructionPlane());
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Right(const ON_3dmConstructionPlane& cplane)
{
  ON_3dmConstructionPlane cp(cplane);
  cp.m_plane.CreateFromFrame(ON_origin, ON_yaxis, ON_zaxis);
  cp.m_name = L"Right";
  return cp;
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Top()
{
  return CRhinoWorldConstructionPlanes::Top(ON_3dmConstructionPlane());
}

ON_3dmConstructionPlane CRhinoWorldConstructionPlanes::Top(const ON_3dmConstructionPlane& cplane)
{
  ON_3dmConstructionPlane cp(cplane);
  cp.m_plane.CreateFromFrame(ON_origin, ON_xaxis, ON_yaxis);
  cp.m_name = L"Top";
  return cp;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleCPlaneWorld command

class CCommandSampleCPlaneWorld : public CRhinoCommand
{
public:
  CCommandSampleCPlaneWorld() = default;
  UUID CommandUUID() override
  {
    // {C8B8266E-91-4322-8B55-0B3C54D630B9}
    static const GUID SampleCPlaneWorldCommand_UUID =
    { 0xC8B8266E, 0x91, 0x4322, { 0x8B, 0x55, 0x0B, 0x3C, 0x54, 0xD6, 0x30, 0xB9 } };
    return SampleCPlaneWorldCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCPlaneWorld"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleCPlaneWorld object
static class CCommandSampleCPlaneWorld theSampleCPlaneWorldCommand;

CRhinoCommand::result CCommandSampleCPlaneWorld::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoView* pView = RhinoApp().ActiveView();
  if (nullptr == pView)
    return CRhinoCommand::failure;

  CRhinoGetOption go;
  go.SetCommandPrompt(L"Choose world CPlane");
  go.AcceptNothing();
  const int index_back = go.AddCommandOption(RHCMDOPTNAME(L"Back"));
  const int index_bottom = go.AddCommandOption(RHCMDOPTNAME(L"Bottom"));
  const int index_front = go.AddCommandOption(RHCMDOPTNAME(L"Front"));
  const int index_left = go.AddCommandOption(RHCMDOPTNAME(L"Left"));
  const int index_right = go.AddCommandOption(RHCMDOPTNAME(L"Right"));
  const int index_top = go.AddCommandOption(RHCMDOPTNAME(L"Top"));
  CRhinoGet::result res = go.GetOption();
  if (res != CRhinoGet::option)
    return CRhinoCommand::cancel;

  const CRhinoCommandOption* pOption = go.Option();
  if (nullptr == pOption)
    return CRhinoCommand::failure;

  const int option_index = pOption->m_option_index;
  const ON_3dmConstructionPlane& old_cplane = pView->ActiveViewport().ConstructionPlane();
  ON_3dmConstructionPlane new_cplane;

  if (option_index == index_back)
    new_cplane = CRhinoWorldConstructionPlanes::Back(old_cplane);
  else if (option_index == index_bottom)
    new_cplane = CRhinoWorldConstructionPlanes::Bottom(old_cplane);
  else if (option_index == index_front)
    new_cplane = CRhinoWorldConstructionPlanes::Front(old_cplane);
  else if (option_index == index_left)
    new_cplane = CRhinoWorldConstructionPlanes::Left(old_cplane);
  else if (option_index == index_right)
    new_cplane = CRhinoWorldConstructionPlanes::Right(old_cplane);
  else if (option_index == index_top)
    new_cplane = CRhinoWorldConstructionPlanes::Top(old_cplane);
  else
    return CRhinoCommand::failure;

  pView->ActiveViewport().PushConstructionPlane(new_cplane);
  pView->Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCPlaneWorld command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
