Imports Rhino
Imports Rhino.Commands
Imports Rhino.Geometry

Namespace examples_vb
  Public Class TextJustifyCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbTextJustify"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim text_entity = New TextEntity()
      text_entity.Plane = Plane.WorldXY
      text_entity.Text = "Hello Rhino!"
      text_entity.Justification = TextJustification.MiddleCenter
      text_entity.FontIndex = doc.Fonts.FindOrCreate("Arial", False, False)

      doc.Objects.AddText(text_entity)
      doc.Views.Redraw()

      Return Result.Success
    End Function
  End Class
End Namespace