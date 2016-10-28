Imports Rhino
Imports Rhino.ApplicationSettings
Imports Rhino.Commands
Imports Rhino.Input.Custom

Namespace examples_vb
  Public Class OrthoCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbOrtho"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim gp = New GetPoint()
      gp.SetCommandPrompt("Start of line")
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If
      Dim start_point = gp.Point()

      Dim original_ortho = ModelAidSettings.Ortho
      If Not original_ortho Then
        ModelAidSettings.Ortho = True
      End If

      gp.SetCommandPrompt("End of line")
      gp.SetBasePoint(start_point, False)
      gp.DrawLineFromPoint(start_point, True)
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If
      Dim end_point = gp.Point()

      If ModelAidSettings.Ortho <> original_ortho Then
        ModelAidSettings.Ortho = original_ortho
      End If

      doc.Objects.AddLine(start_point, end_point)
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace