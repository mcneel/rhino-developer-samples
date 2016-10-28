Partial Friend Class Examples
  Public Shared Function InstanceDefinitionNames(ByVal doc As RhinoDoc) As Result
	Dim instance_definition_names = (
	    From instance_definition In doc.InstanceDefinitions
	    Where instance_definition IsNot Nothing AndAlso Not instance_definition.IsDeleted
	    Select instance_definition.Name)

	For Each n In instance_definition_names
	  RhinoApp.WriteLine("Instance definition = {0}", n)
	Next n

	Return Result.Success
  End Function
End Class
