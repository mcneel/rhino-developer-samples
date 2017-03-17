#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSporph command
//

#pragma region SampleSporph command

static CRhinoObject* RhinoSporphObject(
  const CRhinoObject* obj,
  const ON_Surface* srf0,
  const ON_Surface* srf1,
  const ON_2dPoint& srf0_pt,
  const ON_2dPoint& srf1_pt,
  bool bCopy = false
)
{
  const ON_Surface* srf[2];
  srf[0] = srf0;
  srf[1] = srf1;
  if (0 == obj || 0 == srf[0] || 0 == srf[1])
    return 0;

  CRhinoDoc* doc = obj->Document();
  if (0 == doc)
    return 0;

  ON_2dPoint st[2];
  st[0] = srf0_pt;
  st[1] = srf1_pt;

  if (!srf[0]->Domain(0).Includes(st[0].x) ||
    !srf[0]->Domain(1).Includes(st[0].y) ||
    !srf[1]->Domain(0).Includes(st[1].x) ||
    !srf[1]->Domain(1).Includes(st[1].y)
    )
    return 0;

  CRhinoSporphSpaceMorph morph;
  for (int i = 0; i < 2; i++)
  {
    morph.m_srf[i] = srf[i]->DuplicateSurface();
    if (0 == morph.m_srf[i])
      return 0;

    double s = srf[i]->Domain(0).NormalizedParameterAt(st[i].x);
    if (s > 0.5)
      morph.m_srf[i]->Reverse(0);
    morph.m_s[i] = morph.m_srf[i]->Domain(0);

    double t = srf[i]->Domain(1).NormalizedParameterAt(st[i].y);
    if (t > 0.5)
      morph.m_srf[i]->Reverse(1);
    morph.m_t[i] = morph.m_srf[i]->Domain(1);

    morph.m_srf[i]->SurfaceTree();
  }

  return doc->MorphObject(obj, morph, true, !bCopy);
}

class CCommandSampleSporph : public CRhinoCommand
{
public:
  CCommandSampleSporph() {}
  ~CCommandSampleSporph() {}
  UUID CommandUUID()
  {
    // {4FB3E288-EDC0-4B1A-B845-945CE87F6762}
    static const GUID SampleSporphCommand_UUID =
    { 0x4FB3E288, 0xEDC0, 0x4B1A, { 0xB8, 0x45, 0x94, 0x5C, 0xE8, 0x7F, 0x67, 0x62 } };
    return SampleSporphCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSporph"; }
  const wchar_t* LocalCommandName() const { return L"SampleSporph"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleSporph object
static class CCommandSampleSporph theSampleSporphCommand;

CRhinoCommand::result CCommandSampleSporph::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoCommand::result rc = CRhinoCommand::nothing;

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface or polysurface to sporph");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.GetObjects(1, 1);
  rc = go.CommandResult();
  if (rc != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  const CRhinoObject* obj = obj_ref.Object();
  if (0 == obj)
    return CRhinoCommand::failure;

  CRhinoGetObject gb;
  gb.SetCommandPrompt(L"Select base surface");
  gb.SetGeometryFilter(CRhinoGetObject::surface_object);
  gb.EnablePreSelect(FALSE);
  gb.EnableDeselectAllBeforePostSelect(false);
  gb.GetObjects(1, 1);
  rc = gb.CommandResult();
  if (rc != CRhinoCommand::success)
    return gb.CommandResult();

  const CRhinoObjRef& base_ref = gb.Object(0);
  const ON_Surface* base = base_ref.Surface();
  if (0 == base)
    return CRhinoCommand::failure;

  ON_2dPoint base_pt;
  if (0 == base_ref.SurfaceParameter(&base_pt[0], &base_pt[1]))
  {
    base_pt.x = base->Domain(0).Min();
    base_pt.y = base->Domain(1).Min();
  }

  CRhinoGetObject gt;
  gt.SetCommandPrompt(L"Select target surface");
  gt.SetGeometryFilter(CRhinoGetObject::surface_object);
  gt.EnablePreSelect(FALSE);
  gt.EnableDeselectAllBeforePostSelect(false);
  gt.GetObjects(1, 1);
  rc = gt.CommandResult();
  if (rc != CRhinoCommand::success)
    return gt.CommandResult();

  const CRhinoObjRef& target_ref = gt.Object(0);
  const ON_Surface* target = target_ref.Surface();
  if (0 == target)
    return CRhinoCommand::failure;

  ON_2dPoint target_pt;
  if (0 == target_ref.SurfaceParameter(&target_pt[0], &target_pt[1]))
  {
    target_pt.x = target->Domain(0).Min();
    target_pt.y = target->Domain(1).Min();
  }

  CRhinoObject* sporphed_obj = RhinoSporphObject(obj, base, target, base_pt, target_pt);
  if (sporphed_obj)
    context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSporph command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
