Partial Class Examples
  Public Shared Function EditText(doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim objref As Rhino.DocObjects.ObjRef = Nothing
    Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select text", False, Rhino.DocObjects.ObjectType.Annotation, objref)
    If rc <> Rhino.Commands.Result.Success OrElse objref Is Nothing Then
      Return rc
    End If

    Dim textobj As Rhino.DocObjects.TextObject = TryCast(objref.Object(), Rhino.DocObjects.TextObject)
    If textobj Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If

    Dim textentity As Rhino.Geometry.TextEntity = TryCast(textobj.Geometry, Rhino.Geometry.TextEntity)
    Dim str As String = textentity.Text
    rc = Rhino.Input.RhinoGet.GetString("New text", False, str)
    If rc <> Rhino.Commands.Result.Success OrElse objref Is Nothing Then
      Return rc
    End If

    textentity.Text = str
    textobj.CommitChanges()
    doc.Views.Redraw()
    Return Rhino.Commands.Result.Success
  End Function
End Class
