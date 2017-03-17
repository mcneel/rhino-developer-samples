#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleBoundingBox command
//

#pragma region SampleBoundingBox command

class CCommandSampleBoundingBox : public CRhinoCommand
{
public:
  CCommandSampleBoundingBox() { m_use_cplane = false; }
  ~CCommandSampleBoundingBox() {}
  UUID CommandUUID()
  {
    // {9C68FA5F-413A-40B9-BC49-15AB6685F986}
    static const GUID SampleBoundingBoxCommand_UUID =
    { 0x9C68FA5F, 0x413A, 0x40B9, { 0xBC, 0x49, 0x15, 0xAB, 0x66, 0x85, 0xF9, 0x86 } };
    return SampleBoundingBoxCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleBoundingBox"; }
  const wchar_t* LocalCommandName() const { return L"SampleBoundingBox"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  int ClassifyBoundingBox(ON_3dPoint box_corners[8], ON_3dPoint rect[5], ON_Line& line);

private:
  bool m_use_cplane;
};

// The one and only CCommandSampleBoundingBox object
static class CCommandSampleBoundingBox theSampleBoundingBoxCommand;

CRhinoCommand::result CCommandSampleBoundingBox::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select objects to calculate tight bounding box");
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"CoordinateSystem"), RHCMDOPTVALUE(L"World"), RHCMDOPTVALUE(L"CPlane"), m_use_cplane, &m_use_cplane);
  for (;;)
  {
    CRhinoGet::result res = go.GetObjects(1, 0);
    if (res == CRhinoGet::option)
      continue;
    else if (res != CRhinoGet::object)
      return CRhinoCommand::cancel;
    break;
  }

  int i, object_count = go.ObjectCount();
  ON_SimpleArray<const CRhinoObject*> objects(object_count);
  for (i = 0; i < object_count; i++)
  {
    const CRhinoObject* obj = go.Object(i).Object();
    if (nullptr != obj)
      objects.Append(obj);
  }

  ON_Plane plane, *onb = nullptr;
  if (m_use_cplane)
  {
    CRhinoView* view = go.View();
    if (nullptr != view)
    {
      plane = view->ActiveViewport().ConstructionPlane().m_plane;
      onb = &plane;
    }
  }

  ON_BoundingBox bbox;
  if (!RhinoGetTightBoundingBox(objects, bbox, false, onb))
    return CRhinoCommand::cancel;

  ON_3dPoint box_corners[8];

  box_corners[0] = bbox.Corner(0, 0, 0);
  box_corners[1] = bbox.Corner(1, 0, 0);
  box_corners[2] = bbox.Corner(1, 1, 0);
  box_corners[3] = bbox.Corner(0, 1, 0);

  box_corners[4] = bbox.Corner(0, 0, 1);
  box_corners[5] = bbox.Corner(1, 0, 1);
  box_corners[6] = bbox.Corner(1, 1, 1);
  box_corners[7] = bbox.Corner(0, 1, 1);

  if (nullptr != onb)
  {
    // Transform corners poins from construction plane coordinates
    // to world coordinates if necessary
    ON_Xform plane_to_world(1.0);
    plane_to_world.ChangeBasis(plane, ON_xy_plane);
    for (i = 0; i < 8; i++)
      box_corners[i].Transform(plane_to_world);
  }

  ON_3dPoint rect[5];
  ON_Line line;
  int type = ClassifyBoundingBox(box_corners, rect, line);

  if (type == 3)
  {
    RhinoApp().Print(L"BoundingBox failed. The bounding box is a point.\n");
    // box_corners[0] can be used for the point coordinate if necessary
  }
  else if (type == 2)
  {
    RhinoApp().Print(L"BoundingBox failed. The bounding box is a line.\n");
    // "line" variable contains the line if necessary
  }
  else if (type == 1)
  {
    // The bounding box is a rectangle
    ON_Polyline pline;
    pline.Create(3, FALSE, 5, 3, (double*)&rect);
    context.m_doc.AddCurveObject(pline);
  }
  else
  {
    // The bounding box is a box
    ON_Brep brep_box;
    ON_BrepBox(box_corners, &brep_box);
    context.m_doc.AddBrepObject(brep_box);
  }

  context.m_doc.Redraw();

  return CRhinoCommand::success;
}

int CCommandSampleBoundingBox::ClassifyBoundingBox(ON_3dPoint box_corners[8], ON_3dPoint rect[5], ON_Line& line)
{
  // Classifies the bounding box type and returns a simpler version
  // of the box if it is a rectangle or a line, where:
  //   0 = box
  //   1 = rectangle
  //   2 = line
  //   3 = point

  int numflat = 0;
  bool xflat = false, yflat = false, zflat = false;

  // flat in box x-direction?
  if (FLT_EPSILON > box_corners[0].DistanceTo(box_corners[1]))
  {
    numflat++;
    xflat = true;
  }

  // flat in box y-direction?
  if (FLT_EPSILON > box_corners[0].DistanceTo(box_corners[3]))
  {
    numflat++;
    yflat = true;
  }

  // flat in box z-direction?
  if (FLT_EPSILON > box_corners[0].DistanceTo(box_corners[4]))
  {
    numflat++;
    zflat = true;
  }

  if (numflat == 2)
  {
    line.from = box_corners[0];
    if (!xflat)
      line.to = box_corners[1];
    else if (!yflat)
      line.to = box_corners[3];
    else
      line.to = box_corners[4];
  }
  else if (numflat == 1)
  {
    rect[0] = rect[4] = box_corners[0];
    if (xflat)
    {
      rect[1] = box_corners[4];
      rect[2] = box_corners[7];
      rect[3] = box_corners[3];
    }
    else if (yflat)
    {
      rect[1] = box_corners[1];
      rect[2] = box_corners[5];
      rect[3] = box_corners[4];
    }
    else // zflat
    {
      rect[1] = box_corners[1];
      rect[2] = box_corners[2];
      rect[3] = box_corners[3];
    }
  }

  return numflat;
}

#pragma endregion

//
// END SampleBoundingBox command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
