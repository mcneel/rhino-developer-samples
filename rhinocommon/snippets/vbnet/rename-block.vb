Partial Friend Class Examples
  Public Shared Function RenameBlock(ByVal doc As RhinoDoc) As Result
	' Get the name of the insance definition to rename
	Dim instance_definition_name = ""
	Dim rc = RhinoGet.GetString("Name of block to rename", True, instance_definition_name)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	If String.IsNullOrWhiteSpace(instance_definition_name) Then
	  Return Result.Nothing
	End If

	' Verify instance definition exists
	Dim instance_definition = doc.InstanceDefinitions.Find(instance_definition_name, True)
	If instance_definition Is Nothing Then
	  RhinoApp.WriteLine("Block ""{0}"" not found.", instance_definition_name)
	  Return Result.Nothing
	End If

	' Verify instance definition is rename-able
	If instance_definition.IsDeleted OrElse instance_definition.IsReference Then
	  RhinoApp.WriteLine("Unable to rename block ""{0}"".", instance_definition_name)
	  Return Result.Nothing
	End If

	' Get the new instance definition name
	Dim instance_definition_new_name As String = ""
	rc = RhinoGet.GetString("Name of block to rename", True, instance_definition_new_name)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	If String.IsNullOrWhiteSpace(instance_definition_new_name) Then
	  Return Result.Nothing
	End If

	' Verify the new instance definition name is not already in use
	Dim existing_instance_definition = doc.InstanceDefinitions.Find(instance_definition_new_name, True)
	If existing_instance_definition IsNot Nothing AndAlso Not existing_instance_definition.IsDeleted Then
	  RhinoApp.WriteLine("Block ""{0}"" already exists.", existing_instance_definition)
	  Return Result.Nothing
	End If

	' change the block name
	If Not doc.InstanceDefinitions.Modify(instance_definition.Index, instance_definition_new_name, instance_definition.Description, True) Then
	  RhinoApp.WriteLine("Could not rename {0} to {1}", instance_definition.Name, instance_definition_new_name)
	  Return Result.Failure
	End If

	Return Result.Success
  End Function
End Class
