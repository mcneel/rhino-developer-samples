Partial Friend Class Examples
  Public Shared Function DrawString(ByVal doc As RhinoDoc) As Result
	Dim gp = New GetDrawStringPoint()
	gp.SetCommandPrompt("Point")
	gp.Get()
	Return gp.CommandResult()
  End Function
End Class

Public Class GetDrawStringPoint
	Inherits GetPoint

  Protected Overrides Sub OnDynamicDraw(ByVal e As GetPointDrawEventArgs)
	MyBase.OnDynamicDraw(e)
	Dim xform = e.Viewport.GetTransform(CoordinateSystem.World, CoordinateSystem.Screen)
	Dim current_point = e.CurrentPoint
	current_point.Transform(xform)
	Dim screen_point = New Point2d(current_point.X, current_point.Y)
	Dim msg = String.Format("screen {0:F}, {1:F}", current_point.X, current_point.Y)
	e.Display.Draw2dText(msg, System.Drawing.Color.Blue, screen_point, False)
  End Sub
End Class
