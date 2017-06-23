#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleZoomExtents command
//

#pragma region SampleZoomExtents command

class CCommandSampleZoomExtents : public CRhinoCommand
{
public:
  CCommandSampleZoomExtents() {}
  ~CCommandSampleZoomExtents() {}
  UUID CommandUUID()
  {
    // {62DF7080-8297-4107-8CB1-483204675809}
    static const GUID SampleZoomExtentsCommand_UUID =
    { 0x62DF7080, 0x8297, 0x4107, { 0x8C, 0xB1, 0x48, 0x32, 0x04, 0x67, 0x58, 0x09 } };
    return SampleZoomExtentsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleZoomExtents"; }
  const wchar_t* LocalCommandName() const { return L"SampleZoomExtents"; }
  CRhinoCommand::result RunCommand( const CRhinoCommandContext& );

private:
 void ZoomExtentsHelper(CRhinoDoc& doc, bool bAll, bool bRedraw);
};

// The one and only CCommandSampleZoomExtents object
static class CCommandSampleZoomExtents theSampleZoomExtentsCommand;

CRhinoCommand::result CCommandSampleZoomExtents::RunCommand( const CRhinoCommandContext& context )
{
  ZoomExtentsHelper(context.m_doc, true, true);
  return CRhinoCommand::success;
}

void CCommandSampleZoomExtents::ZoomExtentsHelper(CRhinoDoc& doc, bool bAll, bool bRedraw)
{
  if (bAll)
  {
    ON_SimpleArray<CRhinoView*> views(4);
    doc.GetViewList(views, true, false);
    for (int i = 0; i < views.Count(); i++)
    {
      CRhinoView* view = views[i];
      if (0 != view)
        RhinoDollyExtents(view);
    }

    if (bRedraw)
      doc.Redraw();
  }
  else
  {
    CRhinoView* view = RhinoApp().ActiveView();
    if (0 != view)
    {
      RhinoDollyExtents(view);

      if (bRedraw)
        view->Redraw();
    }
  }
}

#pragma endregion

//
// END SampleZoomExtents command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
