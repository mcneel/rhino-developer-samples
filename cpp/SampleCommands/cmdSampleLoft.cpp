#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleLoft command
//

class CCommandSampleLoft : public CRhinoCommand
{
public:
  CCommandSampleLoft() {}
  ~CCommandSampleLoft() {}
  UUID CommandUUID()
  {
    // {EB83F47-D94A-4F5B-95F6-743C4C1D830B}
    static const GUID SampleLoftCommand_UUID =
    { 0xEB83F47, 0xD94A, 0x4F5B, { 0x95, 0xF6, 0x74, 0x3C, 0x4C, 0x1D, 0x83, 0x0B } };
    return SampleLoftCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleLoft"; }
  const wchar_t* LocalCommandName() { return L"SampleLoft"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleLoft object
static class CCommandSampleLoft theSampleLoftCommand;

CRhinoCommand::result CCommandSampleLoft::RunCommand(const CRhinoCommandContext& context)
{
  // Select curves to loft
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curves to loft");
  go.SetGeometryFilter(CRhinoGetObject::curve_object | CRhinoGetObject::edge_object);
  go.SetGeometryAttributeFilter(CRhinoGetObject::open_curve);
  go.EnablePreSelect(false);
  go.GetObjects(2, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  // Create loft arguments object
  const int obj_count = go.ObjectCount();
  CArgsRhinoLoft args;
  args.m_loftcurves.SetCapacity(obj_count);

  // Add curves to loft arguments object
  int i;
  for (i = 0; i < obj_count; i++)
  {
    const CRhinoObjRef& ref = go.Object(i);
    const ON_Curve* crv = ref.Curve();
    if (crv)
    {
      // New loft curve
      CRhinoLoftCurve* lc = new CRhinoLoftCurve;

      // Duplicate the selected curve. Note,
      // the loft curve will delete this curve.
      lc->m_curve = crv->DuplicateCurve();
      lc->m_curve->RemoveShortSegments(ON_ZERO_TOLERANCE);

      // Set other loft curve parameters
      lc->m_bPlanar = (lc->m_curve->IsPlanar(&lc->m_plane) ? true : false);

      // If referenced geometry is a surface edge,
      // assign associated brep trim.
      lc->m_trim = ref.Trim();

      // Append loft curve to loft argument
      args.m_loftcurves.Append(lc);
    }
  }

  // If we do not have enough loft curves,
  // clean up and bail.
  const int lc_count = args.m_loftcurves.Count();
  if (lc_count < 2)
  {
    for (i = 0; i < lc_count; i++)
      delete args.m_loftcurves[i];
    return CRhinoCommand::failure;
  }

  // If the starting loft curve has a trim, 
  // set the start condition to tangent.
  if (args.m_loftcurves[0] && args.m_loftcurves[0]->m_trim)
  {
    args.m_start_condition = CArgsRhinoLoft::leTangent;
    args.m_bAllowStartTangent = TRUE;
  }

  // If the ending loft curve has a trim, 
  // set the end condition to tangent.
  if (args.m_loftcurves[lc_count - 1] && args.m_loftcurves[lc_count - 1]->m_trim)
  {
    args.m_end_condition = CArgsRhinoLoft::leTangent;
    args.m_bAllowEndTangent = TRUE;
  }

  // Do the loft calculation
  ON_SimpleArray<ON_NurbsSurface*> srf_list;
  bool rc = RhinoSdkLoftSurface(args, srf_list);

  // Delete the loft curves so we do not leak memory.
  for (i = 0; i < args.m_loftcurves.Count(); i++)
    delete args.m_loftcurves[i];
  args.m_loftcurves.Empty();

  // If the loft operation failed, bail.
  if (!rc)
    return CRhinoCommand::failure;

  // If only one surface was calculated, add it to Rhino
  if (srf_list.Count() == 1)
  {
    context.m_doc.AddSurfaceObject(*srf_list[0]);

    // CRhinoDoc::AddSurfaceObject() make a copy.
    // So, delete original so memory is not leaked.
    delete srf_list[0];
  }
  else
  {
    // If more than one surface was calculated, 
    // create a list of breps.
    ON_SimpleArray<ON_Brep*> brep_list;
    for (i = 0; i < srf_list.Count(); i++)
    {
      if (srf_list[i]->IsValid())
      {
        ON_Brep* brep = ON_Brep::New();
        brep->NewFace(*srf_list[i]);

        // ON_Brep::NewFace() make a copy.
        // So, delete original so memory is not leaked.
        delete srf_list[i];

        brep_list.Append(brep);
      }
    }

    // Try joining all breps
    double tol = context.m_doc.AbsoluteTolerance();
    ON_Brep* brep = RhinoJoinBreps(brep_list, tol);
    if (brep)
    {
      context.m_doc.AddBrepObject(*brep);

      // CRhinoDoc::AddBrepObject() make a copy.
      // So, delete original so memory is not leaked.
      delete brep;
    }
    else
    {
      // Othewise just add the individual breps to Rhino.
      for (i = 0; i < brep_list.Count(); i++)
      {
        if (brep_list[i])
        {
          context.m_doc.AddBrepObject(*brep_list[i]);

          // CRhinoDoc::AddBrepObject() make a copy.
          // So, delete original so memory is not leaked.
          delete brep_list[i];
        }
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleLoft command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
