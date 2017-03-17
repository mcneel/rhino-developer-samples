#include "StdAfx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN Z analysis mode class
// 

// This is an example that demonstrates how to add a false color
// analysis mode to Rhino.  This example uses false color to indicate
// the world "z" coordinate.

// {D832A29C-786C-4AE3-A33F-B9BB6DA89B3A}
static const GUID FALSE_COLOR_EXAMPLE_ANALYSIS_MODE_ID = { 0xD832A29C, 0x786C, 0x4AE3, { 0xA3, 0x3F, 0xB9, 0xBB, 0x6D, 0xA8, 0x9B, 0x3A } };

class CZAnalysisVAM : public CRhinoVisualAnalysisMode
{
public:
  CZAnalysisVAM();
  ~CZAnalysisVAM();

  // virtual CRhinoVisualAnalysisMode override
  void GetAnalysisModeName(ON_wString& name) const;

  // virtual CRhinoVisualAnalysisMode override
  bool ObjectSupportsAnalysisMode(const CRhinoObject* object) const;

  // virtual CRhinoVisualAnalysisMode override
  void UpdateVertexColors(const CRhinoObject* object, ON_SimpleArray<const ON_Mesh *>& meshes) const;

  // virtual CRhinoVisualAnalysisMode override
  bool ShowIsoCurves() const;

  // virtual CRhinoVisualAnalysisMode override
  //void DrawMeshObject(const CRhinoMeshObject& mesh_object, CRhinoDisplayPipeline& dp);

  // virtual CRhinoVisualAnalysisMode override
  //void DrawBrepObject(const CRhinoBrepObject& brep_object, CRhinoDisplayPipeline& dp);

  bool m_bShowIsoCurves;

  // This simple example provides a false color based on the
  // world z coordinate.   For details, see the implementation
  // of the FalseColor function.
  ON_Interval m_z_range;
  ON_Interval m_hue_range;

  ON_Color FalseColor(double z) const;

  // Returns a mapping tag that is used to detect when
  // a meshes colors need to be set.  For details, see the
  // implementation  of MappingTag and UpdateVertexColors.
  ON_MappingTag MappingTag() const;
};


// the one and only instance of a CZAnalysisVAM object.
// The CRhinoVisualAnalysisMode constructor registers the mode
// with Rhino.  This class must not be destroyed while Rhino
// is active.
static class CZAnalysisVAM theZAnalysisVAM;

CZAnalysisVAM::CZAnalysisVAM()
  : CRhinoVisualAnalysisMode(FALSE_COLOR_EXAMPLE_ANALYSIS_MODE_ID, CRhinoVisualAnalysisMode::false_color_style)
{
  m_bShowIsoCurves = true;

  // In a real plug-in, user interface would allow
  // the user to change these intervals.

  m_z_range.Set(-10.0, 10.0);
  m_hue_range.Set(0.0, 4.0*ON_PI / 3.0); // red to green to blue
}

CZAnalysisVAM::~CZAnalysisVAM()
{
}

void CZAnalysisVAM::GetAnalysisModeName(ON_wString& name) const
{
  // This name shows up in the object properties details
  // report when the object is in the analysis mode.
  name = L"Z analysis";
}

bool CZAnalysisVAM::ObjectSupportsAnalysisMode(const CRhinoObject* object) const
{
  // This function should return true if the analysis mode works
  // on the object.  This example works on meshes and breps, so
  // its version of ObjectSupportsAnalysisMode looks like this.

  bool rc = false;
  if (object)
  {
    switch (object->ObjectType())
    {
    case ON::mesh_object:
      if (CRhinoMeshObject::Cast(object))
        rc = true;
      break;

    case ON::surface_object:
    case ON::polysrf_filter:
    case ON::brep_object:
      if (CRhinoBrepObject::Cast(object))
        rc = true;
      break;

    case ON::extrusion_object:
      if (CRhinoExtrusionObject::Cast(object))
        rc = true;
      break;
    }
  }
  return rc;
}

ON_MappingTag CZAnalysisVAM::MappingTag() const
{
  ON_MappingTag mt;

  // Since the false colors that are shown will change if
  // the mesh is transformed, we have to initialize the
  // transformation.
  mt.m_mesh_xform.Identity();

  // This is the analysis mode id passed to the 
  // CRhinoVisualAnalysisMode constructor. Use the
  // m_am_id member and it this code will alwasy 
  // work correctly.
  mt.m_mapping_id = m_am_id;

  // This is a 32 bit CRC or the information used to
  // set the false colors.
  // For this example, the m_z_range and m_hue_range
  // intervals controlthe colors, so we calculate 
  // their crc.
  mt.m_mapping_crc = 0;
  mt.m_mapping_crc = ON_CRC32(mt.m_mapping_crc, sizeof(m_z_range), &m_z_range);
  mt.m_mapping_crc = ON_CRC32(mt.m_mapping_crc, sizeof(m_hue_range), &m_hue_range);

  return mt;
}

ON_Color CZAnalysisVAM::FalseColor(double z) const
{
  // Simple example of one way to change a number 
  // into a color.
  double s = m_z_range.NormalizedParameterAt(z);
  if (s < 0.0) s = 0.0; else if (s > 1.0) s = 1.0;
  double hue = m_hue_range.ParameterAt(s);
  ON_Color c;
  c.SetHSV(hue, 1.0, 1.0);
  return c;
}

void CZAnalysisVAM::UpdateVertexColors(const CRhinoObject* object, ON_SimpleArray<const ON_Mesh *>& meshes) const
{
  UNREFERENCED_PARAMETER(object);

  // Rhino calls this function when it is time for you
  // to set the false colors on the analysis mesh vertices.
  // For breps, there is one mesh per face.  For mesh objects,
  // there is a single mesh.
  const int count = meshes.Count();
  if (count > 0)
  {
    // A "mapping tag" is used to determine if the colors
    // need to be set.
    ON_MappingTag mt = MappingTag();

    const ON_Mesh * const * mesh_list = meshes.Array();
    for (int mi = 0; mi < count; mi++)
    {
      const ON_Mesh* mesh = mesh_list[mi];
      if (mesh && mt.Compare(mesh->m_Ctag))
      {
        // The mesh's mapping tag is different from ours. Either
        // the mesh has no false colors, has false colors set by
        // another analysis mode, has false colors set using
        // different m_z_range[]/m_hue_range[] values, or the
        // mesh has been moved.  In any case, we need to set
        // the false colors to the ones we want.

        const int vcount = mesh->m_V.Count();
        ON_SimpleArray<ON_Color>& vertex_colors = const_cast<ON_Mesh*>(mesh)->m_C;
        vertex_colors.SetCount(0);     // in case something else had set the colors
        vertex_colors.Reserve(vcount); // for efficiency
        for (int vi = 0; vi < vcount; vi++)
        {
          double z = mesh->m_V[vi].z;
          ON_Color c = FalseColor(z);
          vertex_colors.Append(c);
        }

        // set the mesh's color tag 
        const_cast<ON_Mesh*>(mesh)->m_Ctag = mt;
      }
    }
  }
}

bool CZAnalysisVAM::ShowIsoCurves() const
{
  // Most shaded analysis modes that work on breps have
  // the option of showing or hiding isocurves.  Run the
  // built-in Rhino ZebraAnalysis to see how Rhino handles
  // the user interface.  If controlling iso-curve visability 
  // is a feature you want to support, then provide user
  // interface to set this member variable.
  return m_bShowIsoCurves;
}

//
// END Z analysis mode class
// 
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAnalysisMode command
//

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN ZAnalysisOn command
//

class CCommandZAnalysisOn : public CRhinoCommand
{
public:
  CCommandZAnalysisOn() {}
  ~CCommandZAnalysisOn() {}
  UUID CommandUUID()
  {
    // {3557762E-2FA4-4D4C-B867-10D652C50DFE}
    static const GUID ZAnalysisOnCommand_UUID =
    { 0x3557762E, 0x2FA4, 0x4D4C, { 0xB8, 0x67, 0x10, 0xD6, 0x52, 0xC5, 0x0D, 0xFE } };
    return ZAnalysisOnCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"ZAnalysisOn"; }
  const wchar_t* LocalCommandName() { return L"ZAnalysisOn"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandZAnalysisOn object
static class CCommandZAnalysisOn theZAnalysisOnCommand;

CRhinoCommand::result CCommandZAnalysisOn::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects for Z analysis.");
  go.SetGeometryFilter(ON::brep_object | ON::mesh_object);
  go.GetObjects(1, 0);
  if (CRhinoCommand::success != go.CommandResult())
    return go.CommandResult();

  ON_MeshParameters mp = context.m_doc.Properties().MeshParameters(ON_MeshParameters::MESH_STYLE::render_mesh_quality);
  context.m_doc.Properties().SetAnalysisMeshSettings(mp);

  int count = 0;
  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObject* rhino_object = go.Object(i).Object();
    if (0 == rhino_object)
      continue;

    if (rhino_object->InAnalysisMode(theZAnalysisVAM.m_am_id))
      continue;

    const_cast<CRhinoObject*>(rhino_object)->DestroyMeshes(ON::analysis_mesh, true);

    if (rhino_object->EnableAnalysisMode(theZAnalysisVAM.m_am_id, true))
      count++;
  }

  RhinoApp().Print(L"%d objects were put into Z analysis mode", count);
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END ZAnalysisOn command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN ZAnalysisOff command
//

class CCommandZAnalysisOff : public CRhinoCommand
{
public:
  CCommandZAnalysisOff() {}
  ~CCommandZAnalysisOff() {}
  UUID CommandUUID()
  {
    // {C8D86634-F36F-412C-B09C-8B279F4165FC}
    static const GUID ZAnalysisOffCommand_UUID =
    { 0xC8D86634, 0xF36F, 0x412C, { 0xB0, 0x9C, 0x8B, 0x27, 0x9F, 0x41, 0x65, 0xFC } };
    return ZAnalysisOffCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"ZAnalysisOff"; }
  const wchar_t* LocalCommandName() { return L"ZAnalysisOff"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandZAnalysisOff object
static class CCommandZAnalysisOff theZAnalysisOffCommand;

CRhinoCommand::result CCommandZAnalysisOff::RunCommand(const CRhinoCommandContext& context)
{
  // This is an easy way to turn off all objects that might be
  // in Z analysis mode.

  CRhinoObjectIterator it(CRhinoObjectIterator::undeleted_objects,CRhinoObjectIterator::active_and_reference_objects);

  for (CRhinoObject* rhino_object = it.First(); rhino_object; rhino_object = it.Next())
  {
    rhino_object->EnableAnalysisMode(theZAnalysisVAM.m_am_id, false);
  }

  RhinoApp().Print(L"Z analysis is off.\n");

  // update the display
  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END ZAnalysisOff command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
