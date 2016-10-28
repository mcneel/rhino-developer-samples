Partial Friend Class Examples
  Public Shared Function GetAngle(ByVal doc As RhinoDoc) As Result
	Dim gp = New GetPoint()
	gp.SetCommandPrompt("Base point")
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim base_point = gp.Point()

	gp.SetCommandPrompt("First reference point")
	gp.DrawLineFromPoint(base_point, True)
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim first_point = gp.Point()

	Dim angle_radians As Double = Nothing
	Dim rc = RhinoGet.GetAngle("Second reference point", base_point, first_point, 0, angle_radians)
	If rc Is Result.Success Then
	  RhinoApp.WriteLine("Angle = {0} degrees", RhinoMath.ToDegrees(angle_radians))
	End If

	Return rc
  End Function
End Class
