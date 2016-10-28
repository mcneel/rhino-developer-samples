Imports Rhino.Geometry

Partial Class Examples
  Public Shared Function AddTruncatedCone(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim bottom_pt As New Point3d(0, 0, 0)
    Const bottom_radius As Double = 2
    Dim bottom_circle As New Circle(bottom_pt, bottom_radius)

    Dim top_pt As New Point3d(0, 0, 10)
    Const top_radius As Double = 6
    Dim top_circle As New Circle(top_pt, top_radius)

    Dim shapeCurve As New LineCurve(bottom_circle.PointAt(0), top_circle.PointAt(0))
    Dim axis As New Line(bottom_circle.Center, top_circle.Center)
    Dim revsrf As RevSurface = RevSurface.Create(shapeCurve, axis)
    Dim tcone_brep As Brep = Brep.CreateFromRevSurface(revsrf, True, True)
    If doc.Objects.AddBrep(tcone_brep) <> Guid.Empty Then
      doc.Views.Redraw()
      Return Rhino.Commands.Result.Success
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
