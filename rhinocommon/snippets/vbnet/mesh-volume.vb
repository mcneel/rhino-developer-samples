Partial Friend Class Examples
  Public Shared Function MeshVolume(ByVal doc As RhinoDoc) As Result
	Dim gm = New GetObject()
	gm.SetCommandPrompt("Select solid meshes for volume calculation")
	gm.GeometryFilter = ObjectType.Mesh
	gm.GeometryAttributeFilter = GeometryAttributeFilter.ClosedMesh
	gm.SubObjectSelect = False
	gm.GroupSelect = True
	gm.GetMultiple(1, 0)
	If gm.CommandResult() <> Result.Success Then
	  Return gm.CommandResult()
	End If

	Dim volume As Double = 0.0
	Dim volume_error As Double = 0.0
	For Each obj_ref In gm.Objects()
	  If obj_ref.Mesh() IsNot Nothing Then
		Dim mass_properties = VolumeMassProperties.Compute(obj_ref.Mesh())
		If mass_properties IsNot Nothing Then
		  volume += mass_properties.Volume
		  volume_error += mass_properties.VolumeError
		End If
	  End If
	Next obj_ref

	RhinoApp.WriteLine("Total volume = {0:f} (+/- {1:f})", volume, volume_error)
	Return Result.Success
  End Function
End Class
