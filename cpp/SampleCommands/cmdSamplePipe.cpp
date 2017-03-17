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
  CCommandSamplePipe() { m_radius = 1.0; }
  ~CCommandSamplePipe() {}
  UUID CommandUUID()
  {
    // {3A97346-F02D-4783-A55C-7D69BD8029DC}
    static const GUID SamplePipeCommand_UUID =
    { 0x3A97346, 0xF02D, 0x4783, { 0xA5, 0x5C, 0x7D, 0x69, 0xBD, 0x80, 0x29, 0xDC } };
    return SamplePipeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SamplePipe"; }
  const wchar_t* LocalCommandName() const { return L"SamplePipe"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  double m_radius;
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
  if (0 == curve)
    return CRhinoCommand::failure;

  CRhinoGetNumber gn;
  gn.SetCommandPrompt(L"Radius of pipe");
  gn.SetDefaultNumber(m_radius);
  gn.SetLowerLimit(0.0, TRUE);
  gn.GetNumber();
  if (gn.CommandResult() != CRhinoCommand::success)
    return gn.CommandResult();

  m_radius = fabs(gn.Number());

  ON_Interval dom = curve->Domain();

  ON_SimpleArray<double> curve_params;
  curve_params.Append(dom.NormalizedParameterAt(dom.Min()));
  curve_params.Append(dom.NormalizedParameterAt(dom.Max()));

  ON_SimpleArray<double> radii;
  radii.Append(m_radius); // radius at dom.Min()
  radii.Append(m_radius); // radius at dom.Max()

  int blend_type = 0;    // local blend
  int cap_mode = 1;      // flat cap
  bool fit_rail = false; // don't fit
  bool bUseExtrusions = true; // Create extrusion objects when possible

  ON_SimpleArray<ON_Object*> results;
  if (RhinoPipe(*curve, curve_params, radii, results, blend_type, cap_mode, fit_rail))
  {
    for (int i = 0; i < results.Count(); i++)
    {
      if (0 != results[i])
      {
        if (results[i]->ObjectType() == ON::extrusion_object)
        {
          if (bUseExtrusions)
          {
            CRhinoExtrusionObject* object = new CRhinoExtrusionObject();
            object->SetExtrusion(ON_Extrusion::Cast(results[i]));
            results[i] = 0; // object now owns the memory
            if (!context.m_doc.AddObject(object))
              delete object; // Don't leak...
          }
          else
          {
            ON_Extrusion* extrusion = ON_Extrusion::Cast(results[i]);
            if (extrusion)
            {
              ON_Brep* brep = extrusion->BrepForm(0, true);
              if (brep)
              {
                CRhinoBrepObject* object = new CRhinoBrepObject();
                object->SetBrep(brep);
                if (!context.m_doc.AddObject(object))
                  delete object; // Don't leak...
              }
            }
            delete results[i]; // Don't leak...
            results[i] = 0;
          }
        }
        else
        {
          CRhinoBrepObject* object = new CRhinoBrepObject();
          object->SetBrep(ON_Brep::Cast(results[i]));
          results[i] = 0; // object now owns the memory
          if (!context.m_doc.AddObject(object))
            delete object; // Don't leak...
        }
      }
    }

    context.m_doc.Redraw();
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SamplePipe command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
