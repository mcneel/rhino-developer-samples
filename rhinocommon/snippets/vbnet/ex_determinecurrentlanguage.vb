Imports Rhino
Imports Rhino.Commands

Namespace examples_vb
  Public Class DetermineCurrentLanguageCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbCurrentLanguage"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim language_id = Rhino.ApplicationSettings.AppearanceSettings.LanguageIdentifier
      Dim culture = New System.Globalization.CultureInfo(language_id)
      RhinoApp.WriteLine("The current language is {0}", culture.EnglishName)
      Return Result.Success
    End Function
  End Class
End Namespace