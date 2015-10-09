Imports Rhino
Imports Rhino.Commands

Namespace examples_vb
  Public Class RenameBlockCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbRenameInstanceDefinition"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      ' Get the name of the insance definition to rename
      Dim instanceDefinitionName As String = ""
      Dim rc = Rhino.Input.RhinoGet.GetString("Name of block to rename", True, instanceDefinitionName)
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

      ' Verify instance definition is rename-able
      If instanceDefinition.IsDeleted OrElse instanceDefinition.IsReference Then
        RhinoApp.WriteLine([String].Format("Unable to rename block ""{0}"".", instanceDefinitionName))
        Return Result.[Nothing]
      End If

      ' Get the new instance definition name
      Dim instanceDefinitionNewName As String = ""
      rc = Rhino.Input.RhinoGet.GetString("Name of block to rename", True, instanceDefinitionNewName)
      If rc <> Result.Success Then
        Return rc
      End If
      If [String].IsNullOrWhiteSpace(instanceDefinitionNewName) Then
        Return Result.[Nothing]
      End If

      ' Verify the new instance definition name is not already in use
      Dim existingInstanceDefinition = doc.InstanceDefinitions.Find(instanceDefinitionNewName, True)
      If existingInstanceDefinition IsNot Nothing AndAlso Not existingInstanceDefinition.IsDeleted Then
        RhinoApp.WriteLine([String].Format("Block ""{0}"" already exists.", existingInstanceDefinition))
        Return Result.[Nothing]
      End If

      ' change the block name
      If Not doc.InstanceDefinitions.Modify(instanceDefinition.Index, instanceDefinitionNewName, instanceDefinition.Description, True) Then
        RhinoApp.WriteLine([String].Format("Could not rename {0} to {1}", instanceDefinition.Name, instanceDefinitionNewName))
        Return Result.Failure
      End If

      Return Result.Success
    End Function
  End Class
End Namespace