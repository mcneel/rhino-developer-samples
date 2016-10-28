Partial Friend Class Examples
  Public Shared Function BoxShell(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim box As Rhino.Geometry.Box = Nothing
	Dim rc As Rhino.Commands.Result = Rhino.Input.RhinoGet.GetBox(box)
	If rc Is Rhino.Commands.Result.Success Then
	  Dim brep As Rhino.Geometry.Brep = Rhino.Geometry.Brep.CreateFromBox(box)
	  If Nothing IsNot brep Then
		Dim facesToRemove As New System.Collections.Generic.List(Of Integer)(1)
		facesToRemove.Add(0)
		Dim shells() As Rhino.Geometry.Brep = Rhino.Geometry.Brep.CreateShell(brep, facesToRemove, 1.0, doc.ModelAbsoluteTolerance)
		If Nothing IsNot shells Then
		  For i As Integer = 0 To shells.Length - 1
			doc.Objects.AddBrep(shells(i))
		  Next i
		  doc.Views.Redraw()
		End If
	  End If
	End If
	Return rc
  End Function
End Class
