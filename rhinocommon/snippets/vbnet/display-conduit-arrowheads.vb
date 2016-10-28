Partial Friend Class Examples
  Private Shared m_draw_conduit As DrawArrowHeadsConduit

  Public Shared Function ConduitArrowHeads(ByVal doc As RhinoDoc) As Result
	If m_draw_conduit IsNot Nothing Then
	  RhinoApp.WriteLine("Turn off existing arrowhead conduit")
	  m_draw_conduit.Enabled = False
	  m_draw_conduit = Nothing
	Else
	  ' get arrow head size
	  Dim go = New GetOption()
	  go.SetCommandPrompt("ArrowHead length in screen size (pixels) or world size (percentage of arrow length)?")
	  go.AddOption("screen")
	  go.AddOption("world")
	  go.Get()
	  If go.CommandResult() <> Result.Success Then
		Return go.CommandResult()
	  End If

	  Dim screen_size As Integer = 0
	  Dim world_size As Double = 0.0
	  If go.Option().EnglishName = "screen" Then
		Dim gi = New GetInteger()
		gi.SetLowerLimit(0, True)
		gi.SetCommandPrompt("Length of arrow head in pixels")
		gi.Get()
		If gi.CommandResult() <> Result.Success Then
		  Return gi.CommandResult()
		End If
		screen_size = gi.Number()
	  Else
		Dim gi = New GetInteger()
		gi.SetLowerLimit(0, True)
		gi.SetUpperLimit(100, False)
		gi.SetCommandPrompt("Length of arrow head in percentage of total arrow length")
		gi.Get()
		If gi.CommandResult() <> Result.Success Then
		  Return gi.CommandResult()
		End If
'INSTANT VB WARNING: Instant VB cannot determine whether both operands of this division are integer types - if they are then you should use the VB integer division operator:
		world_size = gi.Number() / 100.0
	  End If


	  ' get arrow start and end points
	  Dim gp = New GetPoint()
	  gp.SetCommandPrompt("Start of line")
	  gp.Get()
	  If gp.CommandResult() <> Result.Success Then
		Return gp.CommandResult()
	  End If
	  Dim start_point = gp.Point()

	  gp.SetCommandPrompt("End of line")
	  gp.SetBasePoint(start_point, False)
	  gp.DrawLineFromPoint(start_point, True)
	  gp.Get()
	  If gp.CommandResult() <> Result.Success Then
		Return gp.CommandResult()
	  End If
	  Dim end_point = gp.Point()

	  Dim v = end_point - start_point
	  If v.IsTiny(Rhino.RhinoMath.ZeroTolerance) Then
		Return Result.Nothing
	  End If

	  Dim line = New Line(start_point, end_point)

	  m_draw_conduit = New DrawArrowHeadsConduit(line, screen_size, world_size)
	  ' toggle conduit on/off
	  m_draw_conduit.Enabled = True
	  RhinoApp.WriteLine("Draw arrowheads conduit enabled.")
	End If
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
