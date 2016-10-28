Imports Rhino.Geometry

Partial Class Examples
  Public Shared Function IntersectLines(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim go As New Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select lines")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GetMultiple(2, 2)
    If go.CommandResult() <> Rhino.Commands.Result.Success Then
      Return go.CommandResult()
    End If
    If go.ObjectCount <> 2 Then
      Return Rhino.Commands.Result.Failure
    End If

    Dim crv0 As LineCurve = TryCast(go.Object(0).Geometry(), LineCurve)
    Dim crv1 As LineCurve = TryCast(go.Object(1).Geometry(), LineCurve)
    If crv0 Is Nothing OrElse crv1 Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If

    Dim line0 As Line = crv0.Line
    Dim line1 As Line = crv1.Line
    Dim v0 As Vector3d = line0.Direction
    v0.Unitize()
    Dim v1 As Vector3d = line1.Direction
    v1.Unitize()

    If v0.IsParallelTo(v1) <> 0 Then
      Rhino.RhinoApp.WriteLine("Selected lines are parallel.")
      Return Rhino.Commands.Result.[Nothing]
    End If

    Dim a As Double, b As Double
    If Not Rhino.Geometry.Intersect.Intersection.LineLine(line0, line1, a, b) Then
      Rhino.RhinoApp.WriteLine("No intersection found.")
      Return Rhino.Commands.Result.[Nothing]
    End If

    Dim pt0 As Point3d = line0.PointAt(a)
    Dim pt1 As Point3d = line1.PointAt(b)
    ' pt0 and pt1 should be equal, so we will only add pt0 to the document
    doc.Objects.AddPoint(pt0)
    doc.Views.Redraw()
    Return Rhino.Commands.Result.Success
  End Function
End Class
