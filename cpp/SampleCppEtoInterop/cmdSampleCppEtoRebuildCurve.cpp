// cmdSampleCppEtoRebuildCurve.cpp : command file
//
// This sample demonstrates how a native (C++) plug-in can show an Eto (.NET)
// user interface and exchange data with it, using the public interop SDK that
// was added for RH-92307 (see rhinoSdkInterop.h):
//
//   - CRhinoParameterDictionary : a typed, name-keyed bag of values.
//   - RhinoExecuteNamedCallback : invokes a callback that a .NET plug-in
//                                 registered with Rhino.Runtime.HostUtils.
//                                 RegisterNamedCallback.
//
// The C++ side owns the workflow and the geometry; the dialog itself lives in
// .NET. Here we select a curve, read its point count and degree, then ask the
// managed "Rebuild" Eto dialog (registered by the SampleCsEto .NET sample under
// the name "SampleCsEtoRebuildCurveDialog") to collect the user's choices.
// Finally we rebuild the curve back in native code.
//
// IMPORTANT: This command requires the companion SampleCsEto .NET plug-in to be
// loaded, since that plug-in is what registers the named callback. If it is not
// loaded, RhinoExecuteNamedCallback returns false and the command reports it.
//

#include "StdAfx.h"
#include "SampleCppEtoInteropPlugIn.h"

#pragma region SampleCppEtoRebuildCurve command

// Name of the named callback registered by the SampleCsEto .NET plug-in. Both
// sides must agree on this string and on the dictionary keys used below.
static const wchar_t* SampleCppEtoRebuildCurveCallbackName = L"SampleCsEtoRebuildCurveDialog";

// Do NOT put the definition of class CCommandSampleCppEtoRebuildCurve in a header
// file. There is only ONE instance of a CCommandSampleCppEtoRebuildCurve class
// and that instance is the static theSampleCppEtoRebuildCurveCommand that appears
// immediately below the class definition.

class CCommandSampleCppEtoRebuildCurve : public CRhinoCommand
{
public:
  CCommandSampleCppEtoRebuildCurve() = default;
  ~CCommandSampleCppEtoRebuildCurve() = default;

  UUID CommandUUID() override
  {
    // {A2799B9D-E8B2-494E-B890-53D5F5E18C41}
    static const GUID SampleCppEtoRebuildCurveCommand_UUID =
    { 0xA2799B9D, 0xE8B2, 0x494E, { 0xB8, 0x90, 0x53, 0xD5, 0xF5, 0xE1, 0x8C, 0x41 } };
    return SampleCppEtoRebuildCurveCommand_UUID;
  }

  const wchar_t* EnglishCommandName() override { return L"SampleCppEtoRebuildCurve"; }

  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleCppEtoRebuildCurve object
static class CCommandSampleCppEtoRebuildCurve theSampleCppEtoRebuildCurveCommand;

CRhinoCommand::result CCommandSampleCppEtoRebuildCurve::RunCommand(const CRhinoCommandContext& context)
{
  // Select a curve to rebuild.
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select curve to rebuild");
  go.SetGeometryFilter(CRhinoGetObject::curve_object);
  go.EnableSubObjectSelect(false);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef objref = go.Object(0);
  const ON_Curve* curve = objref.Curve();
  if (nullptr == curve)
    return CRhinoCommand::failure;

  // Get the NURBS form so we can seed the dialog with the curve's point
  // count and degree.
  ON_NurbsCurve nurb;
  if (0 == curve->GetNurbForm(nurb))
    return CRhinoCommand::failure;

  // Fill the dictionary with the inputs for the managed dialog.
  CRhinoParameterDictionary dict;
  dict.SetInt(L"PointCount", nurb.CVCount());
  dict.SetInt(L"Degree", nurb.Degree());
  dict.SetBool(L"DeleteInput", true);
  dict.SetBool(L"PreserveTangents", false);

  // Show the managed Eto dialog by way of the named callback. This returns
  // false if no callback with this name is registered, which happens when the
  // companion SampleCsEto .NET plug-in is not loaded.
  if (!RhinoExecuteNamedCallback(SampleCppEtoRebuildCurveCallbackName, dict))
  {
    RhinoApp().Print(
      L"Unable to show the Rhino \"%ls\" dialog. Make sure the SampleCsEto .NET plug-in is loaded.\n",
      EnglishCommandName());
    return CRhinoCommand::failure;
  }

  // Read the result. The dialog sets "Result" to true when the user clicks OK.
  bool ok = false;
  dict.GetBool(L"Result", ok);
  if (!ok)
    return CRhinoCommand::cancel;

  // Read back the (possibly edited) values.
  int point_count = nurb.CVCount();
  int degree = nurb.Degree();
  bool delete_input = true;
  bool preserve_tangents = false;
  dict.GetInt(L"PointCount", point_count);
  dict.GetInt(L"Degree", degree);
  dict.GetBool(L"DeleteInput", delete_input);
  dict.GetBool(L"PreserveTangents", preserve_tangents);

  // Validate, mirroring SampleCsEto's RebuildCurveArgs.IsValid.
  if (point_count < 2 || point_count > 32767 || degree < 1 || degree > 11)
  {
    RhinoApp().Print(L"Invalid rebuild parameters.\n");
    return CRhinoCommand::failure;
  }

  // Rebuild the curve. RhinoRebuildCurve is the public C++ analog of
  // RhinoCommon's Curve.Rebuild(pointCount, degree, preserveTangents).
  ON_NurbsCurve* rebuilt = RhinoRebuildCurve(*curve, degree, point_count, preserve_tangents);
  if (nullptr == rebuilt)
  {
    RhinoApp().Print(L"Unable to rebuild the curve.\n");
    return CRhinoCommand::failure;
  }

  if (delete_input)
    context.m_doc.ReplaceObject(objref, *rebuilt);
  else
    context.m_doc.AddCurveObject(*rebuilt);

  delete rebuilt;

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCppEtoRebuildCurve command
//
