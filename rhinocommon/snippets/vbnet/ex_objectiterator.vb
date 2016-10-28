
Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class ObjectEnumeratorCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbObjectEnumerator"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim object_enumerator_settings = New ObjectEnumeratorSettings()
      object_enumerator_settings.IncludeLights = True
      object_enumerator_settings.IncludeGrips = False
      Dim rhino_objects = doc.Objects.GetObjectList(object_enumerator_settings)

      Dim count As Integer = 0
      For Each rhino_object As RhinoObject In rhino_objects
        If rhino_object.IsSelectable() AndAlso rhino_object.IsSelected(False) = 0 Then
          rhino_object.[Select](True)
          count += 1
        End If
      Next
      If count > 0 Then
        doc.Views.Redraw()
        RhinoApp.WriteLine("{0} object{1} selected", count, If(count = 1, "", "s"))
      End If
      Return Result.Success
    End Function
  End Class
End Namespace