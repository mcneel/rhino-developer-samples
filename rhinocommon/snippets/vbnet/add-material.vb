Partial Friend Class Examples
  Public Shared Function AddMaterial(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	' materials are stored in the document's material table
	Dim index As Integer = doc.Materials.Add()
	Dim mat As Rhino.DocObjects.Material = doc.Materials(index)
	mat.DiffuseColor = System.Drawing.Color.Chocolate
	mat.SpecularColor = System.Drawing.Color.CadetBlue
	mat.CommitChanges()

	' set up object attributes to say they use a specific material
	Dim sp As New Rhino.Geometry.Sphere(Rhino.Geometry.Plane.WorldXY, 5)
	Dim attr As New Rhino.DocObjects.ObjectAttributes()
	attr.MaterialIndex = index
	attr.MaterialSource = Rhino.DocObjects.ObjectMaterialSource.MaterialFromObject
	doc.Objects.AddSphere(sp, attr)

	' add a sphere without the material attributes set
	sp.Center = New Rhino.Geometry.Point3d(10, 10, 0)
	doc.Objects.AddSphere(sp)

	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
