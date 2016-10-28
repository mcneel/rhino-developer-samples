Partial Friend Class Examples
  Public Shared Function IntersectLineCircle(ByVal doc As RhinoDoc) As Result
	Dim circle As Circle = Nothing
	Dim rc = RhinoGet.GetCircle(circle)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	doc.Objects.AddCircle(circle)
	doc.Views.Redraw()

	Dim line As Line = Nothing
	rc = RhinoGet.GetLine(line)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	doc.Objects.AddLine(line)
	doc.Views.Redraw()

	Dim t1 As Double = Nothing, t2 As Double = Nothing
	Dim point1 As Point3d = Nothing, point2 As Point3d = Nothing
	Dim line_circle_intersect = Intersection.LineCircle(line, circle, t1, point1, t2, point2)
	Dim msg As String = ""
	Select Case line_circle_intersect
	  Case LineCircleIntersection.None
		msg = "line does not intersect circle"
	  Case LineCircleIntersection.Single
		msg = String.Format("line intersects circle at point ({0})", point1)
		doc.Objects.AddPoint(point1)
	  Case LineCircleIntersection.Multiple
		msg = String.Format("line intersects circle at points ({0}) and ({1})", point1, point2)
		doc.Objects.AddPoint(point1)
		doc.Objects.AddPoint(point2)
	End Select
	RhinoApp.WriteLine(msg)
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
