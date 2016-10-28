import rhinoscriptsyntax as rs
from Rhino.Geometry import Intersect, Point3d, Vector3d
from scriptcontext import doc

def RunCommand():
  # select a surface
  srfid = rs.GetObject("select serface", rs.filter.surface | rs.filter.polysurface)
  if not srfid: return
  # get the brep
  brep = rs.coercebrep(srfid)
  if not brep: return

  x = rs.GetReal("value of x", 0)
  y = rs.GetReal("value of y", 0)

  pts = [(abs(point.Z), point.Z) for point in Intersect.Intersection.ProjectPointsToBreps(
           [brep], [Point3d(x, y, 0)], Vector3d(0, 0, 1), doc.ModelAbsoluteTolerance)]
           
  pts.sort(reverse=True)
  
  if pts == []:
    print "no Z for given X, Y"
  else:
    rs.AddPoint(Point3d(x, y, pts[0][1]))

if __name__ == "__main__":
  RunCommand()