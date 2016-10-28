Partial Friend Class Examples
  Private ReadOnly Shared m_customconduit As New CustomConduit()
  Public Shared Function DrawOverlay(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	' toggle conduit on/off
	m_customconduit.Enabled = Not m_conduit.Enabled

	RhinoApp.WriteLine("Custom conduit enabled = {0}", m_customconduit.Enabled)
	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
