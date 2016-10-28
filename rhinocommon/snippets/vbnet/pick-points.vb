Partial Friend Class Examples
  Private Shared ReadOnly m_conduit_points As New List(Of ConduitPoint)()

  Public Shared Function PickPoints(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	Dim conduit = New PointsConduit(m_conduit_points)
	conduit.Enabled = True

	Dim gp = New Rhino.Input.Custom.GetPoint()
	Do
	  gp.SetCommandPrompt("click location to create point. (<ESC> exit)")
	  gp.AcceptNothing(True)
	  gp.Get()
	  If gp.CommandResult() <> Rhino.Commands.Result.Success Then
		Exit Do
	  End If
	  m_conduit_points.Add(New ConduitPoint(gp.Point()))
	  doc.Views.Redraw()
	Loop

	Dim gcp = New GetConduitPoint(m_conduit_points)
	Do
	  gcp.SetCommandPrompt("select conduit point. (<ESC> to exit)")
	  gcp.AcceptNothing(True)
	  gcp.Get(True)
	  doc.Views.Redraw()
	  If gcp.CommandResult() <> Rhino.Commands.Result.Success Then
		Exit Do
	  End If
	Loop

	Return Rhino.Commands.Result.Success
  End Function
End Class

Public Class ConduitPoint
  Public Sub New(ByVal point As Point3d)
	Color = System.Drawing.Color.White
	Me.Point = point
  End Sub
  Public Property Color() As System.Drawing.Color
  Public Property Point() As Point3d
End Class

Public Class GetConduitPoint
	Inherits GetPoint

  Private ReadOnly m_conduit_points As List(Of ConduitPoint)

  Public Sub New(ByVal conduitPoints As List(Of ConduitPoint))
	m_conduit_points = conduitPoints
  End Sub

  Protected Overrides Sub OnMouseDown(ByVal e As GetPointMouseEventArgs)
	MyBase.OnMouseDown(e)
	Dim picker = New PickContext()
	picker.View = e.Viewport.ParentView

	picker.PickStyle = PickStyle.PointPick

	Dim xform = e.Viewport.GetPickTransform(e.WindowPoint)
	picker.SetPickTransform(xform)

	For Each cp In m_conduit_points
	  Dim depth As Double = Nothing
	  Dim distance As Double = Nothing
	  If picker.PickFrustumTest(cp.Point, depth, distance) Then
		cp.Color = System.Drawing.Color.Red
	  Else
		cp.Color = System.Drawing.Color.White
	  End If
	Next cp
  End Sub
End Class

Friend Class PointsConduit
	Inherits Rhino.Display.DisplayConduit

  Private ReadOnly m_conduit_points As List(Of ConduitPoint)

  Public Sub New(ByVal conduitPoints As List(Of ConduitPoint))
	m_conduit_points = conduitPoints
  End Sub

  Protected Overrides Sub DrawForeground(ByVal e As Rhino.Display.DrawEventArgs)
	If m_conduit_points IsNot Nothing Then
	  For Each cp In m_conduit_points
	  e.Display.DrawPoint(cp.Point, PointStyle.Simple, 3, cp.Color)
	  Next cp
	End If
  End Sub
End Class
