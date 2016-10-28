Partial Friend Class Examples
  Private Shared m_line_objects As New List(Of RhinoObject)()

  Public Shared Function DisplayOrder(ByVal doc As RhinoDoc) As Result
	' make lines thick so draw order can be easily seen
	Dim dm = DisplayModeDescription.GetDisplayModes().Single(Function(x) x.EnglishName = "Wireframe")
	Dim original_thikcness = dm.DisplayAttributes.CurveThickness
	dm.DisplayAttributes.CurveThickness = 10
	DisplayModeDescription.UpdateDisplayMode(dm)

	AddLine(Point3d.Origin, New Point3d(10,10,0), Color.Red, doc)
	AddLine(New Point3d(10,0,0), New Point3d(0,10,0), Color.Blue, doc)
	AddLine(New Point3d(8,0,0), New Point3d(8,10,0), Color.Green, doc)
	AddLine(New Point3d(0,3,0), New Point3d(10,3,0), Color.Yellow, doc)
	doc.Views.Redraw()
	Pause("draw order: 1st line drawn in front, last line drawn in the back.  Any key to continue ...")

	'all objects have a DisplayOrder of 0 by default so changing it to 1 moves it to the front.  Here we move the 2nd line (blue) to the front
	m_line_objects(1).Attributes.DisplayOrder = 1
	m_line_objects(1).CommitChanges()
	doc.Views.Redraw()
	Pause("Second (blue) line now in front.  Any key to continue ...")

	For i As Integer = 0 To m_line_objects.Count - 1
	  m_line_objects(i).Attributes.DisplayOrder = i
	  m_line_objects(i).CommitChanges()
	Next i
	doc.Views.Redraw()
	Pause("Reverse order of original lines, i.e., Yellow 1st and Red last.  Any key to continue ...")

	' restore original line thickness
	dm.DisplayAttributes.CurveThickness = original_thikcness
	DisplayModeDescription.UpdateDisplayMode(dm)

	doc.Views.Redraw()
	Return Result.Success
  End Function

  Private Shared Sub Pause(ByVal msg As String)
	Dim obj_ref As ObjRef = Nothing
	Dim rc = RhinoGet.GetOneObject(msg, True, ObjectType.AnyObject, obj_ref)
  End Sub

  Private Shared Sub AddLine(ByVal [from] As Point3d, ByVal [to] As Point3d, ByVal color As Color, ByVal doc As RhinoDoc)
	Dim guid = doc.Objects.AddLine([from], [to])
	Dim obj = doc.Objects.Find(guid)
	m_line_objects.Add(obj)
	obj.Attributes.ObjectColor = color
	obj.Attributes.ColorSource = ObjectColorSource.ColorFromObject
	obj.CommitChanges()
  End Sub
End Class
