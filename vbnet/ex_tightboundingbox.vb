Imports Rhino
Imports Rhino.Commands
Imports System.Linq
Imports Rhino.Geometry
Imports Rhino.Input
Imports Rhino.DocObjects
Imports System.Collections.Generic

Namespace examples_vb
  Public Class TightBoundingBoxCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbTightBoundingBox"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select surface to split", True, ObjectType.Surface, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim surface = obj_ref.Surface()
      If surface Is Nothing Then
        Return Result.Failure
      End If

      obj_ref = Nothing
      rc = RhinoGet.GetOneObject("Select cutting curve", True, ObjectType.Curve, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim curve = obj_ref.Curve()
      If curve Is Nothing Then
        Return Result.Failure
      End If

      Dim brep_face = TryCast(surface, BrepFace)
      If brep_face Is Nothing Then
        Return Result.Failure
      End If

      Dim split_brep = brep_face.Split(New List(Of Curve)() From { _
        curve _
      }, doc.ModelAbsoluteTolerance)
      If split_brep Is Nothing Then
        RhinoApp.WriteLine("Unable to split surface.")
        Return Result.[Nothing]
      End If

      Dim meshes = Mesh.CreateFromBrep(split_brep)

      For Each mesh__1 As Mesh In meshes
        Dim bbox = mesh__1.GetBoundingBox(True)
        Select Case bbox.IsDegenerate(doc.ModelAbsoluteTolerance)
          Case 3, 2
            Return Result.Failure
            Exit Select
          Case 1
            ' rectangle
            ' box with 8 corners flattened to rectangle with 4 corners
            Dim rectangle_corners = bbox.GetCorners().Distinct().ToList()
            ' add 1st point as last to close the loop
            rectangle_corners.Add(rectangle_corners(0))
            doc.Objects.AddPolyline(rectangle_corners)
            doc.Views.Redraw()
            Exit Select
          Case 0
            ' box
            Dim brep_box = New Box(bbox).ToBrep()
            doc.Objects.AddBrep(brep_box)
            doc.Views.Redraw()
            Exit Select
        End Select
      Next

      Return Result.Success
    End Function
  End Class
End Namespace
