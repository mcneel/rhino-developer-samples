Imports Rhino
Imports Rhino.Geometry
Imports Rhino.Geometry.Intersect
Imports Rhino.Input.Custom
Imports Rhino.DocObjects
Imports Rhino.Commands

Namespace examples_vb
  Public Class CurveSurfaceIntersectCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbCurveSurfaceIntersect"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim gs = New GetObject()
      gs.SetCommandPrompt("select brep")
      gs.GeometryFilter = ObjectType.Brep
      gs.DisablePreSelect()
      gs.SubObjectSelect = False
      gs.Get()
      If gs.CommandResult() <> Result.Success Then
        Return gs.CommandResult()
      End If
      Dim brep = gs.[Object](0).Brep()

      Dim gc = New GetObject()
      gc.SetCommandPrompt("select curve")
      gc.GeometryFilter = ObjectType.Curve
      gc.DisablePreSelect()
      gc.SubObjectSelect = False
      gc.Get()
      If gc.CommandResult() <> Result.Success Then
        Return gc.CommandResult()
      End If
      Dim curve = gc.Object(0).Curve()

      If brep Is Nothing OrElse curve Is Nothing Then
        Return Result.Failure
      End If

      Dim tolerance = doc.ModelAbsoluteTolerance

      Dim intersectionPoints As Point3d() = Nothing
      Dim overlapCurves As Curve() = Nothing
      If Not Intersection.CurveBrep(curve, brep, tolerance, overlapCurves, intersectionPoints) Then
        RhinoApp.WriteLine("curve brep intersection failed")
        Return Result.Nothing
      End If

      For Each overlapCurve As Curve In overlapCurves
        doc.Objects.AddCurve(overlapCurve)
      Next
      For Each intersectionPoint As Point3d In intersectionPoints
        doc.Objects.AddPoint(intersectionPoint)
      Next

      RhinoApp.WriteLine("{0} overlap curves, and {1} intersection points", overlapCurves.Length, intersectionPoints.Length)
      doc.Views.Redraw()

      Return Result.Success
    End Function
  End Class
End Namespace