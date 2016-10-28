Partial Friend Class Examples
  Public Shared Function DivideByLengthPoints(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Const filter As ObjectType = Rhino.DocObjects.ObjectType.Curve
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select curve to divide", False, filter, objref)
	If rc IsNot Rhino.Commands.Result.Success OrElse objref Is Nothing Then
	  Return rc
	End If

	Dim crv As Rhino.Geometry.Curve = objref.Curve()
	If crv Is Nothing OrElse crv.IsShort(Rhino.RhinoMath.ZeroTolerance) Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim crv_length As Double = crv.GetLength()
	Dim s As String = String.Format("Curve length is {0:f3}. Segment length", crv_length)

	Dim seg_length As Double = crv_length / 2.0
	rc = Rhino.Input.RhinoGet.GetNumber(s, False, seg_length, 0, crv_length)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim points() As Rhino.Geometry.Point3d = Nothing
	crv.DivideByLength(seg_length, True, points)
	If points Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	For Each point As Rhino.Geometry.Point3d In points
	  doc.Objects.AddPoint(point)
	Next point

	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
