#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRayShooter command
//

class CCommandSampleRayShooter : public CRhinoCommand
{
public:
  CCommandSampleRayShooter() {}
  ~CCommandSampleRayShooter() {}
  UUID CommandUUID()
  {
    // {F5A16944-CBA6-4939-84AD-A48BFB0CEE42}
    static const GUID SampleRayShooterCommand_UUID =
    { 0xF5A16944, 0xCBA6, 0x4939, { 0x84, 0xAD, 0xA4, 0x8B, 0xFB, 0x0C, 0xEE, 0x42 } };
    return SampleRayShooterCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleRayShooter"; }
  const wchar_t* LocalCommandName() { return L"SampleRayShooter"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleRayShooter object
static class CCommandSampleRayShooter theSampleRayShooterCommand;

CRhinoCommand::result CCommandSampleRayShooter::RunCommand(const CRhinoCommandContext& context)
{
  // Select reflecting surfaces
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select reflecting surfaces");
  go.SetGeometryFilter(ON::brep_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Build surface tree list
  ON_SimpleArray<const ON_SurfaceTreeNode*> stree_list;
  CMap<unsigned int, unsigned int, ON_BrepFace*, ON_BrepFace*> stree_map;

  int i;
  for (i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoBrepObject* brep_object = CRhinoBrepObject::Cast(go.Object(i).Object());
    if (brep_object)
    {
      const ON_Brep* brep = brep_object->Brep();
      if (brep)
      {
        for (int fi = 0; fi < brep->m_F.Count(); fi++)
        {
          ON_BrepFace* face = brep->Face(fi);
          if (face)
          {
            const ON_SurfaceTree* stree = face->SurfaceTree();
            if (stree)
            {
              stree_list.Append(stree);
              stree_map.SetAt(stree->m_treesn, face);
            }
          }
        }
      }
    }
  }

  if (stree_list.Count() < 1)
  {
    RhinoApp().Print(L"No suitable surfaces were selected.\n");
    return CRhinoCommand::failure;
  }

  // Start of ray
  ON_Line line;
  CRhinoGetPoint gp;
  gp.SetCommandPrompt(L"Start of ray");
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  line.from = gp.Point();

  // Ray direction
  gp.SetCommandPrompt(L"Ray direction");
  gp.SetBasePoint(line.from);
  gp.DrawLineFromPoint(line.from, TRUE);
  gp.GetPoint();
  if (gp.CommandResult() != CRhinoCommand::success)
    return gp.CommandResult();

  line.to = gp.Point();

  if (!line.IsValid() || line.Length() < ON_ZERO_TOLERANCE)
    return CRhinoCommand::nothing;

  // Start shooting...

  const int max_reflections = 15;

  ON_3dPoint P = line.from; // start of ray
  ON_3dVector D = line.Direction(); // ray direction

  // Polyline = path through the of reflected ray
  ON_Polyline polyline(max_reflections + 1);
  polyline.Append(P);

  ON_RayShooter ray;
  ON_X_EVENT hit;

  ON_3dPoint Q = P;
  ON_3dVector R = D;

  for (i = 0; i <= max_reflections; i++)
  {
    ON_3dVector T = R;
    if (!T.Unitize())
      break;

    // Shoot the ray
    memset(&hit, 0, sizeof(hit));
    if (!ray.Shoot(Q, T, stree_list.Count(), stree_list.Array(), hit))
      break;

    // Get the "hit" surface tree node
    const ON_SurfaceTreeNode* stn = hit.m_snodeB[0];
    if (0 == stn)
      stn = hit.m_snodeB[1];
    if (0 == stn)
      break;

    // Look up the brep face, based on the surface tree node's serial number
    ON_BrepFace* face = 0;
    if (!stree_map.Lookup(stn->m_treesn, face) || 0 == face)
      break;

    // Verify the hit point was on the active region of the face
    if (!RhinoIsPointOnFace(*face, context.m_doc.AbsoluteTolerance(), true, hit.m_b[0], hit.m_b[1]))
      break;

    Q = hit.m_B[0]; // surface point
    // Add next point to polyline
    polyline.Append(Q);

    // Set R = reflection direction
    ON_3dVector N = hit.m_B[1]; // surface normal
    double d = -2.0 * (N.x * T.x + N.y * T.y + N.z * T.z);
    R.x = T.x + d * N.x;
    R.y = T.y + d * N.y;
    R.z = T.z + d * N.z;

    ray.m_min_travel_distance = hit.m_A[0].DistanceTo(hit.m_B[0]);
    if (ray.m_min_travel_distance < 1.0e-8)
      ray.m_min_travel_distance = 1.0e-8;
  }

  if (polyline.Count() >= 2)
  {
    context.m_doc.AddCurveObject(polyline);
    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

//
// END SampleRayShooter command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
