Partial Friend Class Examples
  Private Shared m_distance As Double = 1
  Private Shared m_point_start As Rhino.Geometry.Point3d
  Public Shared Function ArrayByDistance(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select object", True, Rhino.DocObjects.ObjectType.AnyObject, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	rc = Rhino.Input.RhinoGet.GetPoint("Start point", False, m_point_start)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim obj = objref.Object()
	If obj Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	' create an instance of a GetPoint class and add a delegate
	' for the DynamicDraw event
	Dim gp = New Rhino.Input.Custom.GetPoint()
	gp.DrawLineFromPoint(m_point_start, True)
	Dim optdouble = New Rhino.Input.Custom.OptionDouble(m_distance)
	Dim constrain As Boolean = False
	Dim optconstrain = New Rhino.Input.Custom.OptionToggle(constrain, "Off", "On")
	gp.AddOptionDouble("Distance", optdouble)
	gp.AddOptionToggle("Constrain", optconstrain)
	AddHandler gp.DynamicDraw, AddressOf ArrayByDistanceDraw
	gp.Tag = obj
	Do While gp.Get() = Rhino.Input.GetResult.Option
	  m_distance = optdouble.CurrentValue
	  If constrain <> optconstrain.CurrentValue Then
		constrain = optconstrain.CurrentValue
		If constrain Then
		  Dim gp2 = New Rhino.Input.Custom.GetPoint()
		  gp2.DrawLineFromPoint(m_point_start, True)
		  gp2.SetCommandPrompt("Second point on constraint line")
		  If gp2.Get() = Rhino.Input.GetResult.Point Then
			gp.Constrain(m_point_start, gp2.Point())
		  Else
			gp.ClearCommandOptions()
			optconstrain.CurrentValue = False
			constrain = False
			gp.AddOptionDouble("Distance", optdouble)
			gp.AddOptionToggle("Constrain", optconstrain)
		  End If
		Else
		  gp.ClearConstraints()
		End If
	  End If
	Loop

	If gp.CommandResult() = Rhino.Commands.Result.Success Then
	  m_distance = optdouble.CurrentValue
	  Dim pt = gp.Point()
	  Dim vec = pt - m_point_start
	  Dim length As Double = vec.Length
	  vec.Unitize()
	  Dim count As Integer = CInt(Fix(length / m_distance))
	  For i As Integer = 1 To count - 1
		Dim translate = vec * (i * m_distance)
		Dim xf = Rhino.Geometry.Transform.Translation(translate)
		doc.Objects.Transform(obj, xf, False)
	  Next i
	  doc.Views.Redraw()
	End If

	Return gp.CommandResult()
  End Function

  ' this function is called whenever the GetPoint's DynamicDraw
  ' event occurs
  Private Shared Sub ArrayByDistanceDraw(ByVal sender As Object, ByVal e As Rhino.Input.Custom.GetPointDrawEventArgs)
	Dim rhobj As Rhino.DocObjects.RhinoObject = TryCast(e.Source.Tag, Rhino.DocObjects.RhinoObject)
	If rhobj Is Nothing Then
	  Return
	End If
	Dim vec = e.CurrentPoint - m_point_start
	Dim length As Double = vec.Length
	vec.Unitize()
	Dim count As Integer = CInt(Fix(length / m_distance))
	For i As Integer = 1 To count - 1
	  Dim translate = vec * (i * m_distance)
	  Dim xf = Rhino.Geometry.Transform.Translation(translate)
	  e.Display.DrawObject(rhobj, xf)
	Next i
  End Sub
End Class
