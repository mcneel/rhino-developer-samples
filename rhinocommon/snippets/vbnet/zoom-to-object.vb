Partial Friend Class Examples
  Public Shared Function ZoomToObject(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim rhObject As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select object to zoom", False, Rhino.DocObjects.ObjectType.None, rhObject)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim obj = rhObject.Object()
	Dim view = doc.Views.ActiveView
	If obj Is Nothing OrElse view Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim bbox = obj.Geometry.GetBoundingBox(True)

	Const pad As Double = 0.05 ' A little padding...
	Dim dx As Double = (bbox.Max.X - bbox.Min.X) * pad
	Dim dy As Double = (bbox.Max.Y - bbox.Min.Y) * pad
	Dim dz As Double = (bbox.Max.Z - bbox.Min.Z) * pad
	bbox.Inflate(dx, dy, dz)
	view.ActiveViewport.ZoomBoundingBox(bbox)
	view.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
