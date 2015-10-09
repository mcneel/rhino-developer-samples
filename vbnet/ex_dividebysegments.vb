Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input
Imports Rhino.Geometry

Namespace examples_vb
  Public Class DivideBySegmentsCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDivideCurveBySegments"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Const filter As ObjectType = ObjectType.Curve
      Dim objref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select curve to divide", False, filter, objref)
      If rc <> Result.Success OrElse objref Is Nothing Then
        Return rc
      End If

      Dim curve = objref.Curve()
      If curve Is Nothing OrElse curve.IsShort(RhinoMath.ZeroTolerance) Then
        Return Result.Failure
      End If

      Dim segment_count = 2
      rc = RhinoGet.GetInteger("Divide curve into how many segments?", False, segment_count)
      If rc <> Result.Success Then
        Return rc
      End If

      Dim points As Point3d() = Nothing
      curve.DivideByCount(segment_count, True, points)
      If points Is Nothing Then
        Return Result.Failure
      End If

      For Each point As Point3d In points
        doc.Objects.AddPoint(point)
      Next

      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace