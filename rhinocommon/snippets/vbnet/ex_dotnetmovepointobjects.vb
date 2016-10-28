Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input
Imports Rhino.Input.Custom
Imports Rhino.Geometry

Namespace examples_vb
  Public Class MovePointsCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbMovePoints"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_refs As ObjRef() = Nothing
      Dim rc = RhinoGet.GetMultipleObjects("Select points to move", False, ObjectType.Point, obj_refs)
      If rc <> Result.Success OrElse obj_refs Is Nothing Then
        Return rc
      End If

      Dim gp = New GetPoint()
      gp.SetCommandPrompt("Point to move from")
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If
      Dim start_point = gp.Point()

      gp.SetCommandPrompt("Point to move to")
      gp.SetBasePoint(start_point, False)
      gp.DrawLineFromPoint(start_point, True)
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If
      Dim end_point = gp.Point()

      Dim xform = Transform.Translation(end_point - start_point)

      For Each obj_ref As ObjRef In obj_refs
        doc.Objects.Transform(obj_ref, xform, True)
      Next

      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace