import Rhino
from Rhino.Geometry import *
import rhinoscriptsyntax as rs
from scriptcontext import doc
import math

def RunCommand():
  surface_id = rs.GetSurfaceObject()[0]
  if surface_id == None:
    return
  surface = rs.coercesurface(surface_id)

  corners = rs.GetRectangle()
  if corners == None:
    return

  plane = Plane(corners[0], corners[1], corners[2])

  is_or_isnt = "" if IsSurfaceInPlane(surface, plane, doc.ModelAbsoluteTolerance) else " not "
  print "Surface is{0} in plane.".format(is_or_isnt)

def IsSurfaceInPlane(surface, plane, tolerance):
  if not surface.IsPlanar(tolerance):
    return False

  bbox = surface.GetBoundingBox(True)
  rc = True
  for corner in bbox.GetCorners():
      if math.fabs(plane.DistanceTo(corner)) > tolerance:
          rc = False
          break

  return rc

if __name__ == "__main__":
    RunCommand()
