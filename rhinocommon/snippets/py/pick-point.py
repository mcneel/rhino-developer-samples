from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Input import *
from Rhino.Input.Custom import *
from scriptcontext import doc
import rhinoscriptsyntax as rs

def RunCommand():
  # creates a point where the mouse is clicked
  # using RhinoCommon
  gp = GetPoint()
  gp.SetCommandPrompt("Click for point")
  gp.Get()
  if gp.CommandResult() <> Result.Success:
    return gp.CommandResult()
  point3d = gp.Point()
  doc.Objects.AddPoint(point3d)
  doc.Views.Redraw()

  # creates a point where the mouse is clicked
  # using the RhinoScript syntax
  point3d = rs.GetPoint("Click for point")
  if point3d == None: return Result.Nothing
  rs.AddPoint(point3d)
  doc.Objects.AddPoint(point3d)


  # selects a point that already exists
  # using RhinoCommon
  rc, obj_ref = RhinoGet.GetOneObject(
    "Select point", False, ObjectType.Point)
  if rc <> Result.Success:
    return rc
  point = obj_ref.Point()
  RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}",
    point.Location.X,
    point.Location.Y,
    point.Location.Z)
  doc.Objects.UnselectAll()

  # also selects a point that already exists
  # using RhinoCommon
  # Used when RhinoGet doesn't provide enough control
  go = GetObject()
  go.SetCommandPrompt("Select point")
  go.GeometryFilter = ObjectType.Point
  go.GetMultiple(1, 0)
  if go.CommandResult() <> Result.Success:
    return go.CommandResult()
  for o_ref in  go.Objects():
    point = o_ref.Point()
    if point <> None:
      RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}",
        point.Location.X,
        point.Location.Y,
        point.Location.Z)
  doc.Objects.UnselectAll()

  # selects a point that already exists
  # using RhinoScript syntax
  point_id = rs.GetObject(
    "Select point", rs.filter.point)
  if point_id == None: return Result.Nothing
  print "point id: {0}".format(point_id)
  rs.UnselectAllObjects()


  # selects multiple points that already exist
  rc, obj_refs = RhinoGet.GetMultipleObjects(
    "Select point", False, ObjectType.Point)
  if rc <> Result.Success:
    return rc
  for o_ref in obj_refs:
    point = o_ref.Point()
    RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}",
      point.Location.X,
      point.Location.Y,
      point.Location.Z)
  doc.Objects.UnselectAll()

  # selects multiple poins that already exists
  # using the RhinoScript syntax
  point_ids = rs.GetObjects(
    "Select point", rs.filter.point)
  for p_id in point_ids:
    print "point id: {0}".format(p_id)

  return Result.Success

if __name__ == "__main__":
  RunCommand()
