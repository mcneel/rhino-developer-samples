Imports Rhino.DocObjects

Partial Class Examples
  Public Shared Function DupBorder(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Const filter As ObjectType = Rhino.DocObjects.ObjectType.Surface Or Rhino.DocObjects.ObjectType.PolysrfFilter
    Dim objref As Rhino.DocObjects.ObjRef = Nothing
    Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface", False, filter, objref)
    If rc <> Rhino.Commands.Result.Success OrElse objref Is Nothing Then
      Return rc
    End If

    Dim rhobj As Rhino.DocObjects.RhinoObject = objref.[Object]()
    Dim brep As Rhino.Geometry.Brep = objref.Brep()
    If rhobj Is Nothing OrElse brep Is Nothing Then
      Return Rhino.Commands.Result.Failure
    End If

    rhobj.[Select](False)
    Dim curves As Rhino.Geometry.Curve() = brep.DuplicateEdgeCurves(True)
    Dim tol As Double = doc.ModelAbsoluteTolerance * 2.1
    curves = Rhino.Geometry.Curve.JoinCurves(curves, tol)
    For i As Integer = 0 To curves.Length - 1
      Dim id As Guid = doc.Objects.AddCurve(curves(i))
      doc.Objects.[Select](id)
    Next
    doc.Views.Redraw()
    Return Rhino.Commands.Result.Success
  End Function
End Class
