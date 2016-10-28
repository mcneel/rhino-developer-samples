Partial Friend Class Examples
  Public Shared Function IsPlanarSurfaceInPlane(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("select surface", True, ObjectType.Surface, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim surface = obj_ref.Surface()

	Dim corners() As Point3d = Nothing
	rc = RhinoGet.GetRectangle(corners)
	If rc IsNot Result.Success Then
	  Return rc
	End If

	Dim plane = New Plane(corners(0), corners(1), corners(2))

	Dim is_or_isnt = " not "
	If IsSurfaceInPlane(surface, plane, doc.ModelAbsoluteTolerance) Then
	  is_or_isnt = ""
	End If

	RhinoApp.WriteLine("Surface is{0} in plane.", is_or_isnt)
	Return Result.Success
  End Function

  Private Shared Function IsSurfaceInPlane(ByVal surface As Surface, ByVal plane As Plane, ByVal tolerance As Double) As Boolean
	If Not surface.IsPlanar(tolerance) Then
	  Return False
	End If

	Dim bbox = surface.GetBoundingBox(True)
	Return bbox.GetCorners().All(Function(c) System.Math.Abs(plane.DistanceTo(c)) <= tolerance)
  End Function
End Class
