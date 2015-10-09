Partial Class Examples
  Public Shared Function OrientOnSrf(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' Select objects to orient
    Dim go As New Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select objects to orient")
    go.SubObjectSelect = False
    go.GroupSelect = True
    go.GetMultiple(1, 0)
    If go.CommandResult() <> Rhino.Commands.Result.Success Then
      Return go.CommandResult()
    End If

    ' Point to orient from
    Dim gp As New Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Point to orient from")
    gp.Get()
    If gp.CommandResult() <> Rhino.Commands.Result.Success Then
      Return gp.CommandResult()
    End If

    ' Define source plane
    Dim view As Rhino.Display.RhinoView = gp.View()
    If view Is Nothing Then
      view = doc.Views.ActiveView
      If view Is Nothing Then
        Return Rhino.Commands.Result.Failure
      End If
    End If
    Dim source_plane As Rhino.Geometry.Plane = view.ActiveViewport.ConstructionPlane()
    source_plane.Origin = gp.Point()

    ' Surface to orient on
    Dim gs As New Rhino.Input.Custom.GetObject()
    gs.SetCommandPrompt("Surface to orient on")
    gs.GeometryFilter = Rhino.DocObjects.ObjectType.Surface
    gs.SubObjectSelect = True
    gs.DeselectAllBeforePostSelect = False
    gs.OneByOnePostSelect = True
    gs.Get()
    If gs.CommandResult() <> Rhino.Commands.Result.Success Then
      Return gs.CommandResult()
    End If

    Dim objref As Rhino.DocObjects.ObjRef = gs.[Object](0)
    ' get selected surface object
    Dim obj As Rhino.DocObjects.RhinoObject = objref.[Object]()
    If obj Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If
    ' get selected surface (face)
    Dim surface As Rhino.Geometry.Surface = objref.Surface()
    If surface Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If
    ' Unselect surface
    obj.[Select](False)

    ' Point on surface to orient to
    gp.SetCommandPrompt("Point on surface to orient to")
    gp.Constrain(surface, False)
    gp.Get()
    If gp.CommandResult() <> Rhino.Commands.Result.Success Then
      Return gp.CommandResult()
    End If

    ' Do transformation
    Dim rc As Rhino.Commands.Result = Rhino.Commands.Result.Failure
    Dim u As Double, v As Double
    If surface.ClosestPoint(gp.Point(), u, v) Then
      Dim target_plane As Rhino.Geometry.Plane
      If surface.FrameAt(u, v, target_plane) Then
        ' Build transformation
        Dim xform As Rhino.Geometry.Transform = Rhino.Geometry.Transform.PlaneToPlane(source_plane, target_plane)

        ' Do the transformation. In this example, we will copy the original objects
        Const delete_original As Boolean = False
        For i As Integer = 0 To go.ObjectCount - 1
          doc.Objects.Transform(go.[Object](i), xform, delete_original)
        Next

        doc.Views.Redraw()
        rc = Rhino.Commands.Result.Success
      End If
    End If
    Return rc
  End Function
End Class
