import rhinoscriptsyntax as rs

surface_id,_,_,_,_,_ = rs.GetSurfaceObject("Select surface for curvature measurement")
point = rs.GetPointOnSurface(surface_id,
    "Select point on surface for curvature measurement")
u,v = rs.SurfaceClosestPoint(surface_id, point)

#point, normal, kappa_u, direction_u, kappa_v, direction_v, gaussian, mean =
surface_curvature = rs.SurfaceCurvature(surface_id, (u,v))

point, normal, kappa_u, direction_u, kappa_v, direction_v, gaussian, mean = surface_curvature

print "Surface curvature evaluation at parameter: ({0}, {1})".format(u,v)

print "  3-D Point: ({0}, {1}, {2})".format(point.X, point.Y, point.Z)

print "  3-D Normal: ({0}, {1}, {2})".format(normal.X, normal.Y, normal.Z)

print "  Maximum principal curvature: {0} ({1}, {2}, {3})".format(
  kappa_u, direction_u.X, direction_u.Y, direction_u.Z)

print "  Minimum principal curvature: {0} ({1}, {2}, {3})".format(
  kappa_v, direction_v.X, direction_v.Y, direction_v.Z)

print "  Gaussian curvature: {0}".format(gaussian)
print "  Mean curvature: {0}".format(mean)
