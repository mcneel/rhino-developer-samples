Partial Friend Class Examples
  Public Shared Function CreateSurfaceFromPointsAndKnots(ByVal doc As RhinoDoc) As Result
	Const is_rational As Boolean = False
	Const number_of_dimensions As Integer = 3
	Const u_degree As Integer = 2
	Const v_degree As Integer = 3
	Const u_control_point_count As Integer = 3
	Const v_control_point_count As Integer = 5

	' The knot vectors do NOT have the 2 superfluous knots
	' at the start and end of the knot vector.  If you are
	' coming from a system that has the 2 superfluous knots,
	' just ignore them when creating NURBS surfaces.
	Dim u_knots = New Double(u_control_point_count + u_degree - 2){}
	Dim v_knots = New Double(v_control_point_count + v_degree - 2){}

	' make up a quadratic knot vector with no interior knots
	u_knots(1) = 0.0
	u_knots(0) = u_knots(1)
	u_knots(3) = 1.0
	u_knots(2) = u_knots(3)

	' make up a cubic knot vector with one simple interior knot
	v_knots(2) = 0.0
	v_knots(1) = v_knots(2)
	v_knots(0) = v_knots(1)
	v_knots(3) = 1.5
	v_knots(6) = 2.0
	v_knots(5) = v_knots(6)
	v_knots(4) = v_knots(5)

	' Rational control points can be in either homogeneous
	' or euclidean form. Non-rational control points do not
	' need to specify a weight.  
	Dim control_points = New Point3d(u_control_point_count - 1, v_control_point_count - 1){}

	For u As Integer = 0 To u_control_point_count - 1
	  For v As Integer = 0 To v_control_point_count - 1
		control_points(u,v) = New Point3d(u, v, u-v)
	  Next v
	Next u

	' creates internal uninitialized arrays for 
	' control points and knots
	Dim nurbs_surface = NurbsSurface.Create(number_of_dimensions, is_rational, u_degree + 1, v_degree + 1, u_control_point_count, v_control_point_count)

	' add the knots
	For u As Integer = 0 To nurbs_surface.KnotsU.Count - 1
	  nurbs_surface.KnotsU(u) = u_knots(u)
	Next u
	For v As Integer = 0 To nurbs_surface.KnotsV.Count - 1
	  nurbs_surface.KnotsV(v) = v_knots(v)
	Next v

	' add the control points
	For u As Integer = 0 To nurbs_surface.Points.CountU - 1
	  For v As Integer = 0 To nurbs_surface.Points.CountV - 1
		nurbs_surface.Points.SetControlPoint(u, v, control_points(u, v))
	  Next v
	Next u

	If nurbs_surface.IsValid Then
	  doc.Objects.AddSurface(nurbs_surface)
	  doc.Views.Redraw()
	  Return Result.Success
	End If
	Return Result.Failure
  End Function
End Class
