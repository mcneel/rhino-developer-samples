Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input
Imports Rhino.UI

Namespace examples_vb
  Public Class ChangeLightColorCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbLightColor"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select light to change color", True, ObjectType.Light, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim light = obj_ref.Light()
      If light Is Nothing Then
        Return Result.Failure
      End If

      Dim diffuse_color = light.Diffuse
      If Dialogs.ShowColorDialog(diffuse_color) Then
        light.Diffuse = diffuse_color
      End If

      doc.Lights.Modify(obj_ref.ObjectId, light)
      Return Result.Success
    End Function
  End Class
End Namespace