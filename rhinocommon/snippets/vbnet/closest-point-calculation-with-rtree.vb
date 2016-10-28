Partial Friend Class Examples
  Private Shared Sub SearchCallback(ByVal sender As Object, ByVal e As RTreeEventArgs)
	Dim data As SearchData = TryCast(e.Tag, SearchData)
	If data Is Nothing Then
	  Return
	End If
	data.HitCount = data.HitCount + 1
	Dim vertex As Point3f = data.Mesh.Vertices(e.Id)
	Dim distance As Double = data.Point.DistanceTo(vertex)
	If data.Index = -1 OrElse data.Distance > distance Then
	  ' shrink the sphere to help improve the test
	  e.SearchSphere = New Sphere(data.Point, distance)
	  data.Index = e.Id
	  data.Distance = distance
	End If
  End Sub

  Private Class SearchData
	Public Sub New(ByVal mesh As Mesh, ByVal point As Point3d)
	  Me.Point = point
	  Me.Mesh = mesh
	  HitCount = 0
	  Index = -1
	  Distance = 0
	End Sub

	Public Property HitCount() As Integer
	Private privatePoint As Point3d
	Public Property Point() As Point3d
		Get
			Return privatePoint
		End Get
		Private Set(ByVal value As Point3d)
			privatePoint = value
		End Set
	End Property
	Private privateMesh As Mesh
	Public Property Mesh() As Mesh
		Get
			Return privateMesh
		End Get
		Private Set(ByVal value As Mesh)
			privateMesh = value
		End Set
	End Property
	Public Property Index() As Integer
	Public Property Distance() As Double
  End Class

  Public Shared Function RTreeClosestPoint(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetOneObject("select mesh", False, Rhino.DocObjects.ObjectType.Mesh, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim mesh As Mesh = objref.Mesh()
	objref.Object().Select(False)
	doc.Views.Redraw()

	Using tree As New RTree()
	  For i As Integer = 0 To mesh.Vertices.Count - 1
		' we can make a C++ function that just builds an rtree from the
		' vertices in one quick shot, but for now...
		tree.Insert(mesh.Vertices(i), i)
	  Next i

	  Do
		Dim point As Point3d = Nothing
		rc = Rhino.Input.RhinoGet.GetPoint("test point", False, point)
		If rc IsNot Rhino.Commands.Result.Success Then
		  Exit Do
		End If

		Dim data As New SearchData(mesh, point)
		' Use the first vertex in the mesh to define a start sphere
		Dim distance As Double = point.DistanceTo(mesh.Vertices(0))
		Dim sphere As New Sphere(point, distance * 1.1)
		If tree.Search(sphere, AddressOf SearchCallback, data) Then
		  doc.Objects.AddPoint(mesh.Vertices(data.Index))
		  doc.Views.Redraw()
		  RhinoApp.WriteLine("Found point in {0} tests", data.HitCount)
		End If
	  Loop
	End Using
	Return Rhino.Commands.Result.Success
  End Function
End Class
