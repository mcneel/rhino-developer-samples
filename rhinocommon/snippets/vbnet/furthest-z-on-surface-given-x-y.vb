Partial Friend Class Examples
  Public Shared Function FurthestZOnSurfaceGivenXY(ByVal doc As RhinoDoc) As Result
'	#Region "user input"
	' select a surface
	Dim gs = New GetObject()
	gs.SetCommandPrompt("select surface")
	gs.GeometryFilter = ObjectType.Surface
	gs.DisablePreSelect()
	gs.SubObjectSelect = False
	gs.Get()
	If gs.CommandResult() <> Result.Success Then
	  Return gs.CommandResult()
	End If
	' get the brep
	Dim brep = gs.Object(0).Brep()
	If brep Is Nothing Then
	  Return Result.Failure
	End If

	' get X and Y
	Dim x As Double = 0.0, y As Double = 0.0
	Dim rc = RhinoGet.GetNumber("value of X coordinate", True, x)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	rc = RhinoGet.GetNumber("value of Y coordinate", True, y)
	If rc IsNot Result.Success Then
	  Return rc
	End If
'	#End Region

	' an earlier version of this sample used a curve-brep intersection to find Z
	'var maxZ = maxZIntersectionMethod(brep, x, y, doc.ModelAbsoluteTolerance);

	' projecting points is another way to find Z
	Dim max_z = MaxZProjectionMethod(brep, x, y, doc.ModelAbsoluteTolerance)

	If max_z IsNot Nothing Then
	  RhinoApp.WriteLine("Maximum surface Z coordinate at X={0}, Y={1} is {2}", x, y, max_z)
	  doc.Objects.AddPoint(New Point3d(x, y, max_z.Value))
	  doc.Views.Redraw()
	Else
	  RhinoApp.WriteLine("no maximum surface Z coordinate at X={0}, Y={1} found.", x, y)
	End If

	Return Result.Success
  End Function

  Private Shared Function MaxZProjectionMethod(ByVal brep As Brep, ByVal x As Double, ByVal y As Double, ByVal tolerance As Double) As Double?
	Dim max_z? As Double = Nothing
	Dim breps = New List(Of Brep) From {brep}
	Dim points = New List(Of Point3d) From {New Point3d(x, y, 0)}
	' grab all the points projected in Z dir.  Aggregate finds furthest Z from XY plane
	Try
	  max_z = (
	      From pt In Intersection.ProjectPointsToBreps(breps, points, New Vector3d(0, 0, 1), tolerance)
	      Select pt.Z).Aggregate(Function(z1, z2)If(Math.Abs(z1) > Math.Abs(z2), z1, z2))
			  ' Here you might be tempted to use .Max() to get the largest Z value but that doesn't work because
			  ' Z might be negative.  This custom aggregate returns the max Z independant of the sign.  If it had a name
			  ' it could be MaxAbs()
	Catch e1 As InvalidOperationException 'Sequence contains no elements
	End Try
	Return max_z
  End Function

  Private Shared Function MaxZIntersectionMethod(ByVal brep As Brep, ByVal x As Double, ByVal y As Double, ByVal tolerance As Double) As Double?
	Dim max_z? As Double = Nothing

	Dim bbox = brep.GetBoundingBox(True)
	Dim max_dist_from_xy = (
	    From corner In bbox.GetCorners()
	    Select corner.Z).Aggregate(Function(z1, z2)If(Math.Abs(z1) > Math.Abs(z2), z1, z2))
							' furthest Z from XY plane.
							' Here you might be tempted to use .Max() to get the largest Z value but that doesn't work because
							' Z might be negative.  This custom aggregate returns the max Z independant of the sign.  If it had a name
							' it could be MaxAbs()
	' multiply distance by 2 to make sure line intersects completely
	Dim line_curve = New LineCurve(New Point3d(x, y, 0), New Point3d(x, y, max_dist_from_xy*2))

	Dim overlap_curves() As Curve = Nothing
	Dim inter_points() As Point3d = Nothing
	If Intersection.CurveBrep(line_curve, brep, tolerance, overlap_curves, inter_points) Then
	  If overlap_curves.Length > 0 OrElse inter_points.Length > 0 Then
		' grab all the points resulting frem the intersection.
		'    1st set: points from overlapping curves,
		'    2nd set: points when there was no overlap
		'    .Aggregate: furthest Z from XY plane.
		max_z = (
		    From c In overlap_curves
		    Select If(Math.Abs(c.PointAtEnd.Z) > Math.Abs(c.PointAtStart.Z), c.PointAtEnd.Z, c.PointAtStart.Z)).Union(
		    From p In inter_points
		    Select p.Z).Aggregate(Function(z1, z2)If(Math.Abs(z1) > Math.Abs(z2), z1, z2))
				 ' Here you might be tempted to use .Max() to get the largest Z value but that doesn't work because
				 ' Z might be negative.  This custom aggregate returns the max Z independant of the sign.  If it had a name
				 ' it could be MaxAbs()
	  End If
	End If
	Return max_z
  End Function
End Class
