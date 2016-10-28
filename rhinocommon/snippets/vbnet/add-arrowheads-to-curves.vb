Partial Friend Class Examples
  Public Shared Function ObjectDecoration(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	' Define a line
	Dim line = New Rhino.Geometry.Line(New Rhino.Geometry.Point3d(0, 0, 0), New Rhino.Geometry.Point3d(10, 0, 0))

	' Make a copy of Rhino's default object attributes
	Dim attribs = doc.CreateDefaultAttributes()

	' Modify the object decoration style
	attribs.ObjectDecoration = Rhino.DocObjects.ObjectDecoration.BothArrowhead

	' Create a new curve object with our attributes
	doc.Objects.AddLine(line, attribs)
	doc.Views.Redraw()

	Return Rhino.Commands.Result.Success
  End Function
End Class
