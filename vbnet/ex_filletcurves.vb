Imports Rhino
Imports Rhino.Commands
Imports Rhino.Geometry
Imports Rhino.Input
Imports Rhino.DocObjects
Imports Rhino.Input.Custom

Namespace examples_vb
  Public Class FilletCurvesCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbFilletCurves"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim gc1 = New GetObject()
      gc1.DisablePreSelect()
      gc1.SetCommandPrompt("Select first curve to fillet (close to the end you want to fillet)")
      gc1.GeometryFilter = ObjectType.Curve
      gc1.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
      gc1.[Get]()
      If gc1.CommandResult() <> Result.Success Then
        Return gc1.CommandResult()
      End If
      Dim curve1_obj_ref = gc1.[Object](0)
      Dim curve1 = curve1_obj_ref.Curve()
      If curve1 Is Nothing Then
        Return Result.Failure
      End If
      Dim curve1_point_near_end = curve1_obj_ref.SelectionPoint()
      If curve1_point_near_end = Point3d.Unset Then
        Return Result.Failure
      End If

      Dim gc2 = New GetObject()
      gc2.DisablePreSelect()
      gc2.SetCommandPrompt("Select second curve to fillet (close to the end you want to fillet)")
      gc2.GeometryFilter = ObjectType.Curve
      gc2.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
      gc2.[Get]()
      If gc2.CommandResult() <> Result.Success Then
        Return gc2.CommandResult()
      End If
      Dim curve2_obj_ref = gc2.[Object](0)
      Dim curve2 = curve2_obj_ref.Curve()
      If curve2 Is Nothing Then
        Return Result.Failure
      End If
      Dim curve2_point_near_end = curve2_obj_ref.SelectionPoint()
      If curve2_point_near_end = Point3d.Unset Then
        Return Result.Failure
      End If

      Dim radius As Double = 0
      Dim rc = RhinoGet.GetNumber("fillet radius", False, radius)
      If rc <> Result.Success Then
        Return rc
      End If

      Dim fillet_curve = Curve.CreateFilletCurves(curve1, curve1_point_near_end, curve2, curve2_point_near_end, radius, True, _
        True, True, doc.ModelAbsoluteTolerance, doc.ModelAngleToleranceDegrees)
      If fillet_curve Is Nothing OrElse fillet_curve.Length <> 1 Then
        Return Result.Failure
      End If

      doc.Objects.AddCurve(fillet_curve(0))
      doc.Views.Redraw()
      Return rc
    End Function
  End Class
End Namespace