#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleBooleanUnion command
//

#pragma region NodePoint class

class NodePoint
{
public:
  NodePoint();
  NodePoint(double x, double y, double z);
  NodePoint(double x, double y, double z, double r);

  bool IsValid() const;

  ON_3dPoint Point() const;
  double Radius() const;

  static double DefaultRadius();

public:
  double m_x;
  double m_y;
  double m_z;
  double m_r;
};

NodePoint::NodePoint()
{
  m_x = ON_UNSET_VALUE;
  m_y = ON_UNSET_VALUE;
  m_z = ON_UNSET_VALUE;
  m_r = ON_UNSET_VALUE;
}

NodePoint::NodePoint(double x, double y, double z)
{
  m_x = x;
  m_y = y;
  m_z = z;
  m_r = NodePoint::DefaultRadius();
}

NodePoint::NodePoint(double x, double y, double z, double r)
{
  m_x = x;
  m_y = y;
  m_z = z;
  m_r = r;
}

bool NodePoint::IsValid() const
{
  bool rc = ON_IsValid(m_x);
  if (rc)
    rc = ON_IsValid(m_y);
  if (rc)
    rc = ON_IsValid(m_z);
  if (rc)
    rc = (m_r > 0.0);
  return rc;
}

ON_3dPoint NodePoint::Point() const
{
  return ON_3dPoint(m_x, m_y, m_z);
}

double NodePoint::Radius() const
{
  return m_r;
}

double NodePoint::DefaultRadius()
{
  return 0.1;
}

#pragma endregion


#pragma region SampleBooleanUnion command

class CCommandSampleBooleanUnion : public CRhinoCommand
{
public:
  CCommandSampleBooleanUnion() {}
  ~CCommandSampleBooleanUnion() {}
  UUID CommandUUID()
  {
    // {B2B511E9-BDC-4484-98F4-5F4805A77F55}
    static const GUID SampleBooleanUnionCommand_UUID =
    { 0xB2B511E9, 0xBDC, 0x4484, { 0x98, 0xF4, 0x5F, 0x48, 0x05, 0xA7, 0x7F, 0x55 } };
    return SampleBooleanUnionCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleBooleanUnion"; }
  const wchar_t* LocalCommandName() const { return L"SampleBooleanUnion"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  ON_Brep* CreateNode(const NodePoint& p);
  ON_Brep* CreateBar(const NodePoint& p0, const NodePoint& p1);
};

// The one and only CCommandSampleBooleanUnion object
static class CCommandSampleBooleanUnion theSampleBooleanUnionCommand;

CRhinoCommand::result CCommandSampleBooleanUnion::RunCommand(const CRhinoCommandContext& context)
{
  const int max_x = 2;
  const int max_y = 2;
  const int max_z = 2;
  const double step = 5;

  // Create points
  ON_SimpleArray<NodePoint> points;
  for (int z = 0; z < max_z; ++z)
  {
    for (int y = 0; y < max_y; ++y)
    {
      for (int x = 0; x < max_x; ++x)
      {
        points.Append(NodePoint(step*x, step*y, step*z));
      }
    }
  }

  ON_SimpleArray<const ON_Brep*> InBreps;

  // Create nodes
  for (int i = 0; i < points.Count(); i++)
  {
    ON_Brep* brep = CreateNode(points[i]);
    if (brep)
      InBreps.Append(brep);
  }

  // Create connecting bars
  for (int i = 0; i < points.Count() - 1; i++)
  {
    for (int j = i; j < points.Count(); j++)
    {
      if (i != j)
      {
        ON_Brep* brep = CreateBar(points[i], points[j]);
        if (brep)
          InBreps.Append(brep);
      }
    }
  }

  double tol = context.m_doc.AbsoluteTolerance();
  bool bResult = false;
  ON_SimpleArray<ON_Brep*> OutBreps;

  // Boolean union
  CWaitCursor wait;
  unsigned int start_tick = GetTickCount();
  bool rc = RhinoBooleanUnion(InBreps, tol, &bResult, OutBreps);
  RhinoApp().Print(L"Boolean Union elapsed time: %i milliseconds\n", GetTickCount() - start_tick);
  wait.Restore();

  // Clean up...
  for (int i = 0; i < InBreps.Count(); i++)
    delete InBreps[i];

  if (rc)
  {
    for (int i = 0; i < OutBreps.Count(); i++)
    {
      CRhinoBrepObject* brep_obj = new CRhinoBrepObject();
      brep_obj->SetBrep(OutBreps[i]);
      OutBreps[i] = 0;
      context.m_doc.AddObject(brep_obj);
    }
  }
  else
  {
    for (int i = 0; i < OutBreps.Count(); i++)
      delete OutBreps[i];
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

ON_Brep* CCommandSampleBooleanUnion::CreateNode(const NodePoint& p)
{
  ON_Brep* brep = 0;

  if (p.IsValid())
  {
    ON_Sphere sphere(p.Point(), p.Radius());
    ON_RevSurface* sphere_srf = sphere.RevSurfaceForm(true);
    if (sphere_srf)
    {
      brep = ON_Brep::New();
      brep->Create(sphere_srf);
      if (!brep->IsValid())
      {
        delete brep;
        brep = 0;
      }
    }
  }

  return brep;
}

ON_Brep* CCommandSampleBooleanUnion::CreateBar(const NodePoint& p0, const NodePoint& p1)
{
  ON_Brep* brep = 0;

  if (p0.IsValid() && p1.IsValid())
  {
    double radius = ((p0.Radius() + p1.Radius()) / 2.0) / 2.0;
    ON_3dVector zaxis = p1.Point() - p0.Point();
    ON_Plane plane(p0.Point(), zaxis);
    ON_Circle circle(plane, radius);
    ON_Cylinder cylinder(circle, zaxis.Length());
    brep = ON_BrepCylinder(cylinder, TRUE, TRUE);
    if (!brep)
    {
      delete brep;
      brep = 0;
    }
  }

  return brep;
}

#pragma endregion

//
// END SampleBooleanUnion command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
