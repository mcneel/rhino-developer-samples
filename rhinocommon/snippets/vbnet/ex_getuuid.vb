Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects
Imports Rhino.Input

Namespace examples_vb
  Public Class GetUuidCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbGetUUID"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select object", False, ObjectType.AnyObject, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      If obj_ref Is Nothing Then
        Return Result.[Nothing]
      End If

      Dim uuid = obj_ref.ObjectId
      RhinoApp.WriteLine("The object's unique id is {0}", uuid.ToString())
      Return Result.Success
    End Function
  End Class
End Namespace