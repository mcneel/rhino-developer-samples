Partial Friend Class Examples
  Public Shared Function ExplodeHatch(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Const filter As ObjectType = Rhino.DocObjects.ObjectType.Hatch
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetOneObject("Select hatch to explode", False, filter, objref)
	If rc IsNot Rhino.Commands.Result.Success OrElse objref Is Nothing Then
	  Return rc
	End If

	Dim hatch As Rhino.Geometry.Hatch = TryCast(objref.Geometry(), Rhino.Geometry.Hatch)
	If Nothing Is hatch Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim hatch_geom() As Rhino.Geometry.GeometryBase = hatch.Explode()
	If Nothing IsNot hatch_geom Then
	  For i As Integer = 0 To hatch_geom.Length - 1
		Dim geom As Rhino.Geometry.GeometryBase = hatch_geom(i)
		If Nothing IsNot geom Then
		  Select Case geom.ObjectType
			Case Rhino.DocObjects.ObjectType.Point
				Dim point As Rhino.Geometry.Point = TryCast(geom, Rhino.Geometry.Point)
				If Nothing IsNot point Then
				  doc.Objects.AddPoint(point.Location)
				End If
			Case Rhino.DocObjects.ObjectType.Curve
				Dim curve As Rhino.Geometry.Curve = TryCast(geom, Rhino.Geometry.Curve)
				If Nothing IsNot curve Then
				  doc.Objects.AddCurve(curve)
				End If
			Case Rhino.DocObjects.ObjectType.Brep
				Dim brep As Rhino.Geometry.Brep = TryCast(geom, Rhino.Geometry.Brep)
				If Nothing IsNot brep Then
				  doc.Objects.AddBrep(brep)
				End If
		  End Select
		End If
	  Next i
	End If

	Return Rhino.Commands.Result.Success
  End Function
End Class
