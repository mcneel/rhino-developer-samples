Partial Friend Class Examples
  Public Shared Function Userdata(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select Face", False, Rhino.DocObjects.ObjectType.Surface, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim face = objref.Face()

	' See if user data of my custom type is attached to the geomtry
	' We need to use the underlying surface in order to get the user data
	' to serialize with the file.
	Dim ud = TryCast(face.UnderlyingSurface().UserData.Find(GetType(MyCustomData)), MyCustomData)
	If ud Is Nothing Then
	  ' No user data found; create one and add it
	  Dim i As Integer = 0
	  rc = Rhino.Input.RhinoGet.GetInteger("Integer Value", False, i)
	  If rc IsNot Rhino.Commands.Result.Success Then
		Return rc
	  End If

	  ud = New MyCustomData(i, "This is some text")
	  face.UnderlyingSurface().UserData.Add(ud)
	Else
	  RhinoApp.WriteLine("{0} = {1}", ud.Description, ud)
	End If
	Return Rhino.Commands.Result.Success
  End Function
End Class
