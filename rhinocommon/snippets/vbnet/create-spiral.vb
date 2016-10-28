Partial Friend Class Examples
  Public Shared Function CreateSpiral(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim axisStart = New Rhino.Geometry.Point3d(0, 0, 0)
	Dim axisDir = New Rhino.Geometry.Vector3d(1, 0, 0)
	Dim radiusPoint = New Rhino.Geometry.Point3d(0, 1, 0)

	Dim curve0 As Rhino.Geometry.NurbsCurve = GetSpirial0()
	If Nothing IsNot curve0 Then
	  doc.Objects.AddCurve(curve0)
	End If

	Dim curve1 As Rhino.Geometry.NurbsCurve = GetSpirial1()
	If Nothing IsNot curve1 Then
	  doc.Objects.AddCurve(curve1)
	End If

	doc.Views.Redraw()

	Return Rhino.Commands.Result.Success
  End Function

  Private Shared Function GetSpirial0() As Rhino.Geometry.NurbsCurve
	Dim axisStart = New Rhino.Geometry.Point3d(0, 0, 0)
	Dim axisDir = New Rhino.Geometry.Vector3d(1, 0, 0)
	Dim radiusPoint = New Rhino.Geometry.Point3d(0, 1, 0)

	Return Rhino.Geometry.NurbsCurve.CreateSpiral(axisStart, axisDir, radiusPoint, 1, 10, 1.0, 1.0)
  End Function

  Private Shared Function GetSpirial1() As Rhino.Geometry.NurbsCurve
	Dim railStart = New Rhino.Geometry.Point3d(0, 0, 0)
	Dim railEnd = New Rhino.Geometry.Point3d(0, 0, 10)
	Dim railCurve = New Rhino.Geometry.LineCurve(railStart, railEnd)

	Dim t0 As Double = railCurve.Domain.Min
	Dim t1 As Double = railCurve.Domain.Max

	Dim radiusPoint = New Rhino.Geometry.Point3d(1, 0, 0)

	Return Rhino.Geometry.NurbsCurve.CreateSpiral(railCurve, t0, t1, radiusPoint, 1, 10, 1.0, 1.0, 12)
  End Function
End Class
