Partial Friend Class Examples
  Public Shared Function AddMeshBox(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim box As Rhino.Geometry.Box = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetBox(box)
	If rc Is Rhino.Commands.Result.Success Then
	  Dim mesh As Rhino.Geometry.Mesh = Rhino.Geometry.Mesh.CreateFromBox(box, 2, 2, 2)
	  If Nothing IsNot mesh Then
		doc.Objects.AddMesh(mesh)
		doc.Views.Redraw()
		Return Rhino.Commands.Result.Success
	  End If
	End If

	Return Rhino.Commands.Result.Failure
  End Function
End Class
