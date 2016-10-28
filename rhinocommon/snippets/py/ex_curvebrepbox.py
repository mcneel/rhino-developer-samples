import Rhino
from Rhino.Geometry import *
from Rhino.Commands import Result
from Rhino.Input import RhinoGet
from Rhino.DocObjects import ObjectType
import rhinoscriptsyntax as rs
from scriptcontext import doc

def RunCommand():
  rc, objRef = RhinoGet.GetOneObject("Select curve", False, ObjectType.Curve)
  if rc <> Result.Success:
    return rc
  curve = objRef.Curve()
  if None == curve:
    return Result.Failure

  view = doc.Views.ActiveView
  plane = view.ActiveViewport.ConstructionPlane()
  # Create a construction plane aligned bounding box
  bbox = curve.GetBoundingBox(plane)

  if bbox.IsDegenerate(doc.ModelAbsoluteTolerance) > 0:
    print "the curve's bounding box is degenerate (flat) in at least one direction so a box cannot be created."
    return Result.Failure

  brep = Brep.CreateFromBox(bbox)
  doc.Objects.AddBrep(brep)
  doc.Views.Redraw()

  return Result.Success

if __name__ == "__main__":
  print RunCommand()
