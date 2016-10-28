Partial Friend Class Examples
  Public Shared Function CreateMeshFromBrep(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select surface or polysurface to mesh", True, ObjectType.Surface Or ObjectType.PolysrfFilter, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim brep = obj_ref.Brep()
	If Nothing Is brep Then
	  Return Result.Failure
	End If

	' you could choose anyone of these for example
	Dim jagged_and_faster = MeshingParameters.Coarse
	Dim smooth_and_slower = MeshingParameters.Smooth
	Dim default_mesh_params = MeshingParameters.Default
	Dim minimal = MeshingParameters.Minimal

	Dim meshes = Mesh.CreateFromBrep(brep, smooth_and_slower)
	If meshes Is Nothing OrElse meshes.Length = 0 Then
	  Return Result.Failure
	End If

	Dim brep_mesh = New Mesh()
	For Each mesh In meshes
	  brep_mesh.Append(mesh)
	Next mesh
	doc.Objects.AddMesh(brep_mesh)
	doc.Views.Redraw()

	Return Result.Success
  End Function
End Class
