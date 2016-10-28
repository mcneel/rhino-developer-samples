Partial Class Examples
  Public Shared Function AddSphere(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim center As New Rhino.Geometry.Point3d(0, 0, 0)
    Const radius As Double = 5.0
    Dim sphere As New Rhino.Geometry.Sphere(center, radius)
    If doc.Objects.AddSphere(sphere) <> Guid.Empty Then
      doc.Views.Redraw()
      Return Rhino.Commands.Result.Success
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
