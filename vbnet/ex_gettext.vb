Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input.Custom

Namespace examples_vb
  Public Class ReadDimensionTextCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbReadDimensionText"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim go = New GetObject()
      go.SetCommandPrompt("Select annotation")
      go.GeometryFilter = ObjectType.Annotation
      go.[Get]()
      If go.CommandResult() <> Result.Success Then
        Return Result.Failure
      End If
      Dim annotation = TryCast(go.[Object](0).[Object](), AnnotationObjectBase)
      If annotation Is Nothing Then
        Return Result.Failure
      End If

      RhinoApp.WriteLine("Annotation text = {0}", annotation.DisplayText)

      Return Result.Success
    End Function
  End Class
End Namespace