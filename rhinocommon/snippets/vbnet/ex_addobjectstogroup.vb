Imports System.Collections.Generic

Partial Class Examples
  Public Shared Function AddObjectsToGroup(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim go As New Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select objects to group")
    go.GroupSelect = True
    go.GetMultiple(1, 0)
    If go.CommandResult() <> Rhino.Commands.Result.Success Then
      Return go.CommandResult()
    End If

    Dim ids As New List(Of Guid)()
    For i As Integer = 0 To go.ObjectCount - 1
      ids.Add(go.[Object](i).ObjectId)
    Next
    Dim index As Integer = doc.Groups.Add(ids)
    doc.Views.Redraw()
    If index >= 0 Then
      Return Rhino.Commands.Result.Success
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
