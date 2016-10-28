Partial Friend Class Examples
  Public Shared Function ArcLengthPoint(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select curve", True, Rhino.DocObjects.ObjectType.Curve,objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If
	Dim crv As Rhino.Geometry.Curve = objref.Curve()
	If crv Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim crv_length As Double = crv.GetLength()
	Dim length As Double = 0
	rc = Rhino.Input.RhinoGet.GetNumber("Length from start", True, length, 0, crv_length)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim pt As Rhino.Geometry.Point3d = crv.PointAtLength(length)
	If pt.IsValid Then
	  doc.Objects.AddPoint(pt)
	  doc.Views.Redraw()
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
