Partial Friend Class Examples
  Public Shared Function ExtractRenderMesh(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objRef As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface", False, Rhino.DocObjects.ObjectType.Brep, objRef)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim obj As Rhino.DocObjects.RhinoObject = objRef.Object()
	If Nothing Is obj Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim objList As New System.Collections.Generic.List(Of Rhino.DocObjects.RhinoObject)(1)
	objList.Add(obj)

	Dim meshObjRefs() As Rhino.DocObjects.ObjRef = Rhino.DocObjects.RhinoObject.GetRenderMeshes(objList, True, False)
	If Nothing IsNot meshObjRefs Then
	  For i As Integer = 0 To meshObjRefs.Length - 1
		Dim meshObjRef As Rhino.DocObjects.ObjRef = meshObjRefs(i)
		If Nothing IsNot meshObjRef Then
		  Dim mesh As Rhino.Geometry.Mesh = meshObjRef.Mesh()
		  If Nothing IsNot mesh Then
			doc.Objects.AddMesh(mesh)
		  End If
		End If
	  Next i
	  doc.Views.Redraw()
	End If

	Return Rhino.Commands.Result.Success
  End Function
End Class
