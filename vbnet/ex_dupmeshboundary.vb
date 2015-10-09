Imports Rhino
Imports Rhino.Commands
Imports Rhino.Input.Custom
Imports Rhino.Geometry
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class DupMeshBoundaryCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDupMeshBoundary"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim gm = New GetObject()
      gm.SetCommandPrompt("Select open mesh")
      gm.GeometryFilter = ObjectType.Mesh
      gm.GeometryAttributeFilter = GeometryAttributeFilter.OpenMesh
      gm.[Get]()
      If gm.CommandResult() <> Result.Success Then
        Return gm.CommandResult()
      End If
      Dim mesh = gm.[Object](0).Mesh()
      If mesh Is Nothing Then
        Return Result.Failure
      End If

      Dim polylines = mesh.GetNakedEdges()
      For Each polyline As Polyline In polylines
        doc.Objects.AddPolyline(polyline)
      Next

      Return Result.Success
    End Function
  End Class
End Namespace