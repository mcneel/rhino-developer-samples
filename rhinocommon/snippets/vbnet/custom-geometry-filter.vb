Partial Friend Class Examples
  Private Shared m_tolerance As Double
  Public Shared Function CustomGeometryFilter(ByVal doc As RhinoDoc) As Result
	m_tolerance = doc.ModelAbsoluteTolerance

	' only use a custom geometry filter if no simpler filter does the job

	' only curves
	Dim gc = New GetObject()
	gc.SetCommandPrompt("select curve")
	gc.GeometryFilter = ObjectType.Curve
	gc.DisablePreSelect()
	gc.SubObjectSelect = False
	gc.Get()
	If gc.CommandResult() <> Result.Success Then
	  Return gc.CommandResult()
	End If
	If Nothing Is gc.Object(0).Curve() Then
	  Return Result.Failure
	End If
	Rhino.RhinoApp.WriteLine("curve was selected")

	' only closed curves
	Dim gcc = New GetObject()
	gcc.SetCommandPrompt("select closed curve")
	gcc.GeometryFilter = ObjectType.Curve
	gcc.GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve
	gcc.DisablePreSelect()
	gcc.SubObjectSelect = False
	gcc.Get()
	If gcc.CommandResult() <> Result.Success Then
	  Return gcc.CommandResult()
	End If
	If Nothing Is gcc.Object(0).Curve() Then
	  Return Result.Failure
	End If
	Rhino.RhinoApp.WriteLine("closed curve was selected")

	' only circles with a radius of 10
	Dim gcc10 = New GetObject()
	gcc10.SetCommandPrompt("select circle with radius of 10")
	gc.GeometryFilter = ObjectType.Curve
	gcc10.SetCustomGeometryFilter(AddressOf CircleWithRadiusOf10GeometryFilter) ' custom geometry filter
	gcc10.DisablePreSelect()
	gcc10.SubObjectSelect = False
	gcc10.Get()
	If gcc10.CommandResult() <> Result.Success Then
	  Return gcc10.CommandResult()
	End If
	If Nothing Is gcc10.Object(0).Curve() Then
	  Return Result.Failure
	End If
	RhinoApp.WriteLine("circle with radius of 10 was selected")

	Return Result.Success
  End Function

  Private Shared Function CircleWithRadiusOf10GeometryFilter(ByVal rhObject As Rhino.DocObjects.RhinoObject, ByVal geometry As GeometryBase, ByVal componentIndex As ComponentIndex) As Boolean
	Dim is_circle_with_radius_of10 As Boolean = False
	Dim circle As Circle = Nothing
	If TypeOf geometry Is Curve AndAlso (TryCast(geometry, Curve)).TryGetCircle(circle) Then
	  is_circle_with_radius_of10 = circle.Radius <= 10.0 + m_tolerance AndAlso circle.Radius >= 10.0 - m_tolerance
	End If
	Return is_circle_with_radius_of10
  End Function
End Class
