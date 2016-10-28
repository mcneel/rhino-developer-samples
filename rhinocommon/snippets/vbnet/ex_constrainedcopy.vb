Partial Class Examples
  Public Shared Function ConstrainedCopy(doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' Get a single planar closed curve
    Dim go = New Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select curve")
    go.GeometryFilter = Rhino.DocObjects.ObjectType.Curve
    go.GeometryAttributeFilter = Rhino.Input.Custom.GeometryAttributeFilter.ClosedCurve
    go.Get()
    If go.CommandResult() <> Rhino.Commands.Result.Success Then
      Return go.CommandResult()
    End If
    Dim objref = go.Object(0)
    Dim base_curve = objref.Curve()
    Dim first_point = objref.SelectionPoint()
    If base_curve Is Nothing OrElse Not first_point.IsValid Then
      Return Rhino.Commands.Result.Cancel
    End If

    Dim plane As Rhino.Geometry.Plane
    If Not base_curve.TryGetPlane(plane) Then
      Return Rhino.Commands.Result.Cancel
    End If

    ' Get a point constrained to a line passing through the initial selection
    ' point and parallel to the plane's normal
    Dim gp = New Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Offset point")
    gp.DrawLineFromPoint(first_point, True)
    Dim line = New Rhino.Geometry.Line(first_point, first_point + plane.Normal)
    gp.Constrain(line)
    gp.Get()
    If gp.CommandResult() <> Rhino.Commands.Result.Success Then
      Return gp.CommandResult()
    End If
    Dim second_point = gp.Point()
    Dim vec As Rhino.Geometry.Vector3d = second_point - first_point
    If vec.Length > 0.001 Then
      Dim xf = Rhino.Geometry.Transform.Translation(vec)
      Dim id As Guid = doc.Objects.Transform(objref, xf, False)
      If id <> Guid.Empty Then
        doc.Views.Redraw()
        Return Rhino.Commands.Result.Success
      End If
    End If
    Return Rhino.Commands.Result.Cancel
  End Function
End Class
