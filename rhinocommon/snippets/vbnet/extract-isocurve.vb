Partial Friend Class Examples
  Public Shared Function ExtractIsoCurve(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select surface", False, ObjectType.Surface, obj_ref)
	If rc IsNot Result.Success OrElse obj_ref Is Nothing Then
	  Return rc
	End If
	Dim surface = obj_ref.Surface()

	Dim gp = New GetPoint()
	gp.SetCommandPrompt("Point on surface")
	gp.Constrain(surface, False)
	Dim option_toggle = New OptionToggle(False, "U", "V")
	gp.AddOptionToggle("Direction", option_toggle)
	Dim point As Point3d = Point3d.Unset
	Do
	  Dim grc = gp.Get()
	  If grc Is GetResult.Option Then
		Continue Do
	  ElseIf grc Is GetResult.Point Then
		point = gp.Point()
		Exit Do
	  Else
		Return Result.Nothing
	  End If
	Loop
	If point Is Point3d.Unset Then
	  Return Result.Nothing
	End If

	Dim direction As Integer = If(option_toggle.CurrentValue, 1, 0) ' V : U
	Dim u_parameter As Double = Nothing, v_parameter As Double = Nothing
	If Not surface.ClosestPoint(point, u_parameter, v_parameter) Then
		Return Result.Failure
	End If

	Dim iso_curve = surface.IsoCurve(direction,If(direction = 1, u_parameter, v_parameter))
	If iso_curve Is Nothing Then
		Return Result.Failure
	End If

	doc.Objects.AddCurve(iso_curve)
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
