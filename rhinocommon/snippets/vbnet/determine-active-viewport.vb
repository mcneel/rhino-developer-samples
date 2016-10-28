Partial Friend Class Examples
  Public Shared Function ActiveViewport(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim view As Rhino.Display.RhinoView = doc.Views.ActiveView
	If view Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim pageview As Rhino.Display.RhinoPageView = TryCast(view, Rhino.Display.RhinoPageView)
	If pageview IsNot Nothing Then
	  Dim layout_name As String = pageview.PageName
	  If pageview.PageIsActive Then
		Rhino.RhinoApp.WriteLine("The layout {0} is active", layout_name)
	  Else
		Dim detail_name As String = pageview.ActiveViewport.Name
		Rhino.RhinoApp.WriteLine("The detail {0} on layout {1} is active", detail_name, layout_name)
	  End If
	Else
	  Dim viewport_name As String = view.MainViewport.Name
	  Rhino.RhinoApp.WriteLine("The viewport {0} is active", viewport_name)
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
