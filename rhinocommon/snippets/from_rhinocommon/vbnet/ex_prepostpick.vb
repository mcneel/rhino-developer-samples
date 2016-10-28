Imports System.Linq
Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class PrePostPickCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbPrePostPick"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim go = New Rhino.Input.Custom.GetObject()
      go.SetCommandPrompt("Select objects")
      go.EnablePreSelect(True, True)
      go.EnablePostSelect(True)
      go.GetMultiple(0, 0)
      If go.CommandResult() <> Result.Success Then
        Return go.CommandResult()
      End If

      Dim selected_objects = go.Objects().ToList()

      If go.ObjectsWerePreselected Then
        go.EnablePreSelect(False, True)
        go.DeselectAllBeforePostSelect = False
        go.EnableUnselectObjectsOnExit(False)
        go.GetMultiple(0, 0)
        If go.CommandResult() = Result.Success Then
          For Each obj As ObjRef In go.Objects()
            selected_objects.Add(obj)
            ' The normal behavior of commands is that when they finish,
            ' objects that were pre-selected remain selected and objects
            ' that were post-selected will not be selected. Because we
            ' potentially could have both, we'll try to do something
            ' consistent and make sure post-selected objects also stay selected
            obj.[Object]().[Select](True)
          Next
        End If
      End If
      Return If(selected_objects.Count > 0, Result.Success, Result.[Nothing])
    End Function
  End Class
End Namespace