Partial Friend Class Examples
  Public Shared Function AddLine(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim gp As New Rhino.Input.Custom.GetPoint()
	gp.SetCommandPrompt("Start of line")
	gp.Get()
	If gp.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return gp.CommandResult()
	End If

	Dim pt_start As Rhino.Geometry.Point3d = gp.Point()

	gp.SetCommandPrompt("End of line")
	gp.SetBasePoint(pt_start, False)
	gp.DrawLineFromPoint(pt_start, True)
	gp.Get()
	If gp.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return gp.CommandResult()
	End If

	Dim pt_end As Rhino.Geometry.Point3d = gp.Point()
	Dim v As Rhino.Geometry.Vector3d = pt_end - pt_start
	If v.IsTiny(Rhino.RhinoMath.ZeroTolerance) Then
	  Return Rhino.Commands.Result.Nothing
	End If

	If doc.Objects.AddLine(pt_start, pt_end) <> Guid.Empty Then
	  doc.Views.Redraw()
	  Return Rhino.Commands.Result.Success
	End If
	Return Rhino.Commands.Result.Failure
  End Function
End Class
