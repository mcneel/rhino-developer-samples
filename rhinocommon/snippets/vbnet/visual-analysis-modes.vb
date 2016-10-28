Partial Friend Class Examples
  Public Shared Function AnalysisMode_on(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	' make sure our custom visual analysis mode is registered
	Dim zmode = Rhino.Display.VisualAnalysisMode.Register(GetType(ZAnalysisMode))

	Const filter As ObjectType = Rhino.DocObjects.ObjectType.Surface Or Rhino.DocObjects.ObjectType.PolysrfFilter Or Rhino.DocObjects.ObjectType.Mesh
	Dim objs() As Rhino.DocObjects.ObjRef = Nothing
	Dim rc = Rhino.Input.RhinoGet.GetMultipleObjects("Select objects for Z analysis", False, filter, objs)
	If rc IsNot Rhino.Commands.Result.Success Then
	  Return rc
	End If

	Dim count As Integer = 0
	For i As Integer = 0 To objs.Length - 1
	  Dim obj = objs(i).Object()

	  ' see if this object is alreay in Z analysis mode
	  If obj.InVisualAnalysisMode(zmode) Then
		Continue For
	  End If

	  If obj.EnableVisualAnalysisMode(zmode, True) Then
		count += 1
	  End If
	Next i
	doc.Views.Redraw()
	RhinoApp.WriteLine("{0} objects were put into Z-Analysis mode.", count)
	Return Rhino.Commands.Result.Success
  End Function

  Public Shared Function AnalysisMode_off(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	Dim zmode = Rhino.Display.VisualAnalysisMode.Find(GetType(ZAnalysisMode))
	' If zmode is null, we've never registered the mode so we know it hasn't been used
	If zmode IsNot Nothing Then
	  For Each obj As Rhino.DocObjects.RhinoObject In doc.Objects
		obj.EnableVisualAnalysisMode(zmode, False)
	  Next obj
	  doc.Views.Redraw()
	End If
	RhinoApp.WriteLine("Z-Analysis is off.")
	Return Rhino.Commands.Result.Success
  End Function
End Class

''' <summary>
''' This simple example provides a false color based on the world z-coordinate.
''' For details, see the implementation of the FalseColor() function.
''' </summary>
Public Class ZAnalysisMode
	Inherits Rhino.Display.VisualAnalysisMode

  Private m_z_range As New Interval(-10,10)
  Private m_hue_range As New Interval(0,4*Math.PI / 3)
  Private Const m_show_isocurves As Boolean = True

  Public Overrides ReadOnly Property Name() As String
	  Get
		  Return "Z-Analysis"
	  End Get
  End Property
  Public Overrides ReadOnly Property Style() As Rhino.Display.VisualAnalysisMode.AnalysisStyle
	  Get
		  Return AnalysisStyle.FalseColor
	  End Get
  End Property

  Public Overrides Function ObjectSupportsAnalysisMode(ByVal obj As Rhino.DocObjects.RhinoObject) As Boolean
	If TypeOf obj Is Rhino.DocObjects.MeshObject OrElse TypeOf obj Is Rhino.DocObjects.BrepObject Then
	  Return True
	End If
	Return False
  End Function

  Protected Overrides Sub UpdateVertexColors(ByVal obj As Rhino.DocObjects.RhinoObject, ByVal meshes() As Mesh)
	' A "mapping tag" is used to determine if the colors need to be set
	Dim mt As Rhino.Render.MappingTag = GetMappingTag(obj.RuntimeSerialNumber)

	For mi As Integer = 0 To meshes.Length - 1
	  Dim mesh = meshes(mi)
	  If mesh.VertexColors.Tag.Id <> Me.Id Then
		' The mesh's mapping tag is different from ours. Either the mesh has
		' no false colors, has false colors set by another analysis mode, has
		' false colors set using different m_z_range[]/m_hue_range[] values, or
		' the mesh has been moved.  In any case, we need to set the false
		' colors to the ones we want.
		Dim colors(mesh.Vertices.Count - 1) As System.Drawing.Color
		For i As Integer = 0 To mesh.Vertices.Count - 1
		  Dim z As Double = mesh.Vertices(i).Z
		  colors(i) = FalseColor(z)
		Next i
		mesh.VertexColors.SetColors(colors)
		' set the mesh's color tag
		mesh.VertexColors.Tag = mt
	  End If
	Next mi
  End Sub

  Public Overrides ReadOnly Property ShowIsoCurves() As Boolean
	Get
	  ' Most shaded analysis modes that work on breps have the option of
	  ' showing or hiding isocurves.  Run the built-in Rhino ZebraAnalysis
	  ' to see how Rhino handles the user interface.  If controlling
	  ' iso-curve visability is a feature you want to support, then provide
	  ' user interface to set this member variable.
	  Return m_show_isocurves
	End Get
  End Property

  ''' <summary>
  ''' Returns a mapping tag that is used to detect when a mesh's colors need to
  ''' be set.
  ''' </summary>
  ''' <returns></returns>
  Private Function GetMappingTag(ByVal serialNumber As UInteger) As Rhino.Render.MappingTag
	Dim mt As New Rhino.Render.MappingTag()
	mt.Id = Me.Id

	' Since the false colors that are shown will change if the mesh is
	' transformed, we have to initialize the transformation.
	mt.MeshTransform = Transform.Identity

	' This is a 32 bit CRC or the information used to set the false colors.
	' For this example, the m_z_range and m_hue_range intervals control the
	' colors, so we calculate their crc.
	Dim crc As UInteger = RhinoMath.CRC32(serialNumber, m_z_range.T0)
	crc = RhinoMath.CRC32(crc, m_z_range.T1)
	crc = RhinoMath.CRC32(crc, m_hue_range.T0)
	crc = RhinoMath.CRC32(crc, m_hue_range.T1)
	mt.MappingCRC = crc
	Return mt
  End Function

  Private Function FalseColor(ByVal z As Double) As System.Drawing.Color
	' Simple example of one way to change a number into a color.
	Dim s As Double = m_z_range.NormalizedParameterAt(z)
	s = Rhino.RhinoMath.Clamp(s, 0, 1)
	Return System.Drawing.Color.FromArgb(CInt(Fix(s * 255)), 0, 0)
  End Function

End Class
