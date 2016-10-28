Partial Friend Class Examples
  Public Shared Function ConduitDrawShadedMesh(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim mesh As Rhino.Geometry.Mesh = MeshBox(100, 500, 10)
	If Nothing Is mesh Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim conduit As New DrawShadedMeshConduit()
	conduit.Mesh = mesh
	conduit.Enabled = True
	doc.Views.Redraw()

	Dim outputString As String = String.Empty
	Rhino.Input.RhinoGet.GetString("Press <Enter> to continue", True, outputString)

	conduit.Enabled = False
	doc.Views.Redraw()

	Return Rhino.Commands.Result.Success
  End Function

  Public Shared Function MeshBox(ByVal x As Double, ByVal y As Double, ByVal z As Double) As Rhino.Geometry.Mesh
	Dim mesh As New Rhino.Geometry.Mesh()
	mesh.Vertices.Add(0, 0, 0)
	mesh.Vertices.Add(x, 0, 0)
	mesh.Vertices.Add(x, y, 0)
	mesh.Vertices.Add(0, y, 0)
	mesh.Vertices.Add(0, 0, z)
	mesh.Vertices.Add(x, 0, z)
	mesh.Vertices.Add(x, y, z)
	mesh.Vertices.Add(0, y, z)
	mesh.Faces.AddFace(3, 2, 1, 0)
	mesh.Faces.AddFace(4, 5, 6, 7)
	mesh.Faces.AddFace(0, 1, 5, 4)
	mesh.Faces.AddFace(1, 2, 6, 5)
	mesh.Faces.AddFace(2, 3, 7, 6)
	mesh.Faces.AddFace(3, 0, 4, 7)
	mesh.Normals.ComputeNormals()
	mesh.Compact()
	If mesh.IsValid Then
	  Return mesh
	End If
	Return Nothing
  End Function
End Class
