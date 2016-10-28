Partial Friend Class Examples
  Public Shared Function ExportControlPoints(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim get_rc = Rhino.Input.RhinoGet.GetOneObject("Select curve", False, Rhino.DocObjects.ObjectType.Curve, objref)
	If get_rc IsNot Rhino.Commands.Result.Success Then
	  Return get_rc
	End If
	Dim curve = objref.Curve()
	If curve Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If
	Dim nc = curve.ToNurbsCurve()

	Dim fd = New SaveFileDialog()
	'fd.Filters = "Text Files | *.txt";
	'fd.Filter = "Text Files | *.txt";
	'fd.DefaultExt = "txt";
	'if( fd.ShowDialog(Rhino.RhinoApp.MainWindow())!= System.Windows.Forms.DialogResult.OK)
	If fd.ShowDialog(Nothing) <> DialogResult.Ok Then
	  Return Rhino.Commands.Result.Cancel
	End If
	Dim path As String = fd.FileName
	Using sw As New System.IO.StreamWriter(path)
	  For Each pt In nc.Points
		Dim loc = pt.Location
		sw.WriteLine("{0} {1} {2}", loc.X, loc.Y, loc.Z)
	  Next pt
	  sw.Close()
	End Using
	Return Rhino.Commands.Result.Success
  End Function
End Class
