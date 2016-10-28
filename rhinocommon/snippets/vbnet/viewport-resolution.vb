Partial Friend Class Examples
  Public Shared Function ViewportResolution(ByVal doc As RhinoDoc) As Result
	Dim active_viewport = doc.Views.ActiveView.ActiveViewport
	RhinoApp.WriteLine("Name = {0}: Width = {1}, Height = {2}", active_viewport.Name, active_viewport.Size.Width, active_viewport.Size.Height)
	Return Result.Success
  End Function
End Class
