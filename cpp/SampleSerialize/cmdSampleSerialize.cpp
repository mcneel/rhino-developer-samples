/////////////////////////////////////////////////////////////////////////////
// cmdSampleSerialize.cpp
//

#include "StdAfx.h"
#include "SampleSerializePlugIn.h"

class CCommandSampleSerialize : public CRhinoCommand
{
public:
  CCommandSampleSerialize() {}
  ~CCommandSampleSerialize() {}
  UUID CommandUUID()
  {
    // {FB954414-900E-434C-B584-BB0FD252F2F0}
    static const GUID SampleSerializeCommand_UUID =
    { 0xFB954414, 0x900E, 0x434C,{ 0xB5, 0x84, 0xBB, 0xF, 0xD2, 0x52, 0xF2, 0xF0 } };
    return SampleSerializeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSerialize"; }
  const wchar_t* LocalCommandName() const { return L"SampleSerialize"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};
static class CCommandSampleSerialize theSampleSerializeCommand;

CRhinoCommand::result CCommandSampleSerialize::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curves to serialize");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.GetObjects(1, 0);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  CSampleSerializeData& data = SampleSerializePlugIn().Data();

  const int object_count = go.ObjectCount();
  ON_SimpleArray<ON_Curve*> duplicates;

  for (int i = 0; i < object_count; i++)
  {
    const ON_Curve* curve = go.Object(i).Curve();
    if (0 == curve)
      return CRhinoCommand::failure;

    ON_Curve* duplicate = curve->Duplicate();
    if (0 != duplicate)
    {
      if (0 == i)
        data.SetCurve(duplicate);
      else
        duplicates.Append(duplicate);
    }
  }

  if (0 != duplicates.Count())
    data.SetCurves(duplicates);

  return CRhinoCommand::success;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleUnserialize command
//

#pragma region SampleUnserialize command

class CCommandSampleUnserialize : public CRhinoCommand
{
public:
  CCommandSampleUnserialize() {}
  ~CCommandSampleUnserialize() {}
  UUID CommandUUID()
  {
    // {A5836FF-CFAA-43BC-93AB-CEDD8E25BF31}
    static const GUID SampleUnserializeCommand_UUID =
    { 0xA5836FF, 0xCFAA, 0x43BC,{ 0x93, 0xAB, 0xCE, 0xDD, 0x8E, 0x25, 0xBF, 0x31 } };
    return SampleUnserializeCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleUnserialize"; }
  const wchar_t* LocalCommandName() const { return L"SampleUnserialize"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleUnserialize object
static class CCommandSampleUnserialize theSampleUnserializeCommand;

CRhinoCommand::result CCommandSampleUnserialize::RunCommand(const CRhinoCommandContext& context)
{
  CSampleSerializeData& data = SampleSerializePlugIn().Data();

  const ON_Curve* curve = data.Curve();
  if (0 != curve)
  {
    ON_Curve* duplicate = curve->Duplicate();
    if (0 != duplicate)
    {
      CRhinoCurveObject* curve_object = new CRhinoCurveObject();
      curve_object->SetCurve(duplicate);
      context.m_doc.AddObject(curve_object);
    }
  }

  ON_SimpleArray<const ON_Curve*> curves;
  const int curve_count = data.Curves(curves);
  for (int i = 0; i < curve_count; i++)
  {
    curve = curves[i];
    if (0 != curve)
    {
      ON_Curve* duplicate = curve->Duplicate();
      if (0 != duplicate)
      {
        CRhinoCurveObject* curve_object = new CRhinoCurveObject();
        curve_object->SetCurve(duplicate);
        context.m_doc.AddObject(curve_object);
      }
    }
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleUnserialize command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
