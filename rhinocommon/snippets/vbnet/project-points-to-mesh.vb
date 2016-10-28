Partial Friend Class Examples
  Public Shared Function ProjectPointsToMeshesEx(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("mesh", False, ObjectType.Mesh, obj_ref)
	If rc IsNot Result.Success Then
		Return rc
	End If
	Dim mesh = obj_ref.Mesh()

	Dim obj_ref_pts() As ObjRef = Nothing
	rc = RhinoGet.GetMultipleObjects("points", False, ObjectType.Point, obj_ref_pts)
	If rc IsNot Result.Success Then
		Return rc
	End If
	Dim points = New List(Of Point3d)()
	For Each obj_ref_pt In obj_ref_pts
	  Dim pt = obj_ref_pt.Point().Location
	  points.Add(pt)
	Next obj_ref_pt

	Dim indices() As Integer = Nothing
	Dim prj_points = Intersection.ProjectPointsToMeshesEx( {mesh}, points, New Vector3d(0, 1, 0), 0, indices)
	For Each prj_pt In prj_points
		doc.Objects.AddPoint(prj_pt)
	Next prj_pt
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
