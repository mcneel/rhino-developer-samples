#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMemoryBuffer command
//

#pragma region SampleMemoryBuffer command

static bool write_curves_example(ON_Buffer& buffer, int version, ON_TextLog& error_log)
{
  // example demonstrates how to write a NURBS curve, line, and circle
  ONX_Model model;

  // file properties (notes, preview image, revision history, ...)

  // set revision history information
  model.m_properties.m_RevisionHistory.NewRevision();

  // set application information
  model.m_properties.m_Application.m_application_name = "OpenNURBS write_curves_example() function";
  model.m_properties.m_Application.m_application_URL = "http://www.opennurbs.org";
  model.m_properties.m_Application.m_application_details = "Example program in OpenNURBS toolkit.";

  // some notes
  model.m_properties.m_Notes.m_notes = "This file was made with the OpenNURBS write_curves_example() function.";
  model.m_properties.m_Notes.m_bVisible = true;

  // file settings (units, tolerances, views, ...)
  model.m_settings.m_ModelUnitsAndTolerances.m_unit_system = ON_UnitSystem::Inches;
  model.m_settings.m_ModelUnitsAndTolerances.m_absolute_tolerance = 0.001;
  model.m_settings.m_ModelUnitsAndTolerances.m_angle_tolerance = ON_PI / 180.0; // radians
  model.m_settings.m_ModelUnitsAndTolerances.m_relative_tolerance = 0.01; // 1%

  // layer table
  {
    // OPTIONAL - define some layers
    ON_Layer layer[3];

    layer[0].SetName(L"Default");
    layer[0].SetVisible(true);
    layer[0].SetLocked(false);
    layer[0].SetColor(ON_Color(0, 0, 0));

    layer[1].SetName(L"green NURBS wiggle");
    layer[1].SetVisible(true);
    layer[1].SetLocked(false);
    layer[1].SetIndex(1);
    layer[1].SetColor(ON_Color(0, 255, 0));

    layer[2].SetName(L"blue circles");
    layer[2].SetVisible(true);
    layer[2].SetLocked(false);
    layer[2].SetIndex(2);
    layer[2].SetColor(ON_Color(0, 0, 255));

    model.AddModelComponent(layer[0], true);
    model.AddModelComponent(layer[1], true);
    model.AddModelComponent(layer[2], true);
  }

  // object table
  {
    {
      // write a line on the default layer
      ON_LineCurve line_curve(ON_Line(ON_3dPoint(1.0, 2.0, -1.5), ON_3dPoint(5.0, 3.0, 2.0)));
      ON_3dmObjectAttributes attributes;
      attributes.m_layer_index = 0;
      attributes.m_name = "straight line curve";
      model.AddModelGeometryComponent(&line_curve, &attributes, true);
    }

    {
      // write a wiggly cubic curve on the "green NURBS wiggle" layer
      ON_NurbsCurve wiggle(
        3,     // dimension
        false, // true if rational
        4,     // order = degree+1
        6      // number of control vertices
      );

      for (int i = 0; i < wiggle.CVCount(); i++)
      {
        ON_3dPoint pt(2 * i, -i, (i - 3)*(i - 3)); // pt = some 3d point
        wiggle.SetCV(i, pt);
      }

      // ON_NurbsCurve's have order+cv_count-2 knots.
      wiggle.SetKnot(0, 0.0);
      wiggle.SetKnot(1, 0.0);
      wiggle.SetKnot(2, 0.0);
      wiggle.SetKnot(3, 1.5);
      wiggle.SetKnot(4, 2.3);
      wiggle.SetKnot(5, 4.0);
      wiggle.SetKnot(6, 4.0);
      wiggle.SetKnot(7, 4.0);

      if (wiggle.IsValid())
      {
        ON_3dmObjectAttributes attributes;
        attributes.m_layer_index = 1;
        attributes.m_name = "wiggly cubic curve";
        model.AddModelGeometryComponent(&wiggle, &attributes, true);
      }
    }

    {
      // write two circles on the "blue circles"
      ON_ArcCurve circle_curve1(ON_Circle(ON_3dPoint(1.0, 2.0, -1.5), 3.0));
      ON_3dmObjectAttributes attributes1;
      attributes1.m_layer_index = 2;
      attributes1.m_name = "radius 3 circle";
      model.AddModelGeometryComponent(&circle_curve1, &attributes1, true);

      ON_ArcCurve circle_curve2(ON_Circle(ON_3dPoint(1.0, 2.0, -1.5), 5.0));
      ON_3dmObjectAttributes attributes2;
      attributes2.m_layer_index = 2;
      attributes2.m_name = "radius 5 circle";
      model.AddModelGeometryComponent(&circle_curve2, &attributes2, true);
    }
  }

  ON_BinaryArchiveBuffer archive(ON::archive_mode::write3dm, &buffer);

  // writes model to archive
  bool ok = model.Write(archive, version, &error_log);

  return ok;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleMemoryBuffer : public CRhinoCommand
{
public:
  CCommandSampleMemoryBuffer() {}
  ~CCommandSampleMemoryBuffer() {}
  UUID CommandUUID()
  {
    // {20B46D76-345B-4495-99AB-DFB4B9FE2D33}
    static const GUID SampleMemoryBufferCommand_UUID =
    { 0x20B46D76, 0x345B, 0x4495, { 0x99, 0xAB, 0xDF, 0xB4, 0xB9, 0xFE, 0x2D, 0x33 } };
    return SampleMemoryBufferCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMemoryBuffer"; }
  const wchar_t* LocalCommandName() const { return L"SampleMemoryBuffer"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMemoryBuffer object
static class CCommandSampleMemoryBuffer theSampleMemoryBufferCommand;

CRhinoCommand::result CCommandSampleMemoryBuffer::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  // Demonstrates how to round-trip an ONX_Model to a binary archive buffer

  int version = 0; // version will be ON_BinaryArchive::CurrentArchiveVersion()

  ON_Buffer buffer;
  ON_wString write_str;
  ON_TextLog write_log(write_str);
  bool ok = write_curves_example(buffer, version, write_log);

  if (ok)
  {
    buffer.Seek(0, SEEK_SET); // Seek from beginning of file

    ONX_Model model;
    ON_wString read_str;
    ON_TextLog read_log(read_str);
    ON_BinaryArchiveBuffer archive(ON::archive_mode::read3dm, &buffer);
    ok = model.Read(archive, &read_log);
    if (ok)
    {
      // TODO: process model object...
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleMemoryBuffer command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
