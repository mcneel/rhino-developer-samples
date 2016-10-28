Partial Friend Class Examples
  Public Shared Function SetViewName(ByVal doc As RhinoDoc) As Result
	Dim view = doc.Views.ActiveView
	If view Is Nothing Then
	  Return Result.Failure
	End If

	view.MainViewport.Name = "Facade"
	Return Result.Success
  End Function
End Class
