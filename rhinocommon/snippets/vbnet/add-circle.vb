Partial Friend Class Examples
  Public Shared Function AddCircle(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim center As New Rhino.Geometry.Point3d(0, 0, 0)
	Const radius As Double = 10.0
	Dim c As New Rhino.Geometry.Circle(center, radius)
	If doc.Objects.AddCircle(c) <> Guid.Empty Then
	  doc.Views.Redraw()
	  Return Rhino.Commands.Result.Success
	End If
	Return Rhino.Commands.Result.Failure
  End Function
End Class
