Partial Friend Class Examples
  Public Shared Function UnrollSurface(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Const filter As ObjectType = Rhino.DocObjects.ObjectType.Brep Or Rhino.DocObjects.ObjectType.Surface
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc As Result = Rhino.Input.RhinoGet.GetOneObject("Select surface or brep to unroll", False, filter, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If
	Dim unroll As Rhino.Geometry.Unroller=Nothing
	Dim brep As Rhino.Geometry.Brep = objref.Brep()
	If brep IsNot Nothing Then
	  unroll = New Rhino.Geometry.Unroller(brep)
	Else
	  Dim srf As Rhino.Geometry.Surface = objref.Surface()
	  If srf IsNot Nothing Then
		unroll = New Rhino.Geometry.Unroller(srf)
	  End If
	End If
	If unroll Is Nothing Then
	  Return Rhino.Commands.Result.Cancel
	End If

	unroll.AbsoluteTolerance = 0.01
	unroll.RelativeTolerance = 0.01

	Dim go As New Rhino.Input.Custom.GetObject()
	go.SetCommandPrompt("Select points, curves, and dots to unroll with surface")
	go.GeometryFilter = Rhino.DocObjects.ObjectType.Point Or Rhino.DocObjects.ObjectType.Curve Or Rhino.DocObjects.ObjectType.TextDot
	go.AcceptNothing(True)
	go.GetMultiple(0, 0)
	If go.CommandResult() <> Rhino.Commands.Result.Success Then
	  Return go.CommandResult()
	End If
	For i As Integer = 0 To go.ObjectCount - 1
	  objref = go.Object(i)
	  Dim g As Rhino.Geometry.GeometryBase = objref.Geometry()
	  Dim pt As Rhino.Geometry.Point = TryCast(g, Rhino.Geometry.Point)
	  Dim crv As Rhino.Geometry.Curve = TryCast(g, Rhino.Geometry.Curve)
	  Dim dot As Rhino.Geometry.TextDot = TryCast(g, Rhino.Geometry.TextDot)
	  If pt IsNot Nothing Then
		unroll.AddFollowingGeometry(pt.Location)
	  ElseIf crv IsNot Nothing Then
		unroll.AddFollowingGeometry(crv)
	  ElseIf dot IsNot Nothing Then
		unroll.AddFollowingGeometry(dot)
	  End If
	Next i

	unroll.ExplodeOutput = False
	Dim curves() As Rhino.Geometry.Curve = Nothing
	Dim points() As Rhino.Geometry.Point3d = Nothing
	Dim dots() As Rhino.Geometry.TextDot = Nothing
	Dim breps() As Rhino.Geometry.Brep = unroll.PerformUnroll(curves, points, dots)
	If breps Is Nothing OrElse breps.Length < 1 Then
	  Return Rhino.Commands.Result.Failure
	End If

	For i As Integer = 0 To breps.Length - 1
	  doc.Objects.AddBrep(breps(i))
	Next i
	For i As Integer = 0 To curves.Length - 1
	  doc.Objects.AddCurve(curves(i))
	Next i
	doc.Objects.AddPoints(points)
	For i As Integer = 0 To dots.Length - 1
	  doc.Objects.AddTextDot(dots(i))
	Next i
	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
