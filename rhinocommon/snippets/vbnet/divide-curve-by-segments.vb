Partial Friend Class Examples
  Public Shared Function DivideCurveBySegments(ByVal doc As RhinoDoc) As Result
	Const filter As ObjectType = ObjectType.Curve
	Dim objref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select curve to divide", False, filter, objref)
	If rc IsNot Result.Success OrElse objref Is Nothing Then
	  Return rc
	End If

	Dim curve = objref.Curve()
	If curve Is Nothing OrElse curve.IsShort(RhinoMath.ZeroTolerance) Then
	  Return Result.Failure
	End If

	Dim segment_count = 2
	rc = RhinoGet.GetInteger("Divide curve into how many segments?", False, segment_count)
	If rc IsNot Result.Success Then
	  Return rc
	End If

	Dim points() As Point3d = Nothing
	curve.DivideByCount(segment_count, True, points)
	If points Is Nothing Then
	  Return Result.Failure
	End If

	For Each point In points
	  doc.Objects.AddPoint(point)
	Next point

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
