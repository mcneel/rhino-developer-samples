from Rhino.Geometry import Point3d, NurbsSurface, ControlPoint
from scriptcontext import doc

def RunCommand():
  bIsRational = False
  dim = 3
  u_degree = 2
  v_degree = 3
  u_cv_count = 3
  v_cv_count = 5
 
  # make up a quadratic knot vector with no interior knots
  u_knot = [0.0, 0.0, 1.0, 1.0] 
 
  # make up a cubic knot vector with one simple interior knot
  v_knot = [0.0, 0.0, 0.0, 1.5, 2.0, 2.0, 2.0]
 
  # Rational control points can be in either homogeneous
  # or euclidean form. Non-rational control points do not
  # need to specify a weight.  
  CV = dict( ((i,j),None) for i in range(2) for j in range(3) )
  for i in range(0, u_cv_count):
    for j in range(0, v_cv_count):
      CV[i,j] = Point3d(i, j, i-j)
 
  # creates internal uninitialized arrays for 
  # control points and knots
  nurbs_surface = NurbsSurface.Create(
    dim,
    bIsRational,
    u_degree + 1,
    v_degree + 1,
    u_cv_count,
    v_cv_count
    )
 
  # add the knots
  for i in range(0, nurbs_surface.KnotsU.Count):
    nurbs_surface.KnotsU[i] = u_knot[i]
  for j in range(0, nurbs_surface.KnotsV.Count):
    nurbs_surface.KnotsV[j] = v_knot[j]

  # add the control points
  for i in range(0, nurbs_surface.Points.CountU):
    for j in range(0, nurbs_surface.Points.CountV):
      nurbs_surface.Points.SetControlPoint(i, j, ControlPoint(CV[i, j]))

  if nurbs_surface.IsValid:
    doc.Objects.AddSurface(nurbs_surface)
    doc.Views.Redraw()

if __name__ == "__main__":
  RunCommand()