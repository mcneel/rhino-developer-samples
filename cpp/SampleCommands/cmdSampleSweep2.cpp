#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSweep2 command
//

#pragma region SampleSweep2 command

class CCommandSampleSweep2 : public CRhinoCommand
{
public:
  CCommandSampleSweep2() = default;
  UUID CommandUUID() override
  {
    // {68BC5D74-91AE-40DE-81BF-50E51A5FA175}
    static const GUID SampleSweep2Command_UUID =
    { 0x68BC5D74, 0x91AE, 0x40DE,{ 0x81, 0xBF, 0x50, 0xE5, 0x1A, 0x5F, 0xA1, 0x75 } };
    return SampleSweep2Command_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSweep2"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSweep2 object
static class CCommandSampleSweep2 theSampleSweep2Command;

CRhinoCommand::result CCommandSampleSweep2::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  // Select first rail curve
  CRhinoGetObject go0;
  go0.SetCommandPrompt(L"Select first rail");
  go0.SetGeometryFilter(CRhinoGetObject::curve_object);
  go0.EnablePreSelect(false);
  go0.GetObjects(1, 1);
  if (go0.CommandResult() != CRhinoCommand::success)
    return go0.CommandResult();

  const ON_Curve* rail0 = go0.Object(0).Curve();
  if (nullptr == rail0)
    return CRhinoCommand::failure;

  // Select second rail curve
  CRhinoGetObject go1;
  go1.SetCommandPrompt(L"Select second rail");
  go1.SetGeometryFilter(CRhinoGetObject::curve_object);
  go1.EnablePreSelect(false);
  go1.EnableDeselectAllBeforePostSelect(false);
  go1.GetObjects(1, 1);
  if (go1.CommandResult() != CRhinoCommand::success)
    return go1.CommandResult();

  const ON_Curve* rail1 = go1.Object(0).Curve();
  if (nullptr == rail1)
    return CRhinoCommand::failure;

  // Select cross section curve
  CRhinoGetObject gc;
  gc.SetCommandPrompt(L"Select cross section");
  gc.SetGeometryFilter(CRhinoGetObject::curve_object);
  gc.EnablePreSelect(false);
  gc.EnableDeselectAllBeforePostSelect(false);
  gc.GetObjects(1, 1);
  if (gc.CommandResult() != CRhinoCommand::success)
    return gc.CommandResult();

  const ON_Curve* crv = gc.Object(0).Curve();
  if (nullptr == crv)
    return CRhinoCommand::failure;

  const ON_Curve* rails[] = { rail0, rail1 };

  ON_SimpleArray<const ON_Curve*> shapes;
  shapes.Append(crv);
  
  bool bClosed = rail0->IsClosed() && rail1->IsClosed();

  // Make a 2-rail sweep
  ON_SimpleArray<ON_Brep*> output_breps;
  bool rc = Rhino2RailSweep(output_breps, rails, shapes, bClosed, doc->AbsoluteTolerance());
  if (rc)
  {
    for (int i = 0; i < output_breps.Count(); i++)
    {
      ON_Brep* brep = output_breps[i];
      if (nullptr != brep)
      {
        CRhinoBrepObject* brep_obj = new CRhinoBrepObject();
        brep_obj->SetBrep(brep);
        brep = nullptr;
        doc->AddObject(brep_obj);
      }
    }
    doc->Redraw();
  }


  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSweep2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
