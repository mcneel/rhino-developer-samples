Imports Rhino
Imports Rhino.DocObjects

Partial Class Examples
  Public Shared Function ObjectDisplayMode(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim rc As Rhino.Commands.Result
    Const filter As ObjectType = ObjectType.Mesh Or ObjectType.Brep
    Dim objref As ObjRef = Nothing
    rc = Rhino.Input.RhinoGet.GetOneObject("Select mesh or surface", True, filter, objref)
    If rc <> Rhino.Commands.Result.Success Then
      Return rc
    End If
    Dim viewportId As Guid = doc.Views.ActiveView.ActiveViewportID

    Dim attr As ObjectAttributes = objref.[Object]().Attributes
    If attr.HasDisplayModeOverride(viewportId) Then
      RhinoApp.WriteLine("Removing display mode override from object")
      attr.RemoveDisplayModeOverride(viewportId)
    Else
      Dim modes As Rhino.Display.DisplayModeDescription() = Rhino.Display.DisplayModeDescription.GetDisplayModes()
      Dim mode As Rhino.Display.DisplayModeDescription = Nothing
      If modes.Length = 1 Then
        mode = modes(0)
      Else
        Dim go As New Rhino.Input.Custom.GetOption()
        go.SetCommandPrompt("Select display mode")
        Dim str_modes As String() = New String(modes.Length - 1) {}
        For i As Integer = 0 To modes.Length - 1
          str_modes(i) = modes(i).EnglishName.Replace(" ", "").Replace("-", "")
        Next
        go.AddOptionList("DisplayMode", str_modes, 0)
        If go.[Get]() = Rhino.Input.GetResult.[Option] Then
          mode = modes(go.[Option]().CurrentListOptionIndex)
        End If
      End If
      If mode Is Nothing Then
        Return Rhino.Commands.Result.Cancel
      End If
      attr.SetDisplayModeOverride(mode, viewportId)
    End If
    doc.Objects.ModifyAttributes(objref, attr, False)
    doc.Views.Redraw()
    Return Rhino.Commands.Result.Success
  End Function
End Class
