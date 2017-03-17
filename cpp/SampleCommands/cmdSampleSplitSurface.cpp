#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSplitSurface command
//

#pragma region SampleSplitSurface command

static ON_BOOL32 SampleSurfaceSplitter(
  const ON_Surface* srf,
  const ON_3dPoint& pt,
  bool bUDir,
  ON_Surface*& west_or_south_side,
  ON_Surface*& east_or_north_side
)
{
  if (0 == srf || !srf->IsValid() || !pt.IsValid())
    return false;

  double u = ON_UNSET_VALUE;
  double v = ON_UNSET_VALUE;
  if (srf->GetClosestPoint(pt, &u, &v))
  {
    int dir = (bUDir) ? 0 : 1;
    double c = (bUDir) ? u : v;
    return srf->Split(dir, c, west_or_south_side, east_or_north_side);
  }

  return false;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleSplitSurface : public CRhinoCommand
{
public:
  CCommandSampleSplitSurface() {}
  ~CCommandSampleSplitSurface() {}
  UUID CommandUUID()
  {
    // {7C281BA6-A501-4134-8B1B-DBA5CA3A2E58}
    static const GUID SampleSplitSurfaceCommand_UUID =
    { 0x7C281BA6, 0xA501, 0x4134, { 0x8B, 0x1B, 0xDB, 0xA5, 0xCA, 0x3A, 0x2E, 0x58 } };
    return SampleSplitSurfaceCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSplitSurface"; }
  const wchar_t* LocalCommandName() const { return L"SampleSplitSurface"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSplitSurface object
static class CCommandSampleSplitSurface theSampleSplitSurfaceCommand;

CRhinoCommand::result CCommandSampleSplitSurface::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface to split in u direction");
  go.SetGeometryFilter(CRhinoGetObject::surface_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_BrepFace* face = go.Object(0).Face();
  if (0 == face)
    return CRhinoCommand::failure;

  const ON_Surface* srf = face->SurfaceOf();
  if (0 == srf)
    return CRhinoCommand::failure;

  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Point on surface to split at");
  gp.Constrain(*srf);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  ON_3dPoint pt = gp.Point();

  ON_Surface* split[] = { 0, 0 };
  if (SampleSurfaceSplitter(srf, pt, true, split[0], split[1]))
  {
    if (split[0])
    {
      CRhinoSurfaceObject* split_obj = new CRhinoSurfaceObject();
      split_obj->SetSurface(split[0]);
      context.m_doc.AddObject(split_obj);
    }

    if (split[1])
    {
      CRhinoSurfaceObject* split_obj = new CRhinoSurfaceObject();
      split_obj->SetSurface(split[1]);
      context.m_doc.AddObject(split_obj);
    }

    context.m_doc.DeleteObject(go.Object(0));
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSplitSurface command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
