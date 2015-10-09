import rhinoscriptsyntax as rs
from scriptcontext import *
from Rhino.Geometry import *

def RunCommand():
  srfid = rs.GetObject("select surface", rs.filter.surface | rs.filter.polysurface)
  if not srfid: return
  brep = rs.coercebrep(srfid)
  if not brep: return
  
  pts = Intersect.Intersection.ProjectPointsToBreps(
      [brep], # brep on which to project
      [Point3d(0, 0, 0), Point3d(3,0,3), Point3d(-2,0,-2)], # points to project
      Vector3d(0, 1, 0), # project on Y axis
      doc.ModelAbsoluteTolerance)

  if pts != None and pts.Length > 0:
    for pt in pts:
      doc.Objects.AddPoint(pt)

  doc.Views.Redraw()

if __name__ == "__main__":
    RunCommand()