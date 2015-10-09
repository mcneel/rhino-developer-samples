Partial Class Examples
  Public Shared Function FindObjectsByName(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Const name As String = "abc"
    Dim settings As New Rhino.DocObjects.ObjectEnumeratorSettings()
    settings.NameFilter = name
    Dim ids As New System.Collections.Generic.List(Of Guid)()
    For Each rhObj As Rhino.DocObjects.RhinoObject In doc.Objects.GetObjectList(settings)
      ids.Add(rhObj.Id)
    Next

    If ids.Count = 0 Then
      Rhino.RhinoApp.WriteLine("No objects with the name " & name)
      Return Rhino.Commands.Result.Failure
    Else
      Rhino.RhinoApp.WriteLine("Found {0} objects", ids.Count)
      For i As Integer = 0 To ids.Count - 1
        Rhino.RhinoApp.WriteLine("  {0}", ids(i))
      Next
    End If

    Return Rhino.Commands.Result.Success
  End Function
End Class
