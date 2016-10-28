Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Geometry
Imports Rhino.Input
Imports Rhino.Input.Custom
Imports Rhino.Commands

Namespace examples_vb
  Public Class ContourCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbContour"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim filter = ObjectType.Surface Or ObjectType.PolysrfFilter Or ObjectType.Mesh
      Dim obj_refs As ObjRef() = Nothing
      Dim rc = RhinoGet.GetMultipleObjects("Select objects to contour", False, filter, obj_refs)
      If rc <> Result.Success Then
        Return rc
      End If

      Dim gp = New GetPoint()
      gp.SetCommandPrompt("Contour plane base point")
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If
      Dim base_point = gp.Point()

      gp.DrawLineFromPoint(base_point, True)
      gp.SetCommandPrompt("Direction perpendicular to contour planes")
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If
      Dim end_point = gp.Point()

      If base_point.DistanceTo(end_point) < RhinoMath.ZeroTolerance Then
        Return Result.[Nothing]
      End If

      Dim distance As Double = 1.0
      rc = RhinoGet.GetNumber("Distance between contours", False, distance)
      If rc <> Result.Success Then
        Return rc
      End If

      Dim interval = Math.Abs(distance)

      Dim curves As Curve() = Nothing
      For Each obj_ref As ObjRef In obj_refs
        Dim geometry = obj_ref.Geometry()
        If geometry Is Nothing Then
          Return Result.Failure
        End If

        If TypeOf geometry Is Brep Then
          curves = Brep.CreateContourCurves(TryCast(geometry, Brep), base_point, end_point, interval)
        Else
          curves = Mesh.CreateContourCurves(TryCast(geometry, Mesh), base_point, end_point, interval)
        End If

        For Each curve As Curve In curves
          Dim curve_object_id = doc.Objects.AddCurve(curve)
          doc.Objects.[Select](curve_object_id)
        Next
      Next

      If curves IsNot Nothing Then
        doc.Views.Redraw()
      End If
      Return Result.Success
    End Function
  End Class
End Namespace