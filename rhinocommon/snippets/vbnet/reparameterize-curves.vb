Partial Friend Class Examples
  Public Shared Function ReparameterizeCurve(ByVal doc As RhinoDoc) As Result
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject("Select curve to reparameterize", False, ObjectType.Curve, obj_ref)
	If rc IsNot Result.Success Then
	  Return rc
	End If
	Dim curve = obj_ref.Curve()
	If curve Is Nothing Then
	  Return Result.Failure
	End If

	Dim domain_start As Double = 0
	rc = RhinoGet.GetNumber("Domain start", False, domain_start)
	If rc IsNot Result.Success Then
	  Return rc
	End If

	Dim domain_end As Double = 0
	rc = RhinoGet.GetNumber("Domain end", False, domain_end)
	If rc IsNot Result.Success Then
	  Return rc
	End If

	If Math.Abs(curve.Domain.T0 - domain_start) < RhinoMath.ZeroTolerance AndAlso Math.Abs(curve.Domain.T1 - domain_end) < RhinoMath.ZeroTolerance Then
	  Return Result.Nothing
	End If

	Dim curve_copy = curve.DuplicateCurve()
	curve_copy.Domain = New Interval(domain_start, domain_end)
	If Not doc.Objects.Replace(obj_ref, curve_copy) Then
	  Return Result.Failure
	Else
	  doc.Views.Redraw()
	  Return Result.Success
	End If
  End Function
End Class
