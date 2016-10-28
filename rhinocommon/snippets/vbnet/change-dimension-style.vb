Partial Friend Class Examples
  Public Shared Function ChangeDimensionStyle(ByVal doc As RhinoDoc) As Result
	For Each rhino_object In doc.Objects.GetObjectList(ObjectType.Annotation)
	  Dim annotation_object = TryCast(rhino_object, AnnotationObjectBase)
	  If annotation_object Is Nothing Then
		  Continue For
	  End If

	  Dim annotation = TryCast(annotation_object.Geometry, AnnotationBase)
	  If annotation Is Nothing Then
		  Continue For
	  End If

	  If annotation.Index = doc.DimStyles.CurrentDimensionStyleIndex Then
		  Continue For
	  End If

	  annotation.Index = doc.DimStyles.CurrentDimensionStyleIndex
	  annotation_object.CommitChanges()
	Next rhino_object

	doc.Views.Redraw()

	Return Result.Success
  End Function
End Class
