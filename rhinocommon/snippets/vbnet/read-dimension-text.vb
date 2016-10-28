Partial Friend Class Examples
  Public Shared Function ReadDimensionText(ByVal doc As RhinoDoc) As Result
	Dim go = New GetObject()
	go.SetCommandPrompt("Select annotation")
	go.GeometryFilter = ObjectType.Annotation
	go.Get()
	If go.CommandResult() <> Result.Success Then
	  Return Result.Failure
	End If
	Dim annotation = TryCast(go.Object(0).Object(), AnnotationObjectBase)
	If annotation Is Nothing Then
	  Return Result.Failure
	End If

	RhinoApp.WriteLine("Annotation text = {0}", annotation.DisplayText)

	Return Result.Success
  End Function
End Class
