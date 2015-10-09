Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects
Imports Rhino.Input

Namespace examples_vb
  Public Class GetEdgeFacesCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbGetEdgeFaces"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select edge curve", False, ObjectType.EdgeFilter, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim edge = obj_ref.Edge()

      Dim face_idxs = edge.AdjacentFaces()
      Dim edge_owning_brep = edge.Brep

      For Each idx As Integer In face_idxs
        Dim face = edge_owning_brep.Faces(idx)
        Dim face_copy = face.DuplicateFace(True)
        Dim id = doc.Objects.AddBrep(face_copy)
        doc.Objects.Find(id).[Select](True)
      Next
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace