Imports Rhino
Imports Rhino.Commands
Imports Rhino.Geometry

Namespace examples_vb
  <System.Runtime.InteropServices.Guid("652FCBE5-D8DC-4472-AB94-5A70998A3895")> _
  Public Class CreateSurfaceFromPointsAndKnotsCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbCreateSurfaceFromPointsAndKnots"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Const isRational As Boolean = False
      Const numberOfDimensions As Integer = 3
      Const uDegree As Integer = 2
      Const vDegree As Integer = 3
      Const uControlPointCount As Integer = 3
      Const vControlPointCount As Integer = 5

      ' The knot vectors do NOT have the 2 superfluous knots
      ' at the start and end of the knot vector.  If you are
      ' coming from a system that has the 2 superfluous knots,
      ' just ignore them when creating NURBS surfaces.
      Dim uKnots = New Double(uControlPointCount + uDegree - 2) {}
      Dim vKnots = New Double(vControlPointCount + vDegree - 2) {}

      ' make up a quadratic knot vector with no interior knots
      uKnots(0) = InlineAssignHelper(uKnots(1), 0.0)
      uKnots(2) = InlineAssignHelper(uKnots(3), 1.0)

      ' make up a cubic knot vector with one simple interior knot
      vKnots(0) = InlineAssignHelper(vKnots(1), InlineAssignHelper(vKnots(2), 0.0))
      vKnots(3) = 1.5
      vKnots(4) = InlineAssignHelper(vKnots(5), InlineAssignHelper(vKnots(6), 2.0))

      ' Rational control points can be in either homogeneous
      ' or euclidean form. Non-rational control points do not
      ' need to specify a weight.  
      Dim controlPoints = New Point3d(uControlPointCount - 1, vControlPointCount - 1) {}

      For u As Integer = 0 To uControlPointCount - 1
        For v As Integer = 0 To vControlPointCount - 1
          controlPoints(u, v) = New Point3d(u, v, u - v)
        Next
      Next

      ' creates internal uninitialized arrays for 
      ' control points and knots
      Dim nurbsSurface__1 = NurbsSurface.Create(numberOfDimensions, isRational, uDegree + 1, vDegree + 1, uControlPointCount, vControlPointCount)

      ' add the knots
      For u As Integer = 0 To nurbsSurface__1.KnotsU.Count - 1
        nurbsSurface__1.KnotsU(u) = uKnots(u)
      Next
      For v As Integer = 0 To nurbsSurface__1.KnotsV.Count - 1
        nurbsSurface__1.KnotsV(v) = vKnots(v)
      Next

      ' add the control points
      For u As Integer = 0 To nurbsSurface__1.Points.CountU - 1
        For v As Integer = 0 To nurbsSurface__1.Points.CountV - 1
          nurbsSurface__1.Points.SetControlPoint(u, v, controlPoints(u, v))
        Next
      Next

      If nurbsSurface__1.IsValid Then
        doc.Objects.AddSurface(nurbsSurface__1)
        doc.Views.Redraw()
        Return Result.Success
      Else
        Return Result.Failure
      End If
    End Function
    Private Shared Function InlineAssignHelper(Of T)(ByRef target As T, value As T) As T
      target = value
      Return value
    End Function
  End Class
End Namespace