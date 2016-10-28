Imports Rhino
Imports Rhino.Input.Custom
Imports Rhino.DocObjects
Imports Rhino.Commands

Namespace examples_vb
  Public Class NormalDirectionOfBrepFaceCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDetermineNormDirectionOfBrepFace"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
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
      ' get the selected face
      Dim face = gs.[Object](0).Face()
      If face Is Nothing Then
        Return Result.Failure
      End If

      ' pick a point on the surface.  Constain
      ' picking to the face.
      Dim gp = New GetPoint()
      gp.SetCommandPrompt("select point on surface")
      gp.Constrain(face, False)
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If

      ' get the parameters of the point on the
      ' surface that is clesest to gp.Point()
      Dim u As Double, v As Double
      If face.ClosestPoint(gp.Point(), u, v) Then
        Dim direction = face.NormalAt(u, v)
        If face.OrientationIsReversed Then
          direction.Reverse()
        End If
        RhinoApp.WriteLine(String.Format("Surface normal at uv({0:f},{1:f}) = ({2:f},{3:f},{4:f})", u, v, direction.X, direction.Y, direction.Z))
      End If
      Return Result.Success
    End Function
  End Class
End Namespace