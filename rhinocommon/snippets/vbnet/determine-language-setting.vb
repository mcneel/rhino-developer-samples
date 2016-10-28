Partial Friend Class Examples
  Public Shared Function DetermineCurrentLanguage(ByVal doc As RhinoDoc) As Result
	Dim language_id = Rhino.ApplicationSettings.AppearanceSettings.LanguageIdentifier
	Dim culture = New System.Globalization.CultureInfo(language_id)
	RhinoApp.WriteLine("The current language is {0}", culture.EnglishName)
	Return Result.Success
  End Function
End Class
