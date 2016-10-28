Partial Friend Class Examples
  Private Shared Function IsBrepBox(ByVal brep As Rhino.Geometry.Brep) As Boolean
	Const zero_tolerance As Double = 1.0e-6 ' or whatever
	Dim rc As Boolean = brep.IsSolid
	If rc Then
	  rc = brep.Faces.Count = 6
	End If

	Dim N = New Rhino.Geometry.Vector3d(5){}
	Dim i As Integer = 0
	Do While rc AndAlso i < 6
	  Dim plane As Rhino.Geometry.Plane = Nothing
	  rc = brep.Faces(i).TryGetPlane(plane, zero_tolerance)
	  If rc Then
		N(i) = plane.ZAxis
		N(i).Unitize()
	  End If
		i += 1
	Loop

	i = 0
	Do While rc AndAlso i < 6
	  Dim count As Integer = 0
	  Dim j As Integer = 0
	  Do While rc AndAlso j < 6
		Dim dot As Double = Math.Abs(N(i) * N(j))
		If dot <= zero_tolerance Then
		  j += 1
		  Continue Do
		End If
		If Math.Abs(dot - 1.0) <= zero_tolerance Then
		  count += 1
		Else
		  rc = False
		End If
		  j += 1
	  Loop

	  If rc Then
		If 2 <> count Then
		  rc = False
		End If
	  End If
		i += 1
	Loop
	Return rc
  End Function

  Public Shared Function IsBrepBox(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim obj_ref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select Brep", True, Rhino.DocObjects.ObjectType.Brep, obj_ref)
	If rc Is Rhino.Commands.Result.Success Then
	  Dim brep = obj_ref.Brep()
	  If brep IsNot Nothing Then
		Rhino.RhinoApp.WriteLine(If(IsBrepBox(brep), "Yes it is a box", "No it is not a box"))
	  End If
	End If
	Return rc
  End Function
End Class
