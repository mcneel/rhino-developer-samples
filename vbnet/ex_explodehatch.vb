Imports Rhino.DocObjects

Partial Class Examples
  Public Shared Function ExplodeHatch(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result

    Const filter As ObjectType = Rhino.DocObjects.ObjectType.Hatch
    Dim objref As Rhino.DocObjects.ObjRef = Nothing
    Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select hatch to explode", False, filter, objref)
    If rc <> Rhino.Commands.Result.Success OrElse objref Is Nothing Then
      Return rc
    End If

    Dim hatch As Rhino.Geometry.Hatch = DirectCast(objref.Geometry(), Rhino.Geometry.Hatch)
    If hatch Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If

    Dim hatch_geom As Rhino.Geometry.GeometryBase() = hatch.Explode()
    If hatch_geom IsNot Nothing Then
      For i As Integer = 0 To hatch_geom.Length - 1
        Dim geom As Rhino.Geometry.GeometryBase = hatch_geom(i)
        If geom IsNot Nothing Then
          Select Case geom.ObjectType
            Case Rhino.DocObjects.ObjectType.Point
              If True Then
                Dim point As Rhino.Geometry.Point = TryCast(geom, Rhino.Geometry.Point)
                If point IsNot Nothing Then
                  doc.Objects.AddPoint(point.Location)
                End If
              End If
              Exit Select
            Case Rhino.DocObjects.ObjectType.Curve
              If True Then
                Dim curve As Rhino.Geometry.Curve = TryCast(geom, Rhino.Geometry.Curve)
                If curve IsNot Nothing Then
                  doc.Objects.AddCurve(curve)
                End If
              End If
              Exit Select
            Case Rhino.DocObjects.ObjectType.Brep
              If True Then
                Dim brep As Rhino.Geometry.Brep = TryCast(geom, Rhino.Geometry.Brep)
                If brep IsNot Nothing Then
                  doc.Objects.AddBrep(brep)
                End If
              End If
              Exit Select
          End Select
        End If
      Next
    End If

    Return Rhino.Commands.Result.Success
  End Function

End Class