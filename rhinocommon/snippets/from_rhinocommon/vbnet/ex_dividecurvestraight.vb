Imports System.Collections.Generic
Imports System.Linq
Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input
Imports Rhino.Geometry
Imports Rhino.Geometry.Intersect

Namespace examples_vb
  Public Class DivideCurveStraightCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDivideCurveStraight"
      End Get
    End Property

    Private Sub NextintersectParamAndPoint(overlapCurves As Curve(), intersectPoints As Point3d(), curve As Curve, ByRef intersectParam As Double, ByRef intersectPoint As Point3d)
      Dim intersect_params_and_points = New Dictionary(Of Double, Point3d)()
      For Each point As Point3d In intersectPoints
        Dim curve_param As Double
        curve.ClosestPoint(point, curve_param)
        intersect_params_and_points(curve_param) = point
      Next
      For Each overlap_curve As Curve In overlapCurves
        intersect_params_and_points(overlap_curve.Domain.Min) = overlap_curve.PointAt(overlap_curve.Domain.Min)
        intersect_params_and_points(overlap_curve.Domain.Max) = overlap_curve.PointAt(overlap_curve.Domain.Max)
      Next
      Dim min_t = intersect_params_and_points.Keys.Min()
      intersectParam = min_t
      intersectPoint = intersect_params_and_points(intersectParam)
    End Sub

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      ' user input
      Dim obj_refs As ObjRef() = Nothing
      Dim rc = RhinoGet.GetMultipleObjects("Select curve to divide", False, ObjectType.Curve Or ObjectType.EdgeFilter, obj_refs)
      If rc <> Result.Success OrElse obj_refs Is Nothing Then
        Return rc
      End If

      Dim distance_between_divisions As Double = 5
      rc = RhinoGet.GetNumber("Distance between divisions", False, distance_between_divisions, 1.0, [Double].MaxValue)
      If rc <> Result.Success Then
        Return rc
      End If


      ' generate the points
      Dim points = New List(Of Point3d)()
      For Each obj_ref As ObjRef In obj_refs
        Dim curve = obj_ref.Curve()
        If curve Is Nothing Then
          Return Result.Failure
        End If

        Dim t0 = curve.Domain.Min
        points.Add(curve.PointAt(t0))

        Dim sphere_center = curve.PointAt(t0)
        Dim t = t0
        Dim rest_of_curve = curve
        While True
          Dim sphere = New Sphere(sphere_center, distance_between_divisions)
          Dim overlap_curves As Curve() = Nothing
          Dim intersect_points As Point3d() = Nothing
          Dim b = Intersection.CurveBrep(rest_of_curve, sphere.ToBrep(), 0.0, overlap_curves, intersect_points)
          If Not b OrElse (overlap_curves.Length = 0 AndAlso intersect_points.Length = 0) Then
            Exit While
          End If
          Dim intersect_param As Double
          Dim intersect_point As Point3d
          NextintersectParamAndPoint(overlap_curves, intersect_points, rest_of_curve, intersect_param, intersect_point)
          points.Add(intersect_point)
          t = intersect_param
          sphere_center = intersect_point
          rest_of_curve = curve.Split(t)(1)
        End While
      Next

      For Each point As Point3d In points
        doc.Objects.AddPoint(point)
      Next

      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace