Partial Friend Class Examples
  Public Shared Function CommandLineOptions(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	' For this example we will use a GetPoint class, but all of the custom
	' "Get" classes support command line options.
	Dim gp As New Rhino.Input.Custom.GetPoint()
	gp.SetCommandPrompt("GetPoint with options")

	' set up the options
	Dim intOption As New Rhino.Input.Custom.OptionInteger(1, 1, 99)
	Dim dblOption As New Rhino.Input.Custom.OptionDouble(2.2, 0, 99.9)
	Dim boolOption As New Rhino.Input.Custom.OptionToggle(True, "Off", "On")
	Dim listValues() As String = { "Item0", "Item1", "Item2", "Item3", "Item4" }

	gp.AddOptionInteger("Integer", intOption)
	gp.AddOptionDouble("Double", dblOption)
	gp.AddOptionToggle("Boolean", boolOption)
	Dim listIndex As Integer = 3
	Dim opList As Integer = gp.AddOptionList("List", listValues, listIndex)

	Do
	  ' perform the get operation. This will prompt the user to input a point, but also
	  ' allow for command line options defined above
	  Dim get_rc As Rhino.Input.GetResult = gp.Get()
	  If gp.CommandResult() <> Rhino.Commands.Result.Success Then
		Return gp.CommandResult()
	  End If

	  If get_rc Is Rhino.Input.GetResult.Point Then
		doc.Objects.AddPoint(gp.Point())
		doc.Views.Redraw()
		Rhino.RhinoApp.WriteLine("Command line option values are")
		Rhino.RhinoApp.WriteLine(" Integer = {0}", intOption.CurrentValue)
		Rhino.RhinoApp.WriteLine(" Double = {0}", dblOption.CurrentValue)
		Rhino.RhinoApp.WriteLine(" Boolean = {0}", boolOption.CurrentValue)
		Rhino.RhinoApp.WriteLine(" List = {0}", listValues(listIndex))
	  ElseIf get_rc Is Rhino.Input.GetResult.Option Then
		If gp.OptionIndex() = opList Then
		  listIndex = gp.Option().CurrentListOptionIndex
		End If
		Continue Do
	  End If
	  Exit Do
	Loop
	Return Rhino.Commands.Result.Success
  End Function
End Class
