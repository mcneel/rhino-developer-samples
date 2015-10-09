Imports Rhino
Imports Rhino.Commands

Namespace examples_vb
  Public Class DeleteBlockCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDeleteInstanceDefinition"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      ' Get the name of the instance definition to rename
      Dim instanceDefinitionName As String = ""
      Dim rc = Rhino.Input.RhinoGet.GetString("Name of block to delete", True, instanceDefinitionName)
      If rc <> Result.Success Then
        Return rc
      End If
      If [String].IsNullOrWhiteSpace(instanceDefinitionName) Then
        Return Result.[Nothing]
      End If

      ' Verify instance definition exists
      Dim instanceDefinition = doc.InstanceDefinitions.Find(instanceDefinitionName, True)
      If instanceDefinition Is Nothing Then
        RhinoApp.WriteLine([String].Format("Block ""{0}"" not found.", instanceDefinitionName))
        Return Result.[Nothing]
      End If

      ' Verify instance definition can be deleted
      If instanceDefinition.IsReference Then
        RhinoApp.WriteLine([String].Format("Unable to delete block ""{0}"".", instanceDefinitionName))
        Return Result.[Nothing]
      End If

      ' delete block and all references
      If Not doc.InstanceDefinitions.Delete(instanceDefinition.Index, True, True) Then
        RhinoApp.WriteLine([String].Format("Could not delete {0} block", instanceDefinition.Name))
        Return Result.Failure
      End If

      Return Result.Success
    End Function
  End Class
End Namespace