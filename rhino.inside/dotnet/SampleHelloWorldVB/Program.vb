Imports System
Imports System.IO
Imports System.Reflection
Imports Rhino.Runtime.InProcess
Imports Rhino.Geometry

Namespace HelloWorld
  Class Program

    <System.STAThread>
    Public Shared Sub Main(ByVal args As String())

      Try
        Using New RhinoCore(args)
          MeshABrep()
          Console.WriteLine("press any key to exit")
          Console.ReadKey()
        End Using

      Catch ex As Exception
        Console.[Error].WriteLine(ex.Message)
      End Try
    End Sub

    Private Shared Sub MeshABrep()
      Dim sphere = New Sphere(Point3d.Origin, 12)
      Dim brep = sphere.ToBrep()
      Dim mp = New MeshingParameters(0.5)
      Dim mesh = Rhino.Geometry.Mesh.CreateFromBrep(brep, mp)
      Console.WriteLine($"Mesh with {mesh(0).Vertices.Count} vertices created")
    End Sub
  End Class
End Namespace
