Partial Friend Class Examples
  Private Shared m_draw_single_sprite As Boolean
  Private Shared m_sprite_size As Single = 30
  Private Shared m_draw_world_location As Boolean = True

  Public Shared Function SpriteDrawing(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	Dim sprite_mode = New Rhino.Input.Custom.OptionToggle(m_draw_single_sprite, "SpriteList", "SingleSprite")
	Dim size_option = New Rhino.Input.Custom.OptionDouble(m_sprite_size)
	Dim space_option = New Rhino.Input.Custom.OptionToggle(m_draw_world_location, "Screen", "World")
	Dim go = New Rhino.Input.Custom.GetOption()
	go.SetCommandPrompt("Sprite drawing mode")
	go.AddOptionToggle("Mode", sprite_mode)
	go.AddOptionDouble("Size", size_option)
	go.AddOptionToggle("DrawSpace", space_option)
	Dim option_go As Integer = go.AddOption("Spin")
	Dim option_file As Integer = go.AddOption("FileSprite")

	AddHandler Rhino.Display.DisplayPipeline.PostDrawObjects, AddressOf DisplayPipeline_PostDrawObjects
	AddHandler Rhino.Display.DisplayPipeline.CalculateBoundingBox, AddressOf DisplayPipeline_CalculateBoundingBox

	doc.Views.Redraw()
	Do While go.Get() = Rhino.Input.GetResult.Option
	  m_draw_single_sprite = sprite_mode.CurrentValue
	  m_sprite_size = CSng(size_option.CurrentValue)
	  m_draw_world_location = space_option.CurrentValue
	  If go.OptionIndex() = option_go Then
		Dim gs = New Rhino.Input.Custom.GetOption()
		gs.SetCommandPrompt("press enter/escape to end")
		gs.SetWaitDuration(1)

		Dim vp = doc.Views.ActiveView.MainViewport
		Do While gs.Get() = Rhino.Input.GetResult.Timeout
		  vp.Rotate(0.1, Vector3d.ZAxis, Point3d.Origin)
		  doc.Views.Redraw()
		Loop
	  ElseIf go.OptionIndex() = option_file Then
		Dim dlg = New Rhino.UI.OpenFileDialog()
		If dlg.ShowDialog() Then
		  Dim bmp As New System.Drawing.Bitmap(dlg.FileName)
		  m_sprite = New Rhino.Display.DisplayBitmap(bmp)
		End If
		doc.Views.Redraw()
	  Else
		doc.Views.Redraw()
	  End If
	Loop

	RemoveHandler Rhino.Display.DisplayPipeline.PostDrawObjects, AddressOf DisplayPipeline_PostDrawObjects
	RemoveHandler Rhino.Display.DisplayPipeline.CalculateBoundingBox, AddressOf DisplayPipeline_CalculateBoundingBox
	Return Rhino.Commands.Result.Success
  End Function

  Private Shared Sub DisplayPipeline_CalculateBoundingBox(ByVal sender As Object, ByVal e As Rhino.Display.CalculateBoundingBoxEventArgs)
	If m_draw_single_sprite Then
	  Dim bbox As New BoundingBox(New Point3d(20, 0, 0), New Point3d(20, 0, 0))
	  e.IncludeBoundingBox(bbox)
	Else
	  Dim items = GetItems()
	  e.IncludeBoundingBox(items.BoundingBox)
	End If
  End Sub

  Private Shared Sub DisplayPipeline_PostDrawObjects(ByVal sender As Object, ByVal e As Rhino.Display.DrawEventArgs)
	Dim spr = GetSprite()
	If m_draw_single_sprite Then
	  If m_draw_world_location Then
		e.Display.DrawSprite(spr, New Point3d(20, 0, 0), m_sprite_size, System.Drawing.Color.White, False)
	  Else
		e.Display.DrawSprite(spr, New Point2d(150, 150), m_sprite_size, System.Drawing.Color.White)
	  End If
	Else
	  Dim items = GetItems()
	  e.Display.DrawSprites(spr, items, m_sprite_size, False)
	End If
  End Sub

  Private Shared m_items As Rhino.Display.DisplayBitmapDrawList
  Private Shared Function GetItems() As Rhino.Display.DisplayBitmapDrawList
	If m_items Is Nothing Then
	  m_items = New Rhino.Display.DisplayBitmapDrawList()
	  Dim points = New System.Collections.Generic.List(Of Point3d)()
	  Dim colors = New System.Collections.Generic.List(Of System.Drawing.Color)()
	  Dim random = New System.Random()
	  For i As Integer = 0 To 199
		Dim x As Double = random.NextDouble()
		Dim y As Double = random.NextDouble()
		Dim z As Double = random.NextDouble()
		points.Add(New Point3d(-30 + x * 60, -30 + y * 60, -30 + z * 60))
		Dim r As Integer = CInt(Fix(x * 255))
		Dim g As Integer = CInt(Fix(y * 255))
		Dim b As Integer = CInt(Fix(z * 255))
		colors.Add(System.Drawing.Color.FromArgb(r, g, b))
	  Next i
	  m_items.SetPoints(points, colors)
	End If
	Return m_items
  End Function

  Private Shared m_sprite As Rhino.Display.DisplayBitmap
  Private Shared Function GetSprite() As Rhino.Display.DisplayBitmap
	If m_sprite Is Nothing Then
	  Dim bmp = New System.Drawing.Bitmap(64, 64)
	  Using g As System.Drawing.Graphics = System.Drawing.Graphics.FromImage(bmp)
		g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias
		g.Clear(System.Drawing.Color.Transparent)
		Dim pen = New System.Drawing.Pen(System.Drawing.Color.White, 6)
		pen.EndCap = System.Drawing.Drawing2D.LineCap.Round
		pen.StartCap = pen.EndCap
		g.DrawArc(pen, New System.Drawing.Rectangle(16, 16, 32, 32), 0, 360)
		g.DrawLine(pen, New System.Drawing.Point(8, 8), New System.Drawing.Point(56, 56))
		g.DrawLine(pen, New System.Drawing.Point(8, 56), New System.Drawing.Point(56, 8))
		pen.Dispose()
	  End Using
	  m_sprite = New Rhino.Display.DisplayBitmap(bmp)
	End If
	Return m_sprite
  End Function
End Class
