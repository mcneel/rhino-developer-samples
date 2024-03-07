#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePipe command
//

#pragma region SamplePipe command

class CCommandSamplePipe : public CRhinoCommand
{
public:
  CCommandSamplePipe() = default;
  ~CCommandSamplePipe() = default;
  UUID CommandUUID() override
  {
    // {3A97346-F02D-4783-A55C-7D69BD8029DC}
    static const GUID SamplePipeCommand_UUID =
    { 0x3A97346, 0xF02D, 0x4783, { 0xA5, 0x5C, 0x7D, 0x69, 0xBD, 0x80, 0x29, 0xDC } };
    return SamplePipeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePipe"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;

private:
  double m_radius = 1.0;
};

// The one and only CCommandSamplePipe object
static class CCommandSamplePipe theSamplePipeCommand;

CRhinoCommand::result CCommandSamplePipe::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select path curve");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const ON_Curve* curve = go.Object(0).Curve();
  if (nullptr == curve)
    return CRhinoCommand::failure;

  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Radius of pipe");
  gn.SetDefaultNumber(m_radius);
  gn.SetLowerLimit(0.0, TRUE);
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  m_radius = fabs(gn.Number());

  ON_SimpleArray<double> curve_params(2);
  ON_Interval domain = curve->Domain();
  curve_params.Append(domain.NormalizedParameterAt(domain.Min()));
  curve_params.Append(domain.NormalizedParameterAt(domain.Max()));

  ON_SimpleArray<double> radii(2);;
  radii.Append(m_radius); // radius at domain.Min()
  radii.Append(m_radius); // radius at domain.Max()

  double abstol = context.m_doc.AbsoluteTolerance();
  double angtol = context.m_doc.AngleToleranceRadians();
  int blend_type = 0;         // local blend
  int cap_mode = 1;           // flat cap
  bool fit_rail = false;      // don't fit
  bool bUseExtrusions = true; // Create extrusion objects when possible

  ON_SimpleArray<ON_Object*> results;
  if (RhinoPipe(*curve, curve_params, radii, results, abstol, angtol, blend_type, cap_mode, fit_rail))
  {
    for (int i = 0; i < results.Count(); i++)
    {
      if (nullptr == results[i])
        continue;

      if (results[i]->ObjectType() == ON::extrusion_object)
      {
        ON_Extrusion* pExtrusion = ON_Extrusion::Cast(results[i]);
        if (pExtrusion)
        {
          if (bUseExtrusions)
          {
            CRhinoExtrusionObject* pExtrusionObject = new CRhinoExtrusionObject();
            pExtrusionObject->SetExtrusion(pExtrusion);

            results[i] = nullptr; // pExtrusionObject now owns this memory

            if (!context.m_doc.AddObject(pExtrusionObject))
              delete pExtrusionObject; // Don't leak...
          }
          else
          {
            ON_Brep* pBrep = pExtrusion->BrepForm(nullptr, true);
            if (pBrep)
            {
              delete results[i]; // Extrusion no longer needed...
              results[i] = nullptr;

              CRhinoBrepObject* pBrepObject = new CRhinoBrepObject();
              pBrepObject->SetBrep(pBrep);

              pBrep = nullptr; // pBrepObject now owns this memory

              if (!context.m_doc.AddObject(pBrepObject))
                delete pBrepObject; // Don't leak...
            }
          }
        }
      }
      else if (results[i]->ObjectType() == ON::brep_object)
      {
        ON_Brep* pBrep = ON_Brep::Cast(results[i]);
        if (pBrep)
        {
          CRhinoBrepObject* pBrepObject = new CRhinoBrepObject();
          pBrepObject->SetBrep(pBrep);

          results[i] = nullptr; // pBrepObject now owns this memory

          if (!context.m_doc.AddObject(pBrepObject))
            delete pBrepObject; // Don't leak...
        }
      }
    }

    for (int i = 0; i < results.Count(); i++)
    {
      if (results[i])
      {
        delete results[i]; // Don't leak...
        results[i] = nullptr;
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePipe command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
