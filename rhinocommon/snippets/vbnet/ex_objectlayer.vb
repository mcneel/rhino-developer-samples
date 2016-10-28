Partial Class Examples
  Public Shared Function DetermineObjectLayer(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim obref As Rhino.DocObjects.ObjRef = Nothing
    Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select object", True, Rhino.DocObjects.ObjectType.AnyObject, obref)
    If rc <> Rhino.Commands.Result.Success Then
      Return rc
    End If
    Dim rhobj As Rhino.DocObjects.RhinoObject = obref.[Object]()
    If rhobj Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If
    Dim index As Integer = rhobj.Attributes.LayerIndex
    Dim name As String = doc.Layers(index).Name
    Rhino.RhinoApp.WriteLine("The selected object's layer is '{0}'", name)
    Return Rhino.Commands.Result.Success
  End Function
End Class
