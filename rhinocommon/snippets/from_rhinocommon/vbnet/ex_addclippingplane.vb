Partial Class Examples
  Public Shared Function AddClippingPlane(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' Define the corners of the clipping plane
    Dim corners As Rhino.Geometry.Point3d() = Nothing
    Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetRectangle(corners)
    If rc <> Rhino.Commands.Result.Success Then
      Return rc
    End If

    ' Get the active view
    Dim view As Rhino.Display.RhinoView = doc.Views.ActiveView
    If view Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If

    Dim p0 As Rhino.Geometry.Point3d = corners(0)
    Dim p1 As Rhino.Geometry.Point3d = corners(1)
    Dim p3 As Rhino.Geometry.Point3d = corners(3)

    ' Create a plane from the corner points
    Dim plane As New Rhino.Geometry.Plane(p0, p1, p3)

    ' Add a clipping plane object to the document
    Dim id As Guid = doc.Objects.AddClippingPlane(plane, p0.DistanceTo(p1), p0.DistanceTo(p3), view.ActiveViewportID)
    If id <> Guid.Empty Then
      doc.Views.Redraw()
      Return Rhino.Commands.Result.Success
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
