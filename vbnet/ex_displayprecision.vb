Imports Rhino
Imports Rhino.Input.Custom
Imports Rhino.Commands

Namespace examples_vb
  Public Class DisplayPrecisionCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDisplayPrecision"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim gi = New GetInteger()
      gi.SetCommandPrompt("New display precision")
      gi.SetDefaultInteger(doc.ModelDistanceDisplayPrecision)
      gi.SetLowerLimit(0, False)
      gi.SetUpperLimit(7, False)
      gi.[Get]()
      If gi.CommandResult() <> Result.Success Then
        Return gi.CommandResult()
      End If
      Dim distance_display_precision = gi.Number()

      If distance_display_precision <> doc.ModelDistanceDisplayPrecision Then
        doc.ModelDistanceDisplayPrecision = distance_display_precision
      End If

      Return Result.Success
    End Function
  End Class
End Namespace