Partial Friend Class Examples
  Public Shared Function ShowSurfaceDirection(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
	Dim objref As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select surface or polysurface for direction display", False, Rhino.DocObjects.ObjectType.Surface Or Rhino.DocObjects.ObjectType.PolysrfFilter, objref)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim brep = objref.Brep()
	If brep Is Nothing Then
	  Return Rhino.Commands.Result.Failure
	End If

	Dim bIsSolid As Boolean = brep.IsSolid

	Dim conduit As New TestSurfaceDirConduit(brep)
	conduit.Enabled = True
	doc.Views.Redraw()

	Dim gf = New Rhino.Input.Custom.GetOption()
	gf.SetCommandPrompt("Press enter when done")
	gf.AcceptNothing(True)
	If Not bIsSolid Then
	  gf.AddOption("Flip")
	End If

	Do
	  Dim res = gf.Get()
	  If res Is Rhino.Input.GetResult.Option Then
		conduit.Flip = Not conduit.Flip
		doc.Views.Redraw()
		Continue Do
	  End If
	  If res Is Rhino.Input.GetResult.Nothing Then
		If Not bIsSolid AndAlso conduit.Flip Then
		  brep.Flip()
		  doc.Objects.Replace(objref, brep)
		End If
	  End If
	  Exit Do
	Loop

	conduit.Enabled = False
	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class

Friend Class TestSurfaceDirConduit
	Inherits Rhino.Display.DisplayConduit

  Private ReadOnly m_brep As Brep
  Private ReadOnly m_points As List(Of Point3d)
  Private ReadOnly m_normals As List(Of Vector3d)

  Public Sub New(ByVal brep As Brep)
	m_brep = brep
	Flip = False

	Const SURFACE_ARROW_COUNT As Integer = 5
	Dim face_count As Integer = m_brep.Faces.Count
	Dim capacity As Integer = face_count * SURFACE_ARROW_COUNT * SURFACE_ARROW_COUNT
	m_points = New List(Of Point3d)(capacity)
	m_normals = New List(Of Vector3d)(capacity)

	For i As Integer = 0 To face_count - 1
	  Dim face = brep.Faces(i)
	  Dim [loop] = face.OuterLoop
	  If [loop] Is Nothing Then
		Continue For
	  End If

	  Dim udomain = face.Domain(0)
	  Dim vdomain = face.Domain(1)
	  Dim loop_bbox = [loop].GetBoundingBox(True)
	  If loop_bbox.IsValid Then
		Dim domain As New Interval(loop_bbox.Min.X, loop_bbox.Max.X)
		domain = Interval.FromIntersection(domain, udomain)
		If domain.IsIncreasing Then
		  udomain = domain
		End If
		domain = New Interval(loop_bbox.Min.Y, loop_bbox.Max.Y)
		domain = Interval.FromIntersection(domain, vdomain)
		If domain.IsIncreasing Then
		  vdomain = domain
		End If
	  End If

	  Dim bUntrimmed As Boolean = face.IsSurface
	  Dim bRev As Boolean = face.OrientationIsReversed
	  For u As Double = 0 To SURFACE_ARROW_COUNT - 1 Step 1.0
		Dim d As Double = u / (SURFACE_ARROW_COUNT - 1.0)
		Dim s As Double = udomain.ParameterAt(d)

		Dim intervals = face.TrimAwareIsoIntervals(1, s)
		If bUntrimmed OrElse intervals.Length > 0 Then
		  For v As Double = 0 To SURFACE_ARROW_COUNT - 1 Step 1.0
			d = v / (SURFACE_ARROW_COUNT - 1.0)
			Dim t As Double = vdomain.ParameterAt(d)
			Dim bAdd As Boolean = bUntrimmed
			Dim k As Integer = 0
			Do While Not bAdd AndAlso k < intervals.Length
			  If intervals(k).IncludesParameter(t) Then
				bAdd = True
			  End If
				k += 1
			Loop
			If bAdd Then
			  Dim pt = face.PointAt(s, t)
			  Dim vec = face.NormalAt(s, t)
			  m_points.Add(pt)
			  If bRev Then
				vec.Reverse()
			  End If
			  m_normals.Add(vec)
			End If
		  Next v
		End If
	  Next u
	Next i
  End Sub

  Public Property Flip() As Boolean

  Protected Overrides Sub DrawOverlay(ByVal e As Rhino.Display.DrawEventArgs)
	If m_points.Count > 0 Then
	  Dim color = Rhino.ApplicationSettings.AppearanceSettings.TrackingColor
	  For i As Integer = 0 To m_points.Count - 1
		If i Mod 100 = 0 AndAlso e.Display.InterruptDrawing() Then
		  Exit For
		End If

		Dim pt = m_points(i)
		Dim dir = m_normals(i)
		If Flip Then
		  dir.Reverse()
		End If
		e.Display.DrawDirectionArrow(pt, dir, color)
	  Next i
	End If
  End Sub
End Class
