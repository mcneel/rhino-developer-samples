#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectPlotWeight command
//

#pragma region SampleObjectPlotWeight command

class CCommandSampleObjectPlotWeight : public CRhinoCommand
{
public:
  CCommandSampleObjectPlotWeight() = default;
  UUID CommandUUID() override
  {
    // {67200D98-4FB6-4588-ABA4-BE525C4430D6}
    static const GUID SampleObjectPlotWeightCommand_UUID =
    { 0x67200D98, 0x4FB6, 0x4588,{ 0xAB, 0xA4, 0xBE, 0x52, 0x5C, 0x44, 0x30, 0xD6 } };
    return SampleObjectPlotWeightCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleObjectPlotWeight"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleObjectPlotWeight object
static class CCommandSampleObjectPlotWeight theSampleObjectPlotWeightCommand;

CRhinoCommand::result CCommandSampleObjectPlotWeight::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects");
  go.EnableSubObjectSelect(FALSE);
  go.EnableReferenceObjectSelect(FALSE);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const int object_count = go.ObjectCount();
  for (int i = 0; i < object_count; i++)
  {
    const CRhinoObjRef& obj_ref = go.Object(i);
    const CRhinoObject* obj = obj_ref.Object();
    if (nullptr != obj)
    { 
      CRhinoObjectAttributes attrib(obj->Attributes());
      attrib.m_plot_weight_mm = 0.25; // Some weight in mm
      attrib.SetPlotWeightSource(ON::plot_weight_from_object);
      context.m_doc.ModifyObjectAttributes(obj_ref, attrib);
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleObjectPlotWeight command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
