Imports Rhino.Geometry

Partial Class Examples
  Public Shared Function AddLinearDimension2(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim origin As New Point3d(1, 1, 0)
    Dim offset As New Point3d(11, 1, 0)
    Dim pt As New Point3d((offset.X - origin.X) / 2, 3, 0)

    Dim plane__1 As Plane = Plane.WorldXY
    plane__1.Origin = origin

    Dim u As Double, v As Double
    plane__1.ClosestParameter(origin, u, v)
    Dim ext1 As New Point2d(u, v)

    plane__1.ClosestParameter(offset, u, v)
    Dim ext2 As New Point2d(u, v)

    plane__1.ClosestParameter(pt, u, v)
    Dim linePt As New Point2d(u, v)

    Dim dimension As New LinearDimension(plane__1, ext1, ext2, linePt)
    If doc.Objects.AddLinearDimension(dimension) <> Guid.Empty Then
      doc.Views.Redraw()
      Return Rhino.Commands.Result.Success
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
