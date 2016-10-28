Partial Friend Class Examples
  Public Shared Function IsocurveDensity(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select surface", False, Rhino.DocObjects.ObjectType.Surface, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim brep_obj = TryCast(objref.Object(), Rhino.DocObjects.BrepObject)
	If brep_obj IsNot Nothing Then
	  brep_obj.Attributes.WireDensity = 3
	  brep_obj.CommitChanges()
	  doc.Views.Redraw()
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
