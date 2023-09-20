#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePickContext command
//

#pragma region SamplePickContext command

class CCommandSamplePickContext : public CRhinoCommand
{
public:
  CCommandSamplePickContext() = default;
  UUID CommandUUID() override
  {
    // {5332C9D-D405-4134-8ECC-7F8D32B1495C}
    static const GUID SamplePickContextCommand_UUID =
    { 0x5332C9D, 0xD405, 0x4134,{ 0x8E, 0xCC, 0x7F, 0x8D, 0x32, 0xB1, 0x49, 0x5C } };
    return SamplePickContextCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePickContext"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSamplePickContext object
static class CCommandSamplePickContext theSamplePickContextCommand;

CRhinoCommand::result CCommandSamplePickContext::RunCommand(const CRhinoCommandContext& context)
{
  /*
  When picking subobjects with a CRhinoPickContext, a CRhinoGetObject object is need to indicate
  what should be picked. The  CRhinoPickContext.m_bSubSelect flag only enables subobject tests,
  in addition to everything else the picker is doing by default.
  */

  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  CRhinoPickContext pick_context;

  CRhinoGetObject go;
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  pick_context.m_go = &go;
  pick_context.m_pick_style = CRhinoPickContext::point_pick;
  pick_context.m_bSubSelect = true;

  CRhinoGetPoint gp;
  gp.SetGetPointCursor(RhinoApp().m_default_cursor);
  gp.Get2dPoint();
  if (CRhinoGet::point2d != gp.Result())
    return gp.CommandResult();

  pick_context.m_view = gp.View();
  CRhinoViewport& vp = gp.View()->ActiveViewport();
  vp.SetClippingRegionTransformation(gp.Point2d().x, gp.Point2d().y, pick_context.m_pick_region);
  vp.VP().GetFrustumLine(gp.Point2d().x, gp.Point2d().y, pick_context.m_pick_line);

  CRhinoObjRefArray pick_list;
  doc->PickObjects(pick_context, pick_list);
  if (pick_list.Count() > 0 && pick_list[0].Edge())
    RhinoApp().Print(L"Edge picked.\n");
  else
    RhinoApp().Print(L"Edge missed.\n");

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePickContext command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
