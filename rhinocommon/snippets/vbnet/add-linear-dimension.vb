Partial Friend Class Examples
  Public Shared Function AddLinearDimension(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim dimension As Rhino.Geometry.LinearDimension = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetLinearDimension(dimension)
	If rc Is Rhino.Commands.Result.Success AndAlso dimension IsNot Nothing Then
	  If doc.Objects.AddLinearDimension(dimension) = Guid.Empty Then
		rc = Rhino.Commands.Result.Failure
	  Else
		doc.Views.Redraw()
	  End If
	End If
	Return rc
  End Function
End Class
