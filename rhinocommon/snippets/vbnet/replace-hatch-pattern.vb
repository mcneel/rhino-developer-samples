Partial Friend Class Examples
  Public Shared Function ReplaceHatchPattern(ByVal doc As RhinoDoc) As Result
	Dim obj_refs() As ObjRef = Nothing
	Dim rc = RhinoGet.GetMultipleObjects("Select hatches to replace", False, ObjectType.Hatch, obj_refs)
	If rc IsNot Result.Success OrElse obj_refs Is Nothing Then
	  Return rc
	End If

	Dim gs = New GetString()
	gs.SetCommandPrompt("Name of replacement hatch pattern")
	gs.AcceptNothing(False)
	gs.Get()
	If gs.CommandResult() <> Result.Success Then
	  Return gs.CommandResult()
	End If
	Dim hatch_name = gs.StringResult()

	Dim pattern_index = doc.HatchPatterns.Find(hatch_name, True)

	If pattern_index < 0 Then
	  RhinoApp.WriteLine("The hatch pattern ""{0}"" not found  in the document.", hatch_name)
	  Return Result.Nothing
	End If

	For Each obj_ref In obj_refs
	  Dim hatch_object = TryCast(obj_ref.Object(), HatchObject)
	  If hatch_object.HatchGeometry.PatternIndex IsNot pattern_index Then
		hatch_object.HatchGeometry.PatternIndex = pattern_index
		hatch_object.CommitChanges()
	  End If
	Next obj_ref
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
