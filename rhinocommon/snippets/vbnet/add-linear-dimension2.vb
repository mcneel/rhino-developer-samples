Partial Friend Class Examples
  Public Shared Function AddLinearDimension2(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim origin As New Point3d(1,1,0)
	Dim offset As New Point3d(11,1,0)
'INSTANT VB WARNING: Instant VB cannot determine whether both operands of this division are integer types - if they are then you should use the VB integer division operator:
	Dim pt As New Point3d((offset.X-origin.X)/2,3,0)

	Dim plane As Plane = Plane.WorldXY
	plane.Origin = origin

	Dim u As Double = Nothing, v As Double = Nothing
	plane.ClosestParameter(origin, u, v)
	Dim ext1 As New Point2d(u, v)

	plane.ClosestParameter(offset, u, v)
	Dim ext2 As New Point2d(u, v)

	plane.ClosestParameter(pt, u, v)
	Dim linePt As New Point2d(u, v)

	Dim dimension As New LinearDimension(plane, ext1, ext2, linePt)
	If doc.Objects.AddLinearDimension(dimension) <> Guid.Empty Then
	  doc.Views.Redraw()
	  Return Rhino.Commands.Result.Success
	End If
	Return Rhino.Commands.Result.Failure
  End Function
End Class
