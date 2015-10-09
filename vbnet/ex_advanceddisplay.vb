Imports System.Collections.Generic
Imports Rhino.Display

Partial Class Examples
  ' The following example demonstrates how to modify advanced display settings using
  ' RhinoCommon. In this example, a display mode's mesh wireframe thickness (in pixels)
  ' will be modified.
  Public Shared Function AdvancedDisplay(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' Use the display attributes manager to build a list of display modes.
    ' Note, these are copies of the originals...
    Dim display_modes As DisplayModeDescription() = DisplayModeDescription.GetDisplayModes()
    If display_modes Is Nothing OrElse display_modes.Length < 1 Then
      Return Rhino.Commands.Result.Failure
    End If

    ' Construct an options picker so the user can pick which
    ' display mode they want modified
    Dim go As New Rhino.Input.Custom.GetOption()
    go.SetCommandPrompt("Display mode to modify mesh thickness")
    Dim opt_list As New List(Of Integer)()

    For i As Integer = 0 To display_modes.Length - 1
      Dim english_name As String = display_modes(i).EnglishName
      english_name = english_name.Replace("_", "")
      english_name = english_name.Replace(" ", "")
      english_name = english_name.Replace("-", "")
      english_name = english_name.Replace(",", "")
      english_name = english_name.Replace(".", "")
      Dim index As Integer = go.AddOption(english_name)
      opt_list.Add(index)
    Next

    ' Get the command option
    go.[Get]()
    If go.CommandResult() <> Rhino.Commands.Result.Success Then
      Return go.CommandResult()
    End If

    Dim selected_index As Integer = go.[Option]().Index
    Dim selected_description As DisplayModeDescription = Nothing
    For i As Integer = 0 To opt_list.Count - 1
      If opt_list(i) = selected_index Then
        selected_description = display_modes(i)
        Exit For
      End If
    Next

    ' Validate...
    If selected_description Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If

    ' Modify the desired display mode. In this case, we
    ' will just set the mesh wireframe thickness to zero.
    selected_description.DisplayAttributes.MeshSpecificAttributes.MeshWireThickness = 0
    ' Use the display attributes manager to update the display mode.
    DisplayModeDescription.UpdateDisplayMode(selected_description)

    ' Force the document to regenerate.
    doc.Views.Redraw()
    Return Rhino.Commands.Result.Success
  End Function
End Class
