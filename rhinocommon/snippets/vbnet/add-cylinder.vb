Partial Friend Class Examples
  Public Shared Function AddCylinder(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim center_point As New Rhino.Geometry.Point3d(0, 0, 0)
	Dim height_point As New Rhino.Geometry.Point3d(0, 0, 10)
	Dim zaxis As Rhino.Geometry.Vector3d = height_point - center_point
	Dim plane As New Rhino.Geometry.Plane(center_point, zaxis)
	Const radius As Double = 5
	Dim circle As New Rhino.Geometry.Circle(plane, radius)
	Dim cylinder As New Rhino.Geometry.Cylinder(circle, zaxis.Length)
	Dim brep As Rhino.Geometry.Brep = cylinder.ToBrep(True, True)
	If brep IsNot Nothing Then
	  doc.Objects.AddBrep(brep)
	  doc.Views.Redraw()
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
