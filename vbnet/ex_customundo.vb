Imports System.Runtime.InteropServices
Imports Rhino

<Guid("A6924FE1-2B94-4918-94F3-B8935B8DC80C")> _
Public Class ex_customundoCommand
  Inherits Rhino.Commands.Command
  Public Overrides ReadOnly Property EnglishName() As String
    Get
      Return "vb_CustomUndoCommand"
    End Get
  End Property

  Private Property MyFavoriteNumber() As Double
    Get
      Return m_MyFavoriteNumber
    End Get
    Set(value As Double)
      m_MyFavoriteNumber = value
    End Set
  End Property
  Private m_MyFavoriteNumber As Double

  Protected Overrides Function RunCommand(doc As RhinoDoc, mode As Rhino.Commands.RunMode) As Rhino.Commands.Result
    ' Rhino automatically sets up an undo record when a command is run,
    ' but... the undo record is not saved if nothing changes in the
    ' document (objects added/deleted, layers changed,...)
    '
    ' If we have a command that doesn't change things in the document,
    ' but we want to have our own custom undo called then we need to do
    ' a little extra work

    Dim d As Double = MyFavoriteNumber
    If Rhino.Input.RhinoGet.GetNumber("Favorite number", True, d) = Rhino.Commands.Result.Success Then
      Dim current_value As Double = MyFavoriteNumber
      doc.AddCustomUndoEvent("Favorite Number", AddressOf OnUndoFavoriteNumber, current_value)
      MyFavoriteNumber = d
    End If
    Return Rhino.Commands.Result.Success
  End Function

  ' event handler for custom undo
  Private Sub OnUndoFavoriteNumber(sender As Object, e As Rhino.Commands.CustomUndoEventArgs)
    ' !!!!!!!!!!
    ' NEVER change any setting in the Rhino document or application.  Rhino
    ' handles ALL changes to the application and document and you will break
    ' the Undo/Redo commands if you make any changes to the application or
    ' document. This is meant only for your own private plug-in data
    ' !!!!!!!!!!

    ' This function can be called either by undo or redo
    ' In order to get redo to work, add another custom undo event with the
    ' current value.  If you don't want redo to work, just skip adding
    ' a custom undo event here
    Dim current_value As Double = MyFavoriteNumber
    e.Document.AddCustomUndoEvent("Favorite Number", AddressOf OnUndoFavoriteNumber, current_value)

    Dim old_value As Double = CDbl(e.Tag)
    RhinoApp.WriteLine("Going back to your favorite = {0}", old_value)
    MyFavoriteNumber = old_value
  End Sub
End Class
