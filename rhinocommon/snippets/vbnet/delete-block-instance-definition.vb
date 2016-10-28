Partial Friend Class Examples
  Public Shared Function DeleteBlock(ByVal doc As RhinoDoc) As Result
	' Get the name of the instance definition to rename
	Dim instance_definition_name As String = ""
	Dim rc = RhinoGet.GetString("Name of block to delete", True, instance_definition_name)
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

	' Verify instance definition can be deleted
	If instance_definition.IsReference Then
	  RhinoApp.WriteLine("Unable to delete block ""{0}"".", instance_definition_name)
	  Return Result.Nothing
	End If

	' delete block and all references
	If Not doc.InstanceDefinitions.Delete(instance_definition.Index, True, True) Then
	  RhinoApp.WriteLine("Could not delete {0} block", instance_definition.Name)
	  Return Result.Failure
	End If

	Return Result.Success
  End Function
End Class
