Imports System.Linq
Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects
Imports Rhino.Input

Namespace examples_vb
  Public Class SelectGroupObjecCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbSelectObjectsInObjectGroups"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rs = RhinoGet.GetOneObject("Select object", False, ObjectType.AnyObject, obj_ref)
      If rs <> Result.Success Then
        Return rs
      End If
      Dim rhino_object = obj_ref.[Object]()
      If rhino_object Is Nothing Then
        Return Result.Failure
      End If

      Dim rhino_object_groups = rhino_object.Attributes.GetGroupList().DefaultIfEmpty(-1)

      Dim selectable_objects = From obj In doc.Objects.GetObjectList(ObjectType.AnyObject) Where obj.IsSelectable(True, False, False, False) Select obj

      For Each selectable_object As RhinoObject In selectable_objects
        For Each group As Integer In selectable_object.Attributes.GetGroupList()
          If rhino_object_groups.Contains(group) Then
            selectable_object.[Select](True)
            Continue For
          End If
        Next
      Next
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace