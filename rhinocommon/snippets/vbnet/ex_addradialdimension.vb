Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Geometry
Imports Rhino.Input

Namespace examples_vb
  Public Class AddRadialDimensionCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbAddRadialDimension"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select curve for radius dimension", True, ObjectType.Curve, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim curve_parameter As Double
      Dim curve = obj_ref.CurveParameter(curve_parameter)
      If curve Is Nothing Then
        Return Result.Failure
      End If

      If curve.IsLinear() OrElse curve.IsPolyline() Then
        RhinoApp.WriteLine("Curve must be non-linear.")
        Return Result.[Nothing]
      End If

      ' in this example just deal with planar curves
      If Not curve.IsPlanar() Then
        RhinoApp.WriteLine("Curve must be planar.")
        Return Result.[Nothing]
      End If

      Dim point_on_curve = curve.PointAt(curve_parameter)
      Dim curvature_vector = curve.CurvatureAt(curve_parameter)
      Dim len = curvature_vector.Length
      If len < RhinoMath.SqrtEpsilon Then
        RhinoApp.WriteLine("Curve is almost linear and therefore has no curvature.")
        Return Result.[Nothing]
      End If

      Dim center = point_on_curve + (curvature_vector / (len * len))
      Dim plane As Plane
      curve.TryGetPlane(plane)
      Dim radial_dimension = New RadialDimension(center, point_on_curve, plane.XAxis, plane.Normal, 5.0)
      doc.Objects.AddRadialDimension(radial_dimension)
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace