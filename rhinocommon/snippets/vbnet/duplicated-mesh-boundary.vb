Partial Friend Class Examples
  Public Shared Function DupMeshBoundary(ByVal doc As RhinoDoc) As Result
	Dim gm = New GetObject()
	gm.SetCommandPrompt("Select open mesh")
	gm.GeometryFilter = ObjectType.Mesh
	gm.GeometryAttributeFilter = GeometryAttributeFilter.OpenMesh
	gm.Get()
	If gm.CommandResult() <> Result.Success Then
	  Return gm.CommandResult()
	End If
	Dim mesh = gm.Object(0).Mesh()
	If mesh Is Nothing Then
	  Return Result.Failure
	End If

	Dim polylines = mesh.GetNakedEdges()
	For Each polyline In polylines
	  doc.Objects.AddPolyline(polyline)
	Next polyline

	Return Result.Success
  End Function
End Class
