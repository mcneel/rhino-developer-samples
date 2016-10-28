import rhinoscriptsyntax as rs
from scriptcontext import doc
import Rhino
from Rhino.Geometry.Intersect import Intersection, LineCircleIntersection

def RunCommand():
  rc, circle = Rhino.Input.RhinoGet.GetCircle()
  if rc != Rhino.Commands.Result.Success:
    return rc
  doc.Objects.AddCircle(circle)
  doc.Views.Redraw()

  rc, line = Rhino.Input.RhinoGet.GetLine()
  if rc != Rhino.Commands.Result.Success:
    return rc
  doc.Objects.AddLine(line)
  doc.Views.Redraw()

  lineCircleIntersect, t1, point1, t2, point2 = Intersection.LineCircle(line, circle)
  message = ""
  if lineCircleIntersect == LineCircleIntersection.None:
    message = "line does not intersect circle"
  elif lineCircleIntersect == LineCircleIntersection.Single:
    message = "line intersects circle at point ({0},{1},{2})".format(point1.X, point1.Y, point1.Z)
    doc.Objects.AddPoint(point1)
  elif lineCircleIntersect == LineCircleIntersection.Multiple:
    message = "line intersects circle at points ({0},{1},{2}) and ({3},{4},{5})".format(
      point1.X, point1.Y, point1.Z, point2.X, point2.Y, point2.Z)
    doc.Objects.AddPoint(point1)
    doc.Objects.AddPoint(point2)
    
  print message
  doc.Views.Redraw()

if __name__ == "__main__":
    RunCommand()