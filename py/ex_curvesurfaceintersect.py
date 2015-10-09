import rhinoscriptsyntax as rs
from scriptcontext import *
import Rhino
import System.Collections.Generic as scg
import System as s

def RunCommand():
  srfid = rs.GetObject("select surface", rs.filter.surface | rs.filter.polysurface)
  if not srfid: return
 
  crvid = rs.GetObject("select curve", rs.filter.curve)
  if not crvid: return

  result = rs.CurveBrepIntersect(crvid, srfid)
  if result == None:
    print "no intersection"
    return
  
  curves, points = result
  for curve in curves:
    doc.Objects.AddCurve(rs.coercecurve(curve))
  for point in points:
    rs.AddPoint(point)

  doc.Views.Redraw()

if __name__ == "__main__":
  RunCommand()
