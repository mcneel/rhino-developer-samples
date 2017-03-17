#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleHighlightCurve command
//

class CSampleHighlightCurveConduit : public CRhinoDisplayConduit
{
public:
  CSampleHighlightCurveConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate);

public:
  unsigned int m_runtime_object_serial_number;
};

CSampleHighlightCurveConduit::CSampleHighlightCurveConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_DRAWOBJECT)
{
}

bool CSampleHighlightCurveConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(dp);
  UNREFERENCED_PARAMETER(bTerminate);

  switch (nChannel)
  {
    case CSupportChannels::SC_DRAWOBJECT:
    {
      if (m_pChannelAttrs->m_pObject->RuntimeSerialNumber() == m_runtime_object_serial_number)
        m_pDisplayAttrs->m_ObjectColor = RGB(255, 105, 180); // hot pink
    }
    break;
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleHighlightCurve : public CRhinoCommand
{
public:
  CCommandSampleHighlightCurve() {}
  ~CCommandSampleHighlightCurve() {}
  UUID CommandUUID()
  {
    // {56EA3FC7-6AB9-4432-AA0B-B0969F927360}
    static const GUID SampleHighlightCurveCommand_UUID =
    { 0x56EA3FC7, 0x6AB9, 0x4432, { 0xAA, 0x0B, 0xB0, 0x96, 0x9F, 0x92, 0x73, 0x60 } };
    return SampleHighlightCurveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleHighlightCurve"; }
  const wchar_t* LocalCommandName() { return L"SampleHighlightCurve"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleHighlightCurve object
static class CCommandSampleHighlightCurve theSampleHighlightCurveCommand;

CRhinoCommand::result CCommandSampleHighlightCurve::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to highlight");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObject* obj = go.Object(0).Object();
  if (0 == obj)
    return CRhinoCommand::failure;

  CSampleHighlightCurveConduit conduit;
  conduit.m_runtime_object_serial_number = obj->RuntimeSerialNumber();
  conduit.Enable(context.m_doc.RuntimeSerialNumber());
  context.m_doc.Redraw();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.GetString();

  conduit.Disable();
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleHighlightCurve command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
