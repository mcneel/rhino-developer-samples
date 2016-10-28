Partial Friend Class Examples
  Public Shared Function SetActiveView(ByVal doc As RhinoDoc) As Result
	' view and view names
	Dim active_view_name = doc.Views.ActiveView.ActiveViewport.Name

	Dim non_active_views = doc.Views.Where(Function(v) v.ActiveViewport.Name <> active_view_name).ToDictionary(Function(v) v.ActiveViewport.Name, Function(v) v)

	' get name of view to set active
	Dim gs = New GetString()
	gs.SetCommandPrompt("Name of view to set active")
	gs.AcceptNothing(True)
	gs.SetDefaultString(active_view_name)
	For Each view_name In non_active_views.Keys
	  gs.AddOption(view_name)
	Next view_name
	Dim result = gs.Get()
	If gs.CommandResult() <> Result.Success Then
	  Return gs.CommandResult()
	End If

	Dim selected_view_name = If(result Is GetResult.Option, gs.Option().EnglishName, gs.StringResult())

	If selected_view_name IsNot active_view_name Then
	  If non_active_views.ContainsKey(selected_view_name) Then
		doc.Views.ActiveView = non_active_views(selected_view_name)
	  Else
		RhinoApp.WriteLine("""{0}"" is not a view name", selected_view_name)
	  End If
	End If

	Return Rhino.Commands.Result.Success
  End Function
End Class
