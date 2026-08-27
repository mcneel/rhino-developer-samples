#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleFilletEdges command
//

#pragma region SampleFilletEdges command

class CCommandSampleFilletEdges : public CRhinoCommand
{
public:
  CCommandSampleFilletEdges() = default;
  ~CCommandSampleFilletEdges() = default;
  UUID CommandUUID() override
  {
    // {3AB91177-74C3-4172-BE82-2532D637576A}
    static const GUID SampleFilletEdgesCommand_UUID =
    { 0x3AB91177, 0x74C3, 0x4172, { 0xBE, 0x82, 0x25, 0x32, 0xD6, 0x37, 0x57, 0x6A } };
    return SampleFilletEdgesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleFilletEdges"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  void AddFilletOptions(CRhinoGet& get, int& blend_type_option, int& rail_type_option);
  void ProcessFilletOption(const CRhinoGet& get, CRhinoGet::result res, int blend_type_option, int rail_type_option);
  bool FilletBrepEdges(CRhinoDoc& doc, const CRhinoObject* obj, const ON_Brep& brep, const ON_SimpleArray<int>& edges);

private:
  double m_radius = 1.0;
  int m_blend_type = static_cast<int>(CRhinoFilletEdge::BlendType::Fillet);
  int m_rail_type = static_cast<int>(CRhinoFilletEdge::RailType::RollingBall);
  bool m_setback = false;
};

// The one and only CCommandSampleFilletEdges object
static class CCommandSampleFilletEdges theSampleFilletEdgesCommand;

CRhinoCommand::result CCommandSampleFilletEdges::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  // Select the edges to fillet, chamfer, or blend
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select edges to fillet");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::edge_curve);
  go.AcceptNumber(true, false);
  for (;;)
  {
    int blend_type_option = 0, rail_type_option = 0;
    AddFilletOptions(go, blend_type_option, rail_type_option);

    const CRhinoGet::result res = go.GetObjects(1, 0);
    if (CRhinoGet::number == res || CRhinoGet::option == res)
    {
      ProcessFilletOption(go, res, blend_type_option, rail_type_option);
      continue;
    }

    break;
  }

  if (CRhinoCommand::success != go.CommandResult())
    return go.CommandResult();

  // When the edges were pre-selected, the command line options above never
  // appeared. So, give the user a chance to modify them here.
  if (go.ObjectsWerePreSelected())
  {
    CRhinoGetOption gs;
    gs.SetCommandPrompt(L"Fillet edge options. Press Enter when done");
    gs.AcceptNothing();
    gs.AcceptNumber(true, false);
    for (;;)
    {
      int blend_type_option = 0, rail_type_option = 0;
      AddFilletOptions(gs, blend_type_option, rail_type_option);

      const CRhinoGet::result res = gs.GetOption();
      if (CRhinoGet::number == res || CRhinoGet::option == res)
      {
        ProcessFilletOption(gs, res, blend_type_option, rail_type_option);
        continue;
      }

      if (CRhinoGet::nothing == res)
        break;

      return gs.CommandResult();
    }
  }

  // CRhinoFilletEdge::FilletEdges() works on a single Brep at a time.
  // So, group the selected edge indices by their parent object.
  ON_SimpleArray<const CRhinoObject*> objects;
  ON_SimpleArray<const ON_Brep*> breps;
  ON_ClassArray<ON_SimpleArray<int>> object_edges;

  for (int i = 0; i < go.ObjectCount(); i++)
  {
    const CRhinoObjRef& obj_ref = go.Object(i);

    const ON_BrepEdge* edge = obj_ref.Edge();
    if (nullptr == edge)
      continue;

    // Note, when the selected object is an extrusion object, this is the
    // Brep form of the extrusion, which is what the edge index refers to.
    const ON_Brep* brep = edge->Brep();
    if (nullptr == brep)
      continue;

    const CRhinoObject* obj = obj_ref.Object();
    if (nullptr == obj)
      continue;

    // Note, the geometry of an instance reference cannot be modified in place
    if (nullptr != CRhinoInstanceObject::Cast(obj))
    {
      RhinoApp().Print(L"The edges of a block instance cannot be filleted.\n");
      continue;
    }

    int index = -1;
    for (int j = 0; j < objects.Count() && index < 0; j++)
    {
      if (objects[j] == obj)
        index = j;
    }

    if (index < 0)
    {
      objects.Append(obj);
      breps.Append(brep);
      object_edges.AppendNew();
      index = objects.Count() - 1;
    }

    // Don't add the same edge twice
    if (object_edges[index].Search(edge->m_edge_index) < 0)
      object_edges[index].Append(edge->m_edge_index);
  }

  int success_count = 0;
  for (int i = 0; i < objects.Count(); i++)
  {
    if (FilletBrepEdges(*doc, objects[i], *breps[i], object_edges[i]))
      success_count++;
  }

  if (0 == success_count)
  {
    RhinoApp().Print(L"Unable to fillet the selected edges.\n");
    return CRhinoCommand::failure;
  }

  doc->Redraw();

  return CRhinoCommand::success;
}

/// <summary>
/// Adds the fillet, chamfer, and blend command line options to a getter.
/// </summary>
/// <param name="get">The getter to add the options to.</param>
/// <param name="blend_type_option">The index of the BlendType option.</param>
/// <param name="rail_type_option">The index of the RailType option.</param>
void CCommandSampleFilletEdges::AddFilletOptions(CRhinoGet& get, int& blend_type_option, int& rail_type_option)
{
  // The list order matches the values of CRhinoFilletEdge::BlendType
  static const CRhinoCommandOptionValue blend_types[3] =
  {
    RHCMDOPTVALUE(L"Chamfer"),
    RHCMDOPTVALUE(L"Fillet"),
    RHCMDOPTVALUE(L"Blend")
  };

  // The list order matches the values of CRhinoFilletEdge::RailType
  static const CRhinoCommandOptionValue rail_types[3] =
  {
    RHCMDOPTVALUE(L"DistanceFromEdge"),
    RHCMDOPTVALUE(L"RollingBall"),
    RHCMDOPTVALUE(L"DistanceBetweenRails")
  };

  get.ClearCommandOptions();
  get.AddCommandOptionNumber(RHCMDOPTNAME(L"Radius"), &m_radius, nullptr, FALSE, ON_ZERO_TOLERANCE, ON_UNSET_VALUE);
  blend_type_option = get.AddCommandOptionList(RHCMDOPTNAME(L"BlendType"), 3, blend_types, m_blend_type);
  rail_type_option = get.AddCommandOptionList(RHCMDOPTNAME(L"RailType"), 3, rail_types, m_rail_type);
  get.AddCommandOptionToggle(RHCMDOPTNAME(L"Setback"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), m_setback, &m_setback);
}

/// <summary>
/// Processes the option, or the number, the user just picked or typed.
/// </summary>
/// <param name="get">The getter that returned the option or the number.</param>
/// <param name="res">The result returned by the getter.</param>
/// <param name="blend_type_option">The index of the BlendType option.</param>
/// <param name="rail_type_option">The index of the RailType option.</param>
void CCommandSampleFilletEdges::ProcessFilletOption(const CRhinoGet& get, CRhinoGet::result res, int blend_type_option, int rail_type_option)
{
  if (CRhinoGet::number == res)
  {
    if (get.Number() > ON_ZERO_TOLERANCE)
      m_radius = get.Number();
    return;
  }

  const CRhinoCommandOption* option = get.Option();
  if (nullptr == option)
    return;

  // Note, the Radius and Setback options maintain their values on their own
  if (blend_type_option == option->m_option_index)
    m_blend_type = option->m_list_option_current;
  else if (rail_type_option == option->m_option_index)
    m_rail_type = option->m_list_option_current;
}

/// <summary>
/// Fillets, chamfers, or blends some edges of a Brep and replaces the
/// object that owns the Brep with the results of the calculation.
/// </summary>
/// <param name="doc">The document that contains the object.</param>
/// <param name="obj">The object to replace.</param>
/// <param name="brep">The Brep that owns the edges.</param>
/// <param name="edges">The indices of the edges to fillet, chamfer, or blend.</param>
/// <returns>true if the object was replaced, false otherwise.</returns>
bool CCommandSampleFilletEdges::FilletBrepEdges(CRhinoDoc& doc, const CRhinoObject* obj, const ON_Brep& brep, const ON_SimpleArray<int>& edges)
{
  if (nullptr == obj || 0 == edges.Count())
    return false;

  // One starting and one ending radius, or distance, per edge.
  // Note, when the starting and ending values differ, the radius,
  // or distance, varies linearly along the edge.
  ON_SimpleArray<double> radii0(edges.Count());
  ON_SimpleArray<double> radii1(edges.Count());
  for (int i = 0; i < edges.Count(); i++)
  {
    radii0.Append(m_radius);
    radii1.Append(m_radius);
  }

  const double tolerance = doc.AbsoluteTolerance();
  const double angle_tolerance = doc.AngleToleranceRadians();

  // CRITICAL: memory for the resulting Breps is allocated.
  // It is the responsibility of the caller to clean it up.
  ON_SimpleArray<ON_Brep*> out_breps;

  CWaitCursor wait;
  const bool rc = CRhinoFilletEdge::FilletEdges(
    brep,
    edges,
    radii0,
    radii1,
    static_cast<CRhinoFilletEdge::BlendType>(m_blend_type),
    static_cast<CRhinoFilletEdge::RailType>(m_rail_type),
    m_setback,
    tolerance,
    angle_tolerance,
    out_breps
  );
  wait.Restore();

  bool replaced = false;

  if (rc)
  {
    // Copy the attributes now, as obj becomes invalid once it is replaced
    const CRhinoObjectAttributes attributes(obj->Attributes());

    // Replace the original object with the first result and
    // add any additional results to the document.
    for (int i = 0; i < out_breps.Count(); i++)
    {
      if (nullptr == out_breps[i])
        continue;

      CRhinoBrepObject* new_obj = new CRhinoBrepObject(attributes);
      new_obj->SetBrep(out_breps[i]);
      out_breps[i] = nullptr; // the new object owns the Brep now

      if (replaced)
      {
        if (!doc.AddObject(new_obj))
          delete new_obj; // don't leak...
      }
      else
      {
        if (doc.ReplaceObject(CRhinoObjRef(obj), new_obj))
          replaced = true;
        else
          delete new_obj; // don't leak...
      }
    }
  }

  // Clean up anything we did not take ownership of
  for (int i = 0; i < out_breps.Count(); i++)
    delete out_breps[i];

  return replaced;
}

#pragma endregion

//
// END SampleFilletEdges command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
