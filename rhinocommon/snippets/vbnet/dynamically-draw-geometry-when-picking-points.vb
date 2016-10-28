Partial Friend Class Examples
  Public Shared Function GetPointDynamicDraw(ByVal doc As RhinoDoc) As Result
	Dim gp = New GetPoint()
	gp.SetCommandPrompt("Center point")
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim center_point = gp.Point()
	If center_point Is Point3d.Unset Then
	  Return Result.Failure
	End If

	Dim gcp = New GetCircleRadiusPoint(center_point)
	gcp.SetCommandPrompt("Radius")
	gcp.ConstrainToConstructionPlane(False)
	gcp.SetBasePoint(center_point, True)
	gcp.DrawLineFromPoint(center_point, True)
	gcp.Get()
	If gcp.CommandResult() <> Result.Success Then
	  Return gcp.CommandResult()
	End If

	Dim radius = center_point.DistanceTo(gcp.Point())
	Dim cplane = doc.Views.ActiveView.ActiveViewport.ConstructionPlane()
	doc.Objects.AddCircle(New Circle(cplane, center_point, radius))
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class

Public Class GetCircleRadiusPoint
	Inherits GetPoint

  Private m_center_point As Point3d

  Public Sub New(ByVal centerPoint As Point3d)
	m_center_point = centerPoint
  End Sub

  Protected Overrides Sub OnDynamicDraw(ByVal e As GetPointDrawEventArgs)
	MyBase.OnDynamicDraw(e)
	Dim cplane = e.RhinoDoc.Views.ActiveView.ActiveViewport.ConstructionPlane()
	Dim radius = m_center_point.DistanceTo(e.CurrentPoint)
	Dim circle = New Circle(cplane, m_center_point, radius)
	e.Display.DrawCircle(circle, System.Drawing.Color.Black)
  End Sub
End Class
