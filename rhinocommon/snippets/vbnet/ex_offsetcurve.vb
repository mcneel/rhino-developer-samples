Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Geometry
Imports Rhino.Input
Imports Rhino.Commands

Namespace examples_vb
  Public Class OffsetCurveCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbOffsetCurve"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rs = RhinoGet.GetOneObject("Select Curve", False, ObjectType.Curve, obj_ref)
      If rs <> Result.Success Then
        Return rs
      End If
      Dim curve = obj_ref.Curve()
      If curve Is Nothing Then
        Return Result.[Nothing]
      End If

      Dim point As Point3d
      rs = RhinoGet.GetPoint("Select Side", False, point)
      If rs <> Result.Success Then
        Return rs
      End If
      If point = Point3d.Unset Then
        Return Result.[Nothing]
      End If

      Dim curves = curve.Offset(point, Vector3d.ZAxis, 1.0, doc.ModelAbsoluteTolerance, CurveOffsetCornerStyle.None)

      For Each offset_curve As Curve In curves
        doc.Objects.AddCurve(offset_curve)
      Next

      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace