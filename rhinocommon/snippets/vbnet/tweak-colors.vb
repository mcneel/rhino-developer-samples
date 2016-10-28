Partial Friend Class Examples
  Public Shared Function TweakColors(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Rhino.ApplicationSettings.AppearanceSettings.SetPaintColor(Rhino.ApplicationSettings.PaintColor.NormalStart, System.Drawing.Color.AliceBlue)
	Rhino.ApplicationSettings.AppearanceSettings.SetPaintColor(Rhino.ApplicationSettings.PaintColor.NormalEnd, System.Drawing.Color.AliceBlue)
	Rhino.ApplicationSettings.AppearanceSettings.SetPaintColor(Rhino.ApplicationSettings.PaintColor.NormalBorder, System.Drawing.Color.LightBlue)
	Rhino.ApplicationSettings.AppearanceSettings.SetPaintColor(Rhino.ApplicationSettings.PaintColor.HotStart, System.Drawing.Color.LightBlue)
	Rhino.ApplicationSettings.AppearanceSettings.SetPaintColor(Rhino.ApplicationSettings.PaintColor.HotEnd, System.Drawing.Color.LightBlue, True)
	Return Rhino.Commands.Result.Success
  End Function
End Class
