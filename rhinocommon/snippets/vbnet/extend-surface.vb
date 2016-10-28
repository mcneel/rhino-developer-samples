Partial Friend Class Examples
  Public Shared Function ExtendSurface(ByVal doc As RhinoDoc) As Result
	Dim go = New Rhino.Input.Custom.GetObject()
	go.SetCommandPrompt("Select edge of surface to extend")
	go.GeometryFilter = ObjectType.EdgeFilter
	go.GeometryAttributeFilter = GeometryAttributeFilter.EdgeCurve
	go.Get()
	If go.CommandResult() <> Result.Success Then
	  Return go.CommandResult()
	End If
	Dim obj_ref = go.Object(0)

	Dim surface = obj_ref.Surface()
	If surface Is Nothing Then
	  RhinoApp.WriteLine("Unable to extend polysurfaces.")
	  Return Result.Failure
	End If

	Dim brep = obj_ref.Brep()
	Dim face = obj_ref.Face()
	If brep Is Nothing OrElse face Is Nothing Then
	  Return Result.Failure
	End If
	If face.FaceIndex < 0 Then
	  Return Result.Failure
	End If

	If Not brep.IsSurface Then
	  RhinoApp.WriteLine("Unable to extend trimmed surfaces.")
	  Return Result.Nothing
	End If

	Dim curve = obj_ref.Curve()

	Dim trim = obj_ref.Trim()
	If trim Is Nothing Then
	  Return Result.Failure
	End If

	If trim.TrimType = BrepTrimType.Seam Then
	  RhinoApp.WriteLine("Unable to extend surface at seam.")
	  Return Result.Nothing
	End If

	Dim extended_surface = surface.Extend(trim.IsoStatus, 5.0, True)
	If extended_surface IsNot Nothing Then
	  Dim mybrep = Brep.CreateFromSurface(extended_surface)
	  doc.Objects.Replace(obj_ref.ObjectId, mybrep)
	  doc.Views.Redraw()
	End If
	Return Result.Success
  End Function
End Class
