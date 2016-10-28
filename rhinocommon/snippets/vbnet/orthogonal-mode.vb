Partial Friend Class Examples
  Public Shared Function Ortho(ByVal doc As RhinoDoc) As Result
	Dim gp = New GetPoint()
	gp.SetCommandPrompt("Start of line")
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim start_point = gp.Point()

	Dim original_ortho = ModelAidSettings.Ortho
	If Not original_ortho Then
	  ModelAidSettings.Ortho = True
	End If

	gp.SetCommandPrompt("End of line")
	gp.SetBasePoint(start_point, False)
	gp.DrawLineFromPoint(start_point, True)
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim end_point = gp.Point()

	If ModelAidSettings.Ortho IsNot original_ortho Then
	  ModelAidSettings.Ortho = original_ortho
	End If

	doc.Objects.AddLine(start_point, end_point)
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
