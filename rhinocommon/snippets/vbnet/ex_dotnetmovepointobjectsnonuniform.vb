Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input

Namespace examples_vb
  Public Class MovePointsNonUniformCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbMovePointsNonUniform"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_refs As ObjRef() = Nothing
      Dim rc = RhinoGet.GetMultipleObjects("Select points to move", False, ObjectType.Point, obj_refs)
      If rc <> Result.Success OrElse obj_refs Is Nothing Then
        Return rc
      End If

      For Each obj_ref As ObjRef In obj_refs
        Dim point3d = obj_ref.Point().Location
        ' modify the point coordinates in some way ...
        point3d.X += 1
        point3d.Y += 1
        point3d.Z += 1

        doc.Objects.Replace(obj_ref, point3d)
      Next

      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace