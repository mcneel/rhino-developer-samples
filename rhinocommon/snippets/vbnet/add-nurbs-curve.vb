Partial Friend Class Examples
  Public Shared Function AddNurbsCurve(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim points As New Rhino.Collections.Point3dList(5)
	points.Add(0, 0, 0)
	points.Add(0, 2, 0)
	points.Add(2, 3, 0)
	points.Add(4, 2, 0)
	points.Add(4, 0, 0)
	Dim nc As Rhino.Geometry.NurbsCurve = Rhino.Geometry.NurbsCurve.Create(False, 3, points)
	Dim rc As Rhino.Commands.Result = Rhino.Commands.Result.Failure
	If nc IsNot Nothing AndAlso nc.IsValid Then
	  If doc.Objects.AddCurve(nc) <> Guid.Empty Then
		doc.Views.Redraw()
		rc = Rhino.Commands.Result.Success
	  End If
	End If
	Return rc
  End Function
End Class
