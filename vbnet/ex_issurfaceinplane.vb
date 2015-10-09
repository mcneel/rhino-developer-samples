Imports System.Linq
Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Geometry
Imports Rhino.Commands
Imports Rhino.Input

Namespace examples_vb
  Public Class IsPlanarSurfaceInPlaneCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbIsPlanarSurfaceInPlane"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("select surface", True, ObjectType.Surface, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim surface = obj_ref.Surface()

      Dim corners As Point3d() = Nothing
      rc = RhinoGet.GetRectangle(corners)
      If rc <> Result.Success Then
        Return rc
      End If

      Dim plane = New Plane(corners(0), corners(1), corners(2))

      Dim is_or_isnt = If(IsSurfaceInPlane(surface, plane, doc.ModelAbsoluteTolerance), "", " not ")
      RhinoApp.WriteLine("Surface is{0} in plane.", is_or_isnt)
      Return Result.Success
    End Function

    Private Function IsSurfaceInPlane(surface As Surface, plane As Plane, tolerance As Double) As Boolean
      If Not surface.IsPlanar(tolerance) Then
        Return False
      End If

      Dim bbox = surface.GetBoundingBox(True)
      Return bbox.GetCorners().All(Function(corner) Math.Abs(plane.DistanceTo(corner)) <= tolerance)
    End Function
  End Class
End Namespace