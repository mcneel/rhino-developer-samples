Imports Rhino
Imports Rhino.Commands
Imports Rhino.Geometry
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class CreateMeshFromBrepCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbCreateMeshFromBrep"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim objRef As ObjRef = Nothing
      Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface to mesh", True, ObjectType.Surface Or ObjectType.PolysrfFilter, objRef)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim brep = objRef.Brep()
      If brep Is Nothing Then
        Return Result.Failure
      End If

      ' you could choose any one of these for example
      Dim jaggedAndFaster = MeshingParameters.Coarse
      Dim smoothAndSlower = MeshingParameters.Smooth
      Dim defaultMeshParams = MeshingParameters.Default
      Dim minimal = MeshingParameters.Minimal

      Dim meshes = Mesh.CreateFromBrep(brep, smoothAndSlower)
      If meshes Is Nothing OrElse meshes.Length = 0 Then
        Return Result.Failure
      End If

      Dim brepmesh = New Mesh()
      For Each facemesh As Mesh In meshes
        brepmesh.Append(facemesh)
      Next

      doc.Objects.AddMesh(brepmesh)
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace