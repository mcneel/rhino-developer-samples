Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects
Imports Rhino.Input

Namespace examples_vb
  Public Class DuplicateObjectCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDuplicateObject"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing

      Dim rc = RhinoGet.GetOneObject("Select object to duplicate", False, ObjectType.AnyObject, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim rhino_object = obj_ref.[Object]()

      Dim geometry_base = rhino_object.DuplicateGeometry()
      If geometry_base IsNot Nothing Then
        If doc.Objects.Add(geometry_base) <> Guid.Empty Then
          doc.Views.Redraw()
        End If
      End If

      Return Result.Success
    End Function
  End Class
End Namespace