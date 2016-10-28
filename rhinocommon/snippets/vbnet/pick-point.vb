Partial Friend Class Examples
  Public Shared Function PickPoint(ByVal doc As RhinoDoc) As Result
	' this creates a point where the mouse is clicked.
	Dim gp = New GetPoint()
	gp.SetCommandPrompt("Click for new point")
	gp.Get()
	If gp.CommandResult() <> Result.Success Then
	  Return gp.CommandResult()
	End If
	Dim point3d = gp.Point()
	doc.Objects.AddPoint(point3d)
	doc.Views.Redraw()

	' selects a point that already exists
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select point", False, ObjectType.Point, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim point = obj_ref.Point()
	RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}", point.Location.X, point.Location.Y, point.Location.Z)
	doc.Objects.UnselectAll()

	' selects multiple points that already exist
	Dim obj_refs() As ObjRef = Nothing
	rc = RhinoGet.GetMultipleObjects("Select point", False, ObjectType.Point, obj_refs)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	For Each o_ref In obj_refs
	  point = o_ref.Point()
	  RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}", point.Location.X, point.Location.Y, point.Location.Z)
	Next o_ref
	doc.Objects.UnselectAll()

	' also selects a point that already exists.
	' Used when RhinoGet doesn't provide enough control
	Dim go = New GetObject()
	go.SetCommandPrompt("Select point")
	go.GeometryFilter = ObjectType.Point
	go.GetMultiple(1, 0)
	If go.CommandResult() <> Result.Success Then
	  Return go.CommandResult()
	End If
	For Each o_ref In go.Objects()
	  point = o_ref.Point()
	  If point IsNot Nothing Then
		RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}", point.Location.X, point.Location.Y, point.Location.Z)
	  End If
	Next o_ref

	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
