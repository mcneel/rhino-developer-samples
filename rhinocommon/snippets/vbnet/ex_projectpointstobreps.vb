Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Input.Custom
Imports Rhino.Commands
Imports System.Collections.Generic
Imports Rhino.Geometry
Imports Rhino.Geometry.Intersect

Namespace examples_vb
  Public Class ProjectPointsToBrepsCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbProjectPtointsToBreps"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim gs = New GetObject()
      gs.SetCommandPrompt("select surface")
      gs.GeometryFilter = ObjectType.Surface Or ObjectType.PolysrfFilter
      gs.DisablePreSelect()
      gs.SubObjectSelect = False
      gs.[Get]()
      If gs.CommandResult() <> Result.Success Then
        Return gs.CommandResult()
      End If
      Dim brep = gs.[Object](0).Brep()
      If brep Is Nothing Then
        Return Result.Failure
      End If

      ' brep on which to project
      ' some random points to project
      ' project on Y axis
      Dim points = Intersection.ProjectPointsToBreps(New List(Of Brep)() From { _
        brep _
      }, New List(Of Point3d)() From { _
        New Point3d(0, 0, 0), _
        New Point3d(3, 0, 3), _
        New Point3d(-2, 0, -2) _
      }, New Vector3d(0, 1, 0), doc.ModelAbsoluteTolerance)

      If points IsNot Nothing AndAlso points.Length > 0 Then
        For Each point As Point3d In points
          doc.Objects.AddPoint(point)
        Next
      End If
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace