Partial Friend Class Examples
  Public Shared Function TextJustify(ByVal doc As RhinoDoc) As Result
	Dim text_entity = New TextEntity With {.Plane = Plane.WorldXY, .Text = "Hello Rhino!", .Justification = TextJustification.MiddleCenter, .FontIndex = doc.Fonts.FindOrCreate("Arial", False, False)}

	doc.Objects.AddText(text_entity)
	doc.Views.Redraw()

	Return Result.Success
  End Function
End Class
