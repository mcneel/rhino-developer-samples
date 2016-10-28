Imports System.Collections.Generic
Imports System.Linq
Imports Rhino
Imports Rhino.Commands
Imports Rhino.Geometry
Imports Rhino.Geometry.Intersect
Imports Rhino.Input
Imports Rhino.Input.Custom
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class FurthestZOnSurfaceCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbFurthestZOnSurfaceGivenXY"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      '#Region "user input"
      ' select a surface
      Dim gs = New GetObject()
      gs.SetCommandPrompt("select surface")
      gs.GeometryFilter = ObjectType.Surface
      gs.DisablePreSelect()
      gs.SubObjectSelect = False
      gs.[Get]()
      If gs.CommandResult() <> Result.Success Then
        Return gs.CommandResult()
      End If
      ' get the brep
      Dim brep = gs.[Object](0).Brep()
      If brep Is Nothing Then
        Return Result.Failure
      End If

      ' get X and Y
      Dim x As Double = 0.0, y As Double = 0.0
      Dim rc = RhinoGet.GetNumber("value of X coordinate", True, x)
      If rc <> Result.Success Then
        Return rc
      End If
      rc = RhinoGet.GetNumber("value of Y coordinate", True, y)
      If rc <> Result.Success Then
        Return rc
      End If
      '#End Region

      ' an earlier version of this sample used a curve-brep intersection to find Z
      'var maxZ = MaxZIntersectionMethod(brep, x, y, doc.ModelAbsoluteTolerance);

      ' projecting points is another way to find Z
      Dim maxZ = MaxZProjectionMethod(brep, x, y, doc.ModelAbsoluteTolerance)

      If maxZ IsNot Nothing Then
        RhinoApp.WriteLine("Maximum surface Z coordinate at X={0}, Y={1} is {2}", x, y, maxZ)
        doc.Objects.AddPoint(New Point3d(x, y, maxZ.Value))
        doc.Views.Redraw()
      Else
        RhinoApp.WriteLine("no maximum surface Z coordinate at X={0}, Y={1} found.", x, y)
      End If

      Return Result.Success
    End Function

    Private Function MaxZProjectionMethod(brep As Brep, x As Double, y As Double, tolerance As Double) As System.Nullable(Of Double)
      Dim maxZ As System.Nullable(Of Double) = Nothing
      Dim breps = New List(Of Brep)() From { _
        brep _
      }
      Dim points = New List(Of Point3d)() From { _
        New Point3d(x, y, 0) _
      }
      ' grab all the points projected in Z dir.  Aggregate finds furthest Z from XY plane
      Try
        maxZ = (From pt In Intersection.ProjectPointsToBreps(breps, points, New Vector3d(0, 0, 1), tolerance) Select pt.Z).Aggregate(Function(z1, z2) If(Math.Abs(z1) > Math.Abs(z2), z1, z2))
        'Sequence contains no elements
      Catch generatedExceptionName As InvalidOperationException
      End Try
      Return maxZ
    End Function

    Private Function MaxZIntersectionMethod(brep As Brep, x As Double, y As Double, tolerance As Double) As System.Nullable(Of Double)
      Dim maxZ As System.Nullable(Of Double) = Nothing

      Dim bbox = brep.GetBoundingBox(True)
      ' furthest Z from XY plane.  Max() doesn't work because of possible negative Z values
      Dim maxDistFromXY = (From corner In bbox.GetCorners() Select corner.Z).Aggregate(Function(z1, z2) If(Math.Abs(z1) > Math.Abs(z2), z1, z2))
      ' multiply distance by 2 to make sure line intersects completely
      Dim lineCurve = New LineCurve(New Point3d(x, y, 0), New Point3d(x, y, maxDistFromXY * 2))

      Dim overlapCurves As Curve() = Nothing
      Dim interPoints As Point3d() = Nothing
      If Intersection.CurveBrep(lineCurve, brep, tolerance, overlapCurves, interPoints) Then
        If overlapCurves.Length > 0 OrElse interPoints.Length > 0 Then
          ' grab all the points resulting frem the intersection. 
          '    1st set: points from overlapping curves, 
          '    2nd set: points when there was no overlap
          '    .Aggregate: furthest Z from XY plane.
          Dim overlapCrvsZs As IEnumerable(Of Double) = (From c In overlapCurves Select DirectCast(IIf(Math.Abs(c.PointAtEnd.Z) > Math.Abs(c.PointAtStart.Z), c.PointAtEnd.Z, c.PointAtStart.Z), Double))
          Dim intersectPtsZs As IEnumerable(Of Double) = (From p In interPoints Select p.Z)
          Dim allZs = overlapCrvsZs.Union(intersectPtsZs).ToArray()
          maxZ = allZs.Aggregate(Function(runZ, nextZ) DirectCast(IIf(Math.Abs(runZ) > Math.Abs(nextZ), runZ, nextZ), Double))
        End If
      End If
      Return maxZ
    End Function
  End Class
End Namespace