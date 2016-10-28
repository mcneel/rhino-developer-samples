Partial Class Examples
  Public Shared Function AddBrepBox(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim pt0 As New Rhino.Geometry.Point3d(0, 0, 0)
    Dim pt1 As New Rhino.Geometry.Point3d(10, 10, 10)
    Dim box As New Rhino.Geometry.BoundingBox(pt0, pt1)
    Dim brep As Rhino.Geometry.Brep = box.ToBrep()
    Dim rc As Rhino.Commands.Result = Rhino.Commands.Result.Failure
    If doc.Objects.AddBrep(brep) <> System.Guid.Empty Then
      rc = Rhino.Commands.Result.Success
      doc.Views.Redraw()
    End If
    Return rc
  End Function
End Class
