Partial Friend Class Examples
  Public Shared Function PrincipalCurvature(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select surface for curvature measurement", True, ObjectType.Surface, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim surface = obj_ref.Surface()

	Dim gp = New Rhino.Input.Custom.GetPoint()
	gp.SetCommandPrompt("Select point on surface for curvature measurement")
	gp.Constrain(surface, False)
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim point_on_surface = gp.Point()

	Dim u As Double = Nothing, v As Double = Nothing
	If Not surface.ClosestPoint(point_on_surface, u, v) Then
	  Return Result.Failure
	End If

	Dim surface_curvature = surface.CurvatureAt(u, v)
	If surface_curvature Is Nothing Then
	  Return Result.Failure
	End If

	RhinoApp.WriteLine("Surface curvature evaluation at parameter: ({0}, {1})", u, v)

	RhinoApp.WriteLine("  3-D Point: ({0}, {1}, {2})", surface_curvature.Point.X, surface_curvature.Point.Y, surface_curvature.Point.Z)

	RhinoApp.WriteLine("  3-D Normal: ({0}, {1}, {2})", surface_curvature.Normal.X, surface_curvature.Normal.Y, surface_curvature.Normal.Z)

	RhinoApp.WriteLine(String.Format("  Maximum principal curvature: {0} ({1}, {2}, {3})", surface_curvature.Kappa(0), surface_curvature.Direction(0).X, surface_curvature.Direction(0).Y, surface_curvature.Direction(0).Z))

	RhinoApp.WriteLine(String.Format("  Minimum principal curvature: {0} ({1}, {2}, {3})", surface_curvature.Kappa(1), surface_curvature.Direction(1).X, surface_curvature.Direction(1).Y, surface_curvature.Direction(1).Z))

	RhinoApp.WriteLine("  Gaussian curvature: {0}", surface_curvature.Gaussian)
	RhinoApp.WriteLine("  Mean curvature: {0}", surface_curvature.Mean)

	Return Result.Success
  End Function
End Class
