Partial Friend Class Examples
  Public Shared Function UnrollSurface2(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Const filter As ObjectType = Rhino.DocObjects.ObjectType.Brep Or Rhino.DocObjects.ObjectType.Surface
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Result = Rhino.Input.RhinoGet.GetOneObject("Select surface or brep to unroll", False, filter, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If
	Dim unroll As Rhino.Geometry.Unroller=Nothing
	Dim brep As Rhino.Geometry.Brep = objref.Brep()
	Dim mesh As Rhino.Geometry.Mesh=Nothing
	If brep IsNot Nothing Then
	  unroll = New Rhino.Geometry.Unroller(brep)
	  mesh = brep.Faces(0).GetMesh(Rhino.Geometry.MeshType.Render)
	Else
	  Dim srf As Rhino.Geometry.Surface = objref.Surface()
	  If srf IsNot Nothing Then
		unroll = New Rhino.Geometry.Unroller(srf)
	  End If
	End If
	If unroll Is Nothing OrElse mesh Is Nothing Then
	  Return Rhino.Commands.Result.Cancel
	End If

	unroll.AddFollowingGeometry(mesh.Vertices.ToPoint3dArray())

	unroll.ExplodeOutput = False
	Dim curves() As Rhino.Geometry.Curve = Nothing
	Dim points() As Rhino.Geometry.Point3d = Nothing
	Dim dots() As Rhino.Geometry.TextDot = Nothing
	unroll.PerformUnroll(curves, points, dots)

	' change the mesh vertices to the flattened form and add it to the document
	If points.Length = mesh.Vertices.Count Then
	  For i As Integer = 0 To points.Length - 1
		mesh.Vertices.SetVertex(i, points(i))
	  Next i
	  mesh.Normals.ComputeNormals()
	End If
	doc.Objects.AddMesh(mesh, objref.Object().Attributes)
	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
