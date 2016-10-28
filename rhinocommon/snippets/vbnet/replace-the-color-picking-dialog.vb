Partial Friend Class Examples
  Private Shared m_dlg As ColorDialog = Nothing

  Public Shared Function ReplaceColorDialog(ByVal doc As RhinoDoc) As Result
	Dialogs.SetCustomColorDialog(AddressOf OnSetCustomColorDialog)
	Return Result.Success
  End Function

  Private Shared Sub OnSetCustomColorDialog(ByVal sender As Object, ByVal e As GetColorEventArgs)
	m_dlg = New ColorDialog()
	If m_dlg.ShowDialog(Nothing) = DialogResult.Ok Then
	  Dim c = m_dlg.Color
	  e.SelectedColor = System.Drawing.Color.FromArgb(c.Ab, c.Rb, c.Gb, c.Bb)
	End If
  End Sub
End Class
