Partial Friend Class Examples
  Public Shared Function ProjectPointsToBreps(ByVal doc As RhinoDoc) As Result
	Dim gs = New GetObject()
	gs.SetCommandPrompt("select surface")
	gs.GeometryFilter = ObjectType.Surface Or ObjectType.PolysrfFilter
	gs.DisablePreSelect()
	gs.SubObjectSelect = False
	gs.Get()
	If gs.CommandResult() <> Result.Success Then
	  Return gs.CommandResult()
	End If
	Dim brep = gs.Object(0).Brep()
	If brep Is Nothing Then
	  Return Result.Failure
	End If

	Dim points = Intersection.ProjectPointsToBreps(New List(Of Brep) _
	    From {brep}, New List(Of Point3d)
	    From {New Point3d(0, 0, 0), New Point3d(3,0,3), New Point3d(-2,0,-2)}, New Vector3d(0, 1, 0), doc.ModelAbsoluteTolerance) ' project on Y axis -  some random points to project -  brep on which to project

	If points IsNot Nothing AndAlso points.Length > 0 Then
	  For Each point In points
		doc.Objects.AddPoint(point)
	  Next point
	End If
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
