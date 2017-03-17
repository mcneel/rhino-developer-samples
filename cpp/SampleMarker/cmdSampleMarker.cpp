#include "StdAfx.h"
#include "SampleMarkerPlugIn.h"
#include "SampleMarkerObject.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMarker command
//

#pragma region SampleMarker command

class CCommandSampleMarker : public CRhinoCommand
{
public:
  CCommandSampleMarker() {}
  ~CCommandSampleMarker() {}
  UUID CommandUUID()
  {
    // {16A2C043-0235-4BB2-B11E-5A7AA29D65E5}
    static const GUID SampleMarkerCommand_UUID =
    { 0x16A2C043, 0x235, 0x4BB2, { 0xB1, 0x1E, 0x5A, 0x7A, 0xA2, 0x9D, 0x65, 0xE5 } };
    return SampleMarkerCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMarker"; }
  const wchar_t* LocalCommandName() const { return L"SampleMarker"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMarker object
static class CCommandSampleMarker theSampleMarkerCommand;

CRhinoCommand::result CCommandSampleMarker::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Location of marker object");
  gp.GetPoint();
  if (gp.CommandResult() == CRhinoCommand::success)
  {
    CSampleMarkerObject* marker = new CSampleMarkerObject();
    marker->SetPoint(gp.Point());
    context.m_doc.AddObject(marker);
    context.m_doc.Redraw();
  }

  return gp.CommandResult();
}

#pragma endregion

//
// END SampleMarker command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
