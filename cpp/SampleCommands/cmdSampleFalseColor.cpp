#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleFalseColor command
//

class CSampleFalseColorConduit : public CRhinoDisplayConduit
{
public:
  CSampleFalseColorConduit(const ON_Mesh* pMesh = 0);
  void SetFalseColorMesh(const ON_Mesh* pMesh);
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel);
  const ON_Mesh* m_pMesh;
};

CSampleFalseColorConduit::CSampleFalseColorConduit(const ON_Mesh* pMesh)
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_DRAWOVERLAY)
  , m_pMesh(pMesh)
{
}

void CSampleFalseColorConduit::SetFalseColorMesh(const ON_Mesh* pMesh)
{
  m_pMesh = pMesh;
}

bool CSampleFalseColorConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  UNREFERENCED_PARAMETER(bTerminateChannel);

  if (m_pMesh && m_pMesh->IsValid())
  {
    if (nActiveChannel == CSupportChannels::SC_CALCBOUNDINGBOX)
    {
      m_pChannelAttrs->m_BoundingBox.Union(m_pMesh->BoundingBox());
    }
    else if (nActiveChannel == CSupportChannels::SC_DRAWOVERLAY)
    {
      // Turn off lighting when drawing this mesh...
      m_pDisplayAttrs->m_pMaterial->m_FrontMaterial.SetDisableLighting(true);
      m_pDisplayAttrs->m_pMaterial->m_BackMaterial.SetDisableLighting(true);
      dp.DrawShadedMesh(*m_pMesh);
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleFalseColor : public CRhinoCommand
{
public:
  CCommandSampleFalseColor() {}
  ~CCommandSampleFalseColor() {}
  UUID CommandUUID()
  {
    // {226E8751-5805-4E52-B198-3C905EB12679}
    static const GUID SampleFalseColorCommand_UUID =
    { 0x226E8751, 0x5805, 0x4E52, { 0xB1, 0x98, 0x3C, 0x90, 0x5E, 0xB1, 0x26, 0x79 } };
    return SampleFalseColorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleFalseColor"; }
  const wchar_t* LocalCommandName() { return L"SampleFalseColor"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  ON_Color FalseColor(double z) const;
};

// The one and only CCommandSampleFalseColor object
static class CCommandSampleFalseColor theSampleFalseColorCommand;

CRhinoCommand::result CCommandSampleFalseColor::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoMeshDensity mesh_density;
  mesh_density.m_around = 20;
  mesh_density.m_vertical = 20;

  ON_Plane plane(ON_3dPoint(-10, 0, -10), ON_3dVector::XAxis, ON_3dVector::ZAxis);
  ON_Mesh* mesh = RhinoMeshPlane(plane, ON_Interval(0, 20), ON_Interval(0, 20), mesh_density);
  if (0 == mesh)
    return CRhinoCommand::failure;

  const int vcount = mesh->m_V.Count();
  ON_SimpleArray<ON_Color>& vertex_colors = mesh->m_C;
  vertex_colors.SetCount(0);
  vertex_colors.Reserve(vcount);
  for (int vi = 0; vi < vcount; vi++)
  {
    double z = mesh->m_V[vi].z;
    ON_Color c = FalseColor(z);
    vertex_colors.Append(c);
  }

  CSampleFalseColorConduit conduit;
  conduit.SetFalseColorMesh(mesh);
  conduit.Enable(context.m_doc.RuntimeSerialNumber());
  context.m_doc.Regen();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  conduit.SetFalseColorMesh(0);

  delete mesh;
  mesh = 0;

  context.m_doc.Regen();

  return CRhinoCommand::success;
}

ON_Color CCommandSampleFalseColor::FalseColor(double z) const
{
  ON_Interval z_range(-10.0, 10.0);
  ON_Interval hue_range(0.0, 4.0 * ON_PI / 3.0);

  double s = z_range.NormalizedParameterAt(z);
  if (s < 0.0)
    s = 0.0;
  else if (s > 1.0)
    s = 1.0;

  double hue = hue_range.ParameterAt(s);

  ON_Color c;
  c.SetHSV(hue, 1.0, 1.0);

  return c;
}

//
// END SampleFalseColor command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
