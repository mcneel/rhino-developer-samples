Partial Friend Class Examples
  Public Shared Function SingleColorBackfaces(ByVal doc As RhinoDoc) As Result
	Dim display_mode_descs = From dm In DisplayModeDescription.GetDisplayModes()
	                         Where dm.EnglishName = "Shaded"
	                         Select dm 'DisplayModeDescription.GetDisplayModes();

	For Each dmd In display_mode_descs
	  RhinoApp.WriteLine("CurveColor {0}", dmd.DisplayAttributes.CurveColor.ToKnownColor())
	  RhinoApp.WriteLine("ObjectColor {0}", dmd.DisplayAttributes.ObjectColor.ToKnownColor())
	Next dmd
	Return Result.Success
  End Function
End Class
