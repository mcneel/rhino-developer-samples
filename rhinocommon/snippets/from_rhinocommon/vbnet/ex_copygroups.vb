Imports System.Collections.Generic
Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects
Imports Rhino.Geometry

Namespace examples_vb
  Public Class CopyGroupsCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbCopyGroups"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim go = New Rhino.Input.Custom.GetObject()
      go.SetCommandPrompt("Select objects to copy in place")
      go.GroupSelect = True
      go.SubObjectSelect = False
      go.GetMultiple(1, 0)
      If go.CommandResult() <> Result.Success Then
        Return go.CommandResult()
      End If

      Dim xform = Transform.Identity
      Dim group_map = New Dictionary(Of Integer, Integer)()

      For Each obj_ref As ObjRef In go.Objects()
        If obj_ref IsNot Nothing Then
          Dim obj = obj_ref.[Object]()
          Dim duplicate = doc.Objects.Transform(obj_ref.ObjectId, xform, False)
          RhinoUpdateObjectGroups(obj, group_map)
        End If
      Next
      doc.Views.Redraw()
      Return Result.Success
    End Function

    Private Shared Sub RhinoUpdateObjectGroups(ByRef obj As RhinoObject, ByRef group_map As Dictionary(Of Integer, Integer))
      If obj Is Nothing Then
        Return
      End If

      Dim attrib_group_count As Integer = obj.Attributes.GroupCount
      If attrib_group_count = 0 Then
        Return
      End If

      Dim doc = obj.Document
      If doc Is Nothing Then
        Return
      End If

      Dim groups = doc.Groups

      Dim group_count As Integer = groups.Count
      If group_count = 0 Then
        Return
      End If

      If group_map.Count = 0 Then
        For i As Integer = 0 To group_count - 1
          group_map.Add(i, -1)
        Next
      End If

      Dim attributes = obj.Attributes
      Dim group_list = attributes.GetGroupList()
      If group_list Is Nothing Then
        Return
      End If
      attrib_group_count = group_list.Length

      For i As Integer = 0 To attrib_group_count - 1
        Dim old_group_index As Integer = group_list(i)
        Dim new_group_index As Integer = group_map(old_group_index)
        If new_group_index = -1 Then
          new_group_index = doc.Groups.Add()
          group_map(old_group_index) = new_group_index
        End If
        group_list(i) = new_group_index
      Next

      attributes.RemoveFromAllGroups()
      For i As Integer = 0 To attrib_group_count - 1
        attributes.AddToGroup(group_list(i))
      Next

      obj.CommitChanges()
    End Sub
  End Class
End Namespace