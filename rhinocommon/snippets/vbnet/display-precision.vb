Partial Friend Class Examples
  Public Shared Function DisplayPrecision(ByVal doc As RhinoDoc) As Result
	Dim gi = New GetInteger()
	gi.SetCommandPrompt("New display precision")
	gi.SetDefaultInteger(doc.ModelDistanceDisplayPrecision)
	gi.SetLowerLimit(0, False)
	gi.SetUpperLimit(7, False)
	gi.Get()
	If gi.CommandResult() <> Result.Success Then
	  Return gi.CommandResult()
	End If
	Dim distance_display_precision = gi.Number()

	If distance_display_precision IsNot doc.ModelDistanceDisplayPrecision Then
	  doc.ModelDistanceDisplayPrecision = distance_display_precision
	End If

	Return Result.Success
  End Function
End Class
