Imports System.Collections.Generic
Imports Rhino
Imports Rhino.Commands
Imports Rhino.Geometry
Imports Rhino.Geometry.Intersect
Imports Rhino.Input
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class ProjectPointsToMeshesExCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbProjectPointsToMeshesEx"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef
      Dim rc = RhinoGet.GetOneObject("mesh", False, ObjectType.Mesh, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim mesh = obj_ref.Mesh()

      Dim obj_ref_pts As ObjRef()
      rc = RhinoGet.GetMultipleObjects("points", False, ObjectType.Point, obj_ref_pts)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim points As New List(Of Point3d)()
      For Each obj_ref_pt As ObjRef In obj_ref_pts
        Dim pt = obj_ref_pt.Point().Location
        points.Add(pt)
      Next

      Dim indices As Integer()
      Dim prj_points = Intersection.ProjectPointsToMeshesEx(New Mesh() {mesh}, points, New Vector3d(0, 1, 0), 0, indices)
      For Each prj_pt As Point3d In prj_points
        doc.Objects.AddPoint(prj_pt)
      Next
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace

