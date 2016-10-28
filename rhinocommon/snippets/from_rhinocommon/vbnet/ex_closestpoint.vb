Imports Rhino
Imports Rhino.Geometry

Namespace examples_vb
  <System.Runtime.InteropServices.Guid("B89D4A42-A712-4FA4-9ABF-6BE1FB962D24")> _
  Public Class RTreeClosestPoint
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vb_RtreeClosestPoint"
      End Get
    End Property

    Private Sub SearchCallback(sender As Object, e As RTreeEventArgs)
      Dim data As SearchData = TryCast(e.Tag, SearchData)
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
      Public Sub New(mesh__1 As Mesh, point__2 As Point3d)
        Point = point__2
        Mesh = mesh__1
        HitCount = 0
        Index = -1
        Distance = 0
      End Sub

      Public Property HitCount As Integer
      Public Property Point As Point3d
      Public Property Mesh As Mesh
      Public Property Index As Integer
      Public Property Distance As Double
    End Class

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As Rhino.Commands.RunMode) As Rhino.Commands.Result
      Dim objref As Rhino.DocObjects.ObjRef = Nothing
      Dim rc = Rhino.Input.RhinoGet.GetOneObject("select mesh", False, Rhino.DocObjects.ObjectType.Mesh, objref)
      If rc <> Rhino.Commands.Result.Success Then
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
        Next

        Dim point As Point3d
        While True
          rc = Rhino.Input.RhinoGet.GetPoint("test point", False, point)
          If rc <> Rhino.Commands.Result.Success Then
            Exit While
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
        End While
      End Using
      Return Rhino.Commands.Result.Success
    End Function
  End Class
End Namespace
