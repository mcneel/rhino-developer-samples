Partial Class Examples
  Public Shared Function AddNurbsCircle(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' The easy way to get a NURBS curve from a circle is with
    ' the following two lines of code.
    '
    ' Dim c As New Rhino.Geometry.Circle(20)
    ' Dim nc As Rhino.Geometry.NurbsCurve = c.ToNurbsCurve()
    '
    ' This sample demonstrates creating a NURBS curve from scratch.
    Const dimension As Integer = 3
    Const isRational As Boolean = True
    Const order As Integer = 3
    Const cv_count As Integer = 9
    Dim nc As New Rhino.Geometry.NurbsCurve(dimension, isRational, order, cv_count)
    nc.Points.SetPoint(0, 1.0, 0.0, 0.0, 1.0)
    nc.Points.SetPoint(1, 0.707107, 0.707107, 0.0, 0.707107)
    nc.Points.SetPoint(2, 0.0, 1.0, 0.0, 1.0)
    nc.Points.SetPoint(3, -0.707107, 0.707107, 0.0, 0.707107)
    nc.Points.SetPoint(4, -1.0, 0.0, 0.0, 1.0)
    nc.Points.SetPoint(5, -0.707107, -0.707107, 0.0, 0.707107)
    nc.Points.SetPoint(6, 0.0, -1.0, 0.0, 1.0)
    nc.Points.SetPoint(7, 0.707107, -0.707107, 0.0, 0.707107)
    nc.Points.SetPoint(8, 1.0, 0.0, 0.0, 1.0)
    nc.Knots(0) = 0.0
    nc.Knots(1) = 0.0
    nc.Knots(2) = 0.5 * Math.PI
    nc.Knots(3) = 0.5 * Math.PI
    nc.Knots(4) = Math.PI
    nc.Knots(5) = Math.PI
    nc.Knots(6) = 1.5 * Math.PI
    nc.Knots(7) = 1.5 * Math.PI
    nc.Knots(8) = 2.0 * Math.PI
    nc.Knots(9) = 2.0 * Math.PI
    If nc.IsValid Then
      doc.Objects.AddCurve(nc)
      doc.Views.Redraw()
      Return Rhino.Commands.Result.Success
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
