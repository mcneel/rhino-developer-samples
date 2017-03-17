#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDupBorder command
//

#pragma region SampleDupBorder command

class CCommandSampleDupBorder : public CRhinoCommand
{
public:
  CCommandSampleDupBorder() {}
  ~CCommandSampleDupBorder() {}
  UUID CommandUUID()
  {
    // {1338EF5F-B751-4DAF-95AD-4395A7A1A2F8}
    static const GUID SampleDupBorderCommand_UUID =
    { 0x1338EF5F, 0xB751, 0x4DAF, { 0x95, 0xAD, 0x43, 0x95, 0xA7, 0xA1, 0xA2, 0xF8 } };
    return SampleDupBorderCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDupBorder"; }
  const wchar_t* LocalCommandName() const { return L"SampleDupBorder"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDupBorder obj
static class CCommandSampleDupBorder theSampleDupBorderCommand;

CRhinoCommand::result CCommandSampleDupBorder::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select surface or polysurface");
  go.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go.AcceptNothing();
  go.GetObjects(1, 1);
  if (go.CommandResult() != success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  const CRhinoObject* obj = obj_ref.Object();
  const ON_Brep* brep = obj_ref.Brep();
  if (nullptr == obj || nullptr == brep)
    return CRhinoCommand::failure;

  obj->Select(false);

  ON_SimpleArray<const ON_Curve*> curve_array(brep->m_E.Count());

  for (int i = 0; i < brep->m_E.Count(); i++)
  {
    const ON_BrepEdge& edge = brep->m_E[i];

    // Find only the naked edges 
    if (edge.m_ti.Count() == 1 && edge.m_c3i >= 0)
    {
      ON_Curve* curve = edge.DuplicateCurve();

      // Make the curve direction go in the natural
      // boundary loop direction so that the curve
      // directions come out consistantly
      if (brep->m_T[edge.m_ti[0]].m_bRev3d)
        curve->Reverse();
      if (brep->m_T[edge.m_ti[0]].Face()->m_bRev)
        curve->Reverse();

      curve_array.Append(curve);
    }
  }

  double tol = 2.1 * context.m_doc.AbsoluteTolerance();
  ON_SimpleArray<ON_Curve*> output_array;

  // Join the curves
  if (RhinoMergeCurves(curve_array, output_array, tol))
  {
    for (int i = 0; i < output_array.Count(); i++)
    {
      CRhinoCurveObject* curve_object = new CRhinoCurveObject;
      curve_object->SetCurve(output_array[i]);
      if (context.m_doc.AddObject(curve_object))
        curve_object->Select();
      else
        delete curve_object;
    }
  }

  // Don't leak memory
  for (int i = 0; i < curve_array.Count(); i++)
    delete curve_array[i];

  context.m_doc.Redraw();
  return success;
}

#pragma endregion

//
// END SampleDupBorder command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
