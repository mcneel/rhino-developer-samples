Partial Class Examples
  Public Shared Function IntersectCurves(doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' Select two curves to intersect
    Dim go = New Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select two curves")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GetMultiple(2, 2)
    If go.CommandResult() <> Rhino.Commands.Result.Success Then
      Return go.CommandResult()
    End If

    ' Validate input
    Dim curveA = go.[Object](0).Curve()
    Dim curveB = go.[Object](1).Curve()
    If curveA Is Nothing OrElse curveB Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If

    ' Calculate the intersection
    Const intersection_tolerance As Double = 0.001
    Const overlap_tolerance As Double = 0.0
    Dim events = Rhino.Geometry.Intersect.Intersection.CurveCurve(curveA, curveB, intersection_tolerance, overlap_tolerance)

    ' Process the results
    If events IsNot Nothing Then
      For i As Integer = 0 To events.Count - 1
        Dim ccx_event = events(i)
        doc.Objects.AddPoint(ccx_event.PointA)
        If ccx_event.PointA.DistanceTo(ccx_event.PointB) > Double.Epsilon Then
          doc.Objects.AddPoint(ccx_event.PointB)
          doc.Objects.AddLine(ccx_event.PointA, ccx_event.PointB)
        End If
      Next
      doc.Views.Redraw()
    End If
    Return Rhino.Commands.Result.Success
  End Function
End Class
