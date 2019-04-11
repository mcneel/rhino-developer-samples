Imports Rhino
Imports Rhino.Commands

Namespace SampleVbWinForms

  Public Class SampleVbWinFormsCommand
    Inherits Command

    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "SampleVbWinForms"
      End Get
    End Property

    Protected Overrides Function RunCommand(ByVal doc As RhinoDoc, ByVal mode As RunMode) As Result
      Return Result.Success
    End Function
  End Class

End Namespace