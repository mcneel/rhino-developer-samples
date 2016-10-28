Imports Rhino
Imports Rhino.Commands
Imports Rhino.Display
Imports Rhino.Geometry
Imports Rhino.Input.Custom
Imports Rhino.DocObjects
Imports System.Drawing

Namespace examples_vb
  Public Class MeshDrawingCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbDrawMesh"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim gs = New GetObject()
      gs.SetCommandPrompt("select sphere")
      gs.GeometryFilter = ObjectType.Surface
      gs.DisablePreSelect()
      gs.SubObjectSelect = False
      gs.[Get]()
      If gs.CommandResult() <> Result.Success Then
        Return gs.CommandResult()
      End If

      Dim sphere As Sphere
      gs.[Object](0).Surface().TryGetSphere(sphere)
      If sphere.IsValid Then
        Dim mesh__1 = Mesh.CreateFromSphere(sphere, 10, 10)
        If mesh__1 Is Nothing Then
          Return Result.Failure
        End If
        Dim conduit = New DrawBlueMeshConduit(mesh__1)
        conduit.Enabled = True

        doc.Views.Redraw()

        Dim inStr As String = ""
        Rhino.Input.RhinoGet.GetString("press <Enter> to continue", True, inStr)

        conduit.Enabled = False
        doc.Views.Redraw()
        Return Result.Success
      Else
        Return Result.Failure
      End If
    End Function
  End Class

  Class DrawBlueMeshConduit
    Inherits DisplayConduit
    Private _mesh As Mesh = Nothing
    Private _color As Color
    Private _material As DisplayMaterial = Nothing
    Private _bbox As BoundingBox

    Public Sub New(mesh As Mesh)
      ' set up as much data as possible so we do the minimum amount of work possible inside
      ' the actual display code
      _mesh = mesh
      _color = System.Drawing.Color.Blue
      _material = New DisplayMaterial()
      _material.Diffuse = _color
      If _mesh IsNot Nothing AndAlso _mesh.IsValid Then
        _bbox = _mesh.GetBoundingBox(True)
      End If
    End Sub

    ' this is called every frame inside the drawing code so try to do as little as possible
    ' in order to not degrade display speed. Don't create new objects if you don't have to as this
    ' will incur an overhead on the heap and garbage collection.
    Protected Overrides Sub CalculateBoundingBox(e As CalculateBoundingBoxEventArgs)
      MyBase.CalculateBoundingBox(e)
      ' Since we are dynamically drawing geometry, we needed to override
      ' CalculateBoundingBox. Otherwise, there is a good chance that our
      ' dynamically drawing geometry would get clipped.

      ' Union the mesh's bbox with the scene's bounding box
      e.IncludeBoundingBox(_bbox)
    End Sub

    Protected Overrides Sub PreDrawObjects(e As DrawEventArgs)
      MyBase.PreDrawObjects(e)
      Dim vp = e.Display.Viewport
      If vp.DisplayMode.EnglishName.ToLower() = "wireframe" Then
        e.Display.DrawMeshWires(_mesh, _color)
      Else
        e.Display.DrawMeshShaded(_mesh, _material)
      End If
    End Sub
  End Class
End Namespace