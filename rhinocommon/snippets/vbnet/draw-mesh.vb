Partial Friend Class Examples
  Public Shared Function DrawMesh(ByVal doc As RhinoDoc) As Result
	Dim gs = New GetObject()
	gs.SetCommandPrompt("select sphere")
	gs.GeometryFilter = ObjectType.Surface
	gs.DisablePreSelect()
	gs.SubObjectSelect = False
	gs.Get()
	If gs.CommandResult() <> Result.Success Then
	  Return gs.CommandResult()
	End If

	Dim sphere As Sphere = Nothing
	gs.Object(0).Surface().TryGetSphere(sphere)
	If sphere.IsValid Then
	  Dim mesh = Mesh.CreateFromSphere(sphere, 10, 10)
	  If mesh Is Nothing Then
		Return Result.Failure
	  End If

	  Dim conduit = New DrawBlueMeshConduit(mesh) With {.Enabled = True}
	  doc.Views.Redraw()

	  Dim in_str = ""
	  Rhino.Input.RhinoGet.GetString("press <Enter> to continue", True, in_str)

	  conduit.Enabled = False
	  doc.Views.Redraw()
	  Return Result.Success
	Else
	  Return Result.Failure
	End If
  End Function
End Class

Friend Class DrawBlueMeshConduit
	Inherits DisplayConduit

  Private ReadOnly m_mesh As Mesh
  Private ReadOnly m_color As Color
  Private ReadOnly m_material As DisplayMaterial
  Private ReadOnly m_bbox As BoundingBox

  Public Sub New(ByVal mesh As Mesh)
	' set up as much data as possible so we do the minimum amount of work possible inside
	' the actual display code
	m_mesh = mesh
	m_color = System.Drawing.Color.Blue
	m_material = New DisplayMaterial()
	m_material.Diffuse = m_color
	If m_mesh IsNot Nothing AndAlso m_mesh.IsValid Then
	  m_bbox = m_mesh.GetBoundingBox(True)
	End If
  End Sub

  ' this is called every frame inside the drawing code so try to do as little as possible
  ' in order to not degrade display speed. Don't create new objects if you don't have to as this
  ' will incur an overhead on the heap and garbage collection.
  Protected Overrides Sub CalculateBoundingBox(ByVal e As CalculateBoundingBoxEventArgs)
	MyBase.CalculateBoundingBox(e)
	' Since we are dynamically drawing geometry, we needed to override
	' CalculateBoundingBox. Otherwise, there is a good chance that our
	' dynamically drawing geometry would get clipped.

	' Union the mesh's bbox with the scene's bounding box
	e.BoundingBox.Union(m_bbox)
  End Sub

  Protected Overrides Sub PreDrawObjects(ByVal e As DrawEventArgs)
	MyBase.PreDrawObjects(e)
	Dim vp = e.Display.Viewport
	If vp.DisplayMode.EnglishName.ToLower() = "wireframe" Then
	  e.Display.DrawMeshWires(m_mesh, m_color)
	Else
	  e.Display.DrawMeshShaded(m_mesh, m_material)
	End If
  End Sub
End Class
