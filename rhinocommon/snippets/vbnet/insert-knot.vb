Partial Friend Class Examples
  Public Shared Function InsertKnot(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Const filter As ObjectType = Rhino.DocObjects.ObjectType.Curve
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Result = Rhino.Input.RhinoGet.GetOneObject("Select curve for knot insertion", False, filter, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If
	Dim curve As Rhino.Geometry.Curve = objref.Curve()
	If Nothing Is curve Then
	  Return Rhino.Commands.Result.Failure
	End If
	Dim nurb As Rhino.Geometry.NurbsCurve = curve.ToNurbsCurve()
	If Nothing Is nurb Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim gp As New Rhino.Input.Custom.GetPoint()
	gp.SetCommandPrompt("Point on curve to add knot")
	gp.Constrain(nurb, False)
	gp.Get()
	If gp.CommandResult() = Rhino.Commands.Result.Success Then
	  Dim t As Double = Nothing
	  Dim crv As Rhino.Geometry.Curve = gp.PointOnCurve(t)
	  If crv IsNot Nothing AndAlso nurb.Knots.InsertKnot(t) Then
		doc.Objects.Replace(objref, nurb)
		doc.Views.Redraw()
	  End If
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
