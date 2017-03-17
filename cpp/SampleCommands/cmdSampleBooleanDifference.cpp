#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleBooleanDifference command
//

class CCommandSampleBooleanDifference : public CRhinoCommand
{
public:
  CCommandSampleBooleanDifference() {}
  ~CCommandSampleBooleanDifference() {}
  UUID CommandUUID()
  {
    // {D3F06EEC-FE99-4591-88FB-A2E1A8BC6EF4}
    static const GUID SampleBooleanDifferenceCommand_UUID =
    { 0xD3F06EEC, 0xFE99, 0x4591, { 0x88, 0xFB, 0xA2, 0xE1, 0xA8, 0xBC, 0x6E, 0xF4 } };
    return SampleBooleanDifferenceCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleBooleanDifference"; }
  const wchar_t* LocalCommandName() const { return L"SampleBooleanDifference"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleBooleanDifference object
static class CCommandSampleBooleanDifference theSampleBooleanDifferenceCommand;

CRhinoCommand::result CCommandSampleBooleanDifference::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go0;
  go0.SetCommandPrompt(L"Select surfaces or polysurfaces to subtract from");
  go0.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go0.EnableSubObjectSelect(FALSE);
  go0.GetObjects(1, 0);
  if (go0.CommandResult() != CRhinoCommand::success)
    return go0.CommandResult();

  const int InBreps0_count = go0.ObjectCount();
  ON_SimpleArray<const CRhinoObject*> InBreps0_objects(InBreps0_count);
  ON_SimpleArray<const ON_Brep*> InBreps0(InBreps0_count);

  for (int i = 0; i < InBreps0_count; i++)
  {
    const CRhinoObject* obj = go0.Object(i).Object();
    const ON_Brep* brep = go0.Object(i).Brep();
    if (0 == obj || 0 == brep)
      return CRhinoCommand::failure;

    InBreps0_objects.Append(obj);
    InBreps0.Append(brep);
  }

  CRhinoGetObject go1;
  go1.SetCommandPrompt(L"Select surfaces or polysurfaces to subtract with");
  go1.SetGeometryFilter(CRhinoGetObject::surface_object | CRhinoGetObject::polysrf_object);
  go1.EnableSubObjectSelect(FALSE);
  go1.EnablePreSelect(FALSE, TRUE);
  go1.EnableDeselectAllBeforePostSelect(false);
  go1.GetObjects(1, 0);
  if (go1.CommandResult() != CRhinoCommand::success)
    return go1.CommandResult();

  const int InBreps1_count = go1.ObjectCount();
  ON_SimpleArray<const CRhinoObject*> InBreps1_objects(InBreps1_count);
  ON_SimpleArray<const ON_Brep*> InBreps1(InBreps1_count);

  for (int i = 0; i < InBreps1_count; i++)
  {
    const CRhinoObject* obj = go1.Object(i).Object();
    const ON_Brep* brep = go1.Object(i).Brep();
    if (0 == obj || 0 == brep)
      return CRhinoCommand::failure;

    InBreps1_objects.Append(obj);
    InBreps1.Append(brep);
  }

  double tol = context.m_doc.AbsoluteTolerance();
  bool something_happened = false;
  ON_SimpleArray<ON_Brep*> OutBreps;
  ON_SimpleArray<int> InputIndexForOutput;

  CWaitCursor wait;
  bool rc = RhinoBooleanDifference(InBreps0, InBreps1, tol, &something_happened, OutBreps, InputIndexForOutput);
  wait.Restore();

  if (!rc || !something_happened)
  {
    for (int i = 0; i < OutBreps.Count(); i++)
      delete OutBreps[i];

    RhinoApp().Print(L"Boolean difference failed.\n");
    return CRhinoCommand::failure;
  }

  for (int i = 0; i < OutBreps.Count(); i++)
  {
    const ON_3dmObjectAttributes& attribs = InBreps0_objects[InputIndexForOutput[i]]->Attributes();

    CRhinoBrepObject* brep_obj = new CRhinoBrepObject(attribs);
    brep_obj->SetBrep(OutBreps[i]);
    OutBreps[i] = 0;

    context.m_doc.AddObject(brep_obj);

  }

  if (true)
  {
    for (int i = 0; i < InBreps0_count; i++)
      context.m_doc.DeleteObject(go0.Object(i));

    for (int i = 0; i < InBreps1_count; i++)
      context.m_doc.DeleteObject(go1.Object(i));
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

//
// END SampleBooleanDifference command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
