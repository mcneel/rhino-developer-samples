Partial Class Examples
  Public Shared Function AddTorus(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Const major_radius As Double = 4.0
    Const minor_radius As Double = 2.0

    Dim plane As Rhino.Geometry.Plane = Rhino.Geometry.Plane.WorldXY
    Dim torus As New Rhino.Geometry.Torus(plane, major_radius, minor_radius)
    Dim revsrf As Rhino.Geometry.RevSurface = torus.ToRevSurface()
    If doc.Objects.AddSurface(revsrf) <> Guid.Empty Then
      doc.Views.Redraw()
      Return Rhino.Commands.Result.Success
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
