#include "stdafx.h"

static double GetBoxVolume(const ON_3dPoint box_corners[8])
{
  ON_3dVector A = box_corners[1] - box_corners[0];
  ON_3dVector B = box_corners[3] - box_corners[0];
  ON_3dVector C = box_corners[4] - box_corners[0];
  return A * ON_CrossProduct(B, C);
}

static ON_Extrusion* MakeExtrusionBox(const ON_3dPoint box_corners[8])
{
  // make certain box_corners are orthoganal
  const int i1 = (GetBoxVolume(box_corners) < 0.0) ? 3 : 1;
  const int i3 = (1 == i1) ? 3 : 1;

  ON_Plane plane;
  plane.origin = box_corners[0];
  plane.xaxis = box_corners[i1] - box_corners[0];
  plane.yaxis = box_corners[i3] - box_corners[0];
  plane.zaxis = box_corners[4] - box_corners[0];

  const double dx = plane.xaxis.Length();
  const double dy = plane.yaxis.Length();
  const double dz = plane.zaxis.Length();

  plane.xaxis.Unitize();
  plane.yaxis.Unitize();
  plane.zaxis.Unitize();
  plane.UpdateEquation();
  if (!plane.IsValid())
    return 0;

  ON_Line path(box_corners[0], box_corners[4]);

  ON_PolylineCurve* profile = new ON_PolylineCurve();
  profile->m_dim = 2;
  profile->m_t.Append(0.0);
  profile->m_t.Append(dx);
  profile->m_t.Append(dx + dy);
  profile->m_t.Append(2.0 * dx + dy);
  profile->m_t.Append(2.0 * (dx + dy));
  profile->m_pline.Append(ON_3dPoint(0.0, 0.0, 0.0));
  profile->m_pline.Append(ON_3dPoint(dx, 0.0, 0.0));
  profile->m_pline.Append(ON_3dPoint(dx, dy, 0.0));
  profile->m_pline.Append(ON_3dPoint(0.0, dy, 0.0));
  profile->m_pline.Append(ON_3dPoint(0.0, 0.0, 0.0));

  if (!profile->IsValid())
  {
    delete profile;
    return 0;
  }

  ON_Extrusion* extrusion = new ON_Extrusion();
  if (!extrusion->SetPathAndUp(path.from, path.to, plane.yaxis))
  {
    delete profile;
    delete extrusion;
    return 0;
  }

  if (!extrusion->SetOuterProfile(profile, true))
  {
    delete profile;
    delete extrusion;
    return 0;
  }

  if (!extrusion->SetDomain(extrusion->PathParameter(), 0.0, dz))
  {
    delete extrusion;
    return 0;
  }

  if (!extrusion->IsValid())
  {
    delete extrusion;
    return 0;
  }

  return extrusion;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleExtrusionBox command
//

#pragma region SampleExtrusionBox command

class CCommandSampleExtrusionBox : public CRhinoCommand
{
public:
  CCommandSampleExtrusionBox() {}
  ~CCommandSampleExtrusionBox() {}
  UUID CommandUUID()
  {
    // {3A130CC8-6C01-4268-BFF8-E9530585CB34}
    static const GUID SampleExtrusionBoxCommand_UUID =
    { 0x3A130CC8, 0x6C01, 0x4268, { 0xBF, 0xF8, 0xE9, 0x53, 0x05, 0x85, 0xCB, 0x34 } };
    return SampleExtrusionBoxCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleExtrusionBox"; }
  const wchar_t* LocalCommandName() const { return L"SampleExtrusionBox"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleExtrusionBox object
static class CCommandSampleExtrusionBox theSampleExtrusionBoxCommand;

CRhinoCommand::result CCommandSampleExtrusionBox::RunCommand(const CRhinoCommandContext& context)
{
  CArgsRhinoGetBox args;
  ON_3dPoint box_corners[8];
  CRhinoCommand::result rc = RhinoGetBox(args, box_corners);
  if (rc != CRhinoCommand::success)
    return rc;

  ON_Extrusion* extrusion_box = MakeExtrusionBox(box_corners);
  if (0 == extrusion_box)
    return CRhinoCommand::failure;

  CRhinoExtrusionObject* extrusion_object = new CRhinoExtrusionObject();
  extrusion_object->SetExtrusion(extrusion_box);
  extrusion_box = 0;

  context.m_doc.AddObject(extrusion_object);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleExtrusionBox command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
