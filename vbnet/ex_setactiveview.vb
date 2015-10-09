Imports Rhino
Imports Rhino.Commands
Imports Rhino.Input
Imports Rhino.Input.Custom
Imports Rhino.Display
Imports System.Collections.Generic
Imports System.Linq

Namespace examples_vb
  Public Class SetActiveViewCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbSetActiveView"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      ' view and view names
      Dim active_view_name = doc.Views.ActiveView.ActiveViewport.Name

      Dim non_active_views = doc.Views.Where(Function(v) v.ActiveViewport.Name <> active_view_name).ToDictionary(Function(v) v.ActiveViewport.Name, Function(v) v)

      ' get name of view to set active
      Dim gs = New GetString()
      gs.SetCommandPrompt("Name of view to set active")
      gs.AcceptNothing(True)
      gs.SetDefaultString(active_view_name)
      For Each view_name As String In non_active_views.Keys
        gs.AddOption(view_name)
      Next
      Dim result__1 = gs.[Get]()
      If gs.CommandResult() <> Result.Success Then
        Return gs.CommandResult()
      End If

      Dim selected_view_name = If(result__1 = GetResult.[Option], gs.[Option]().EnglishName, gs.StringResult())

      If selected_view_name <> active_view_name Then
        If non_active_views.ContainsKey(selected_view_name) Then
          doc.Views.ActiveView = non_active_views(selected_view_name)
        Else
          RhinoApp.WriteLine("""{0}"" is not a view name", selected_view_name)
        End If
      End If

      Return Rhino.Commands.Result.Success
    End Function
  End Class
End Namespace