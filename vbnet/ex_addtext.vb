Partial Class Examples
  Public Shared Function AddAnnotationText(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim pt As New Rhino.Geometry.Point3d(10, 0, 0)
    Const text As String = "Hello RhinoCommon"
    Const height As Double = 2.0
    Const font As String = "Arial"
    Dim plane As Rhino.Geometry.Plane = doc.Views.ActiveView.ActiveViewport.ConstructionPlane()
    plane.Origin = pt
    Dim id As Guid = doc.Objects.AddText(text, plane, height, font, False, False)
    If id <> Guid.Empty Then
      doc.Views.Redraw()
      Return Rhino.Commands.Result.Success
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
