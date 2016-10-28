Partial Friend Class Examples
  Public Shared Function SurfaceFromCorners(ByVal doc As RhinoDoc) As Result
	Dim surface = NurbsSurface.CreateFromCorners(New Point3d(5, 0, 0), New Point3d(5, 5, 5), New Point3d(0, 5, 0), New Point3d(0, 0, 0))

	doc.Objects.AddSurface(surface)
	doc.Views.Redraw()

	Return Rhino.Commands.Result.Success
  End Function
End Class
