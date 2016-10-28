Partial Friend Class Examples
  Public Shared Function FindObjectsByName(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Const name As String = "abc"
	Dim settings As New Rhino.DocObjects.ObjectEnumeratorSettings()
	settings.NameFilter = name
	Dim ids As New System.Collections.Generic.List(Of Guid)()
	For Each rhObj As Rhino.DocObjects.RhinoObject In doc.Objects.GetObjectList(settings)
	  ids.Add(rhObj.Id)
	Next rhObj

	If ids.Count = 0 Then
	  Rhino.RhinoApp.WriteLine("No objects with the name " & name)
	  Return Rhino.Commands.Result.Failure
	End If

	Rhino.RhinoApp.WriteLine("Found {0} objects", ids.Count)
	For Each id As Guid In ids
	  Rhino.RhinoApp.WriteLine("  {0}", id)
	Next id

	Return Rhino.Commands.Result.Success
  End Function
End Class
