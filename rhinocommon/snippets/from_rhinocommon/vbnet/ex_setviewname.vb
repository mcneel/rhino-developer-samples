Imports Rhino
Imports Rhino.Commands

Namespace examples_vb
  Public Class SetViewNameCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbSetViewName"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim view As Rhino.Display.RhinoView = doc.Views.ActiveView
      If view Is Nothing Then
        Return Rhino.Commands.Result.Failure
      End If

      view.MainViewport.Name = "Facade"
      Return Result.Success
    End Function
  End Class
End Namespace