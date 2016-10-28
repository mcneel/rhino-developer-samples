Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Geometry

Namespace examples_vb
  Public Class ChangeDimensionStyleCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbChangeDimensionStyle"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      For Each rhino_object As RhinoObject In doc.Objects.GetObjectList(ObjectType.Annotation)
        Dim annotation_object = TryCast(rhino_object, AnnotationObjectBase)
        If annotation_object Is Nothing Then
          Continue For
        End If

        Dim annotation = TryCast(annotation_object.Geometry, AnnotationBase)
        If annotation Is Nothing Then
          Continue For
        End If

        If annotation.Index = doc.DimStyles.CurrentDimensionStyleIndex Then
          Continue For
        End If

        annotation.Index = doc.DimStyles.CurrentDimensionStyleIndex
        annotation_object.CommitChanges()
      Next

      doc.Views.Redraw()

      Return Result.Success
    End Function
  End Class
End Namespace