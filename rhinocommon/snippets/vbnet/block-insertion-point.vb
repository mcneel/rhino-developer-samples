Partial Friend Class Examples
  Public Shared Function BlockInsertionPoint(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Result = Rhino.Input.RhinoGet.GetOneObject("Select instance", True, Rhino.DocObjects.ObjectType.InstanceReference, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If
	Dim instance As Rhino.DocObjects.InstanceObject = TryCast(objref.Object(), Rhino.DocObjects.InstanceObject)
	If instance IsNot Nothing Then
	  Dim pt As Rhino.Geometry.Point3d = instance.InsertionPoint
	  doc.Objects.AddPoint(pt)
	  doc.Views.Redraw()
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
