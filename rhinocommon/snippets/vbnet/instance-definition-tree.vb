Partial Friend Class Examples
  Public Shared Function InstanceDefinitionTree(ByVal doc As RhinoDoc) As Result
	Dim instance_definitions = doc.InstanceDefinitions
	Dim instance_definition_count = instance_definitions.Count

	If instance_definition_count = 0 Then
	  RhinoApp.WriteLine("Document contains no instance definitions.")
	  Return Result.Nothing
	End If

	Dim dump = New TextLog()
	dump.IndentSize = 4

	For i As Integer = 0 To instance_definition_count - 1
	  DumpInstanceDefinition(instance_definitions(i), dump, True)
	Next i

	RhinoApp.WriteLine(dump.ToString())

	Return Result.Success
  End Function

  Private Shared Sub DumpInstanceDefinition(ByVal instanceDefinition As InstanceDefinition, ByRef dump As TextLog, ByVal isRoot As Boolean)
	If instanceDefinition IsNot Nothing AndAlso Not instanceDefinition.IsDeleted Then
	  Dim node As String = If(isRoot, "─", "└")
	  dump.Print(String.Format("{0} Instance definition {1} = {2}" & ControlChars.Lf, node, instanceDefinition.Index, instanceDefinition.Name))

	  If instanceDefinition.ObjectCount > 0 Then
		dump.PushIndent()
		For i As Integer = 0 To instanceDefinition.ObjectCount - 1
		  Dim obj = instanceDefinition.Object(i)
		  If obj Is Nothing Then
			  Continue For
		  End If
		  If TypeOf obj Is InstanceObject Then
			DumpInstanceDefinition((TryCast(obj, InstanceObject)).InstanceDefinition, dump, False) ' Recursive...
		  Else
			dump.Print(ChrW(&H2514).ToString() & " Object {0} = {1}" & ControlChars.Lf, i, obj.ShortDescription(False))
		  End If
		Next i
		dump.PopIndent()
	  End If
	End If
  End Sub
End Class
