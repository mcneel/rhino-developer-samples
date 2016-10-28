Partial Friend Class Examples
  Public Shared Function ReverseCurve(ByVal doc As RhinoDoc) As Result
	Dim obj_refs() As ObjRef = Nothing
	Dim rc = RhinoGet.GetMultipleObjects("Select curves to reverse", True, ObjectType.Curve, obj_refs)
	If rc IsNot Result.Success Then
	  Return rc
	End If

	For Each obj_ref In obj_refs
	  Dim curve_copy = obj_ref.Curve().DuplicateCurve()
	  If curve_copy IsNot Nothing Then
		curve_copy.Reverse()
		doc.Objects.Replace(obj_ref, curve_copy)
	  End If
	Next obj_ref
	Return Result.Success
  End Function
End Class
