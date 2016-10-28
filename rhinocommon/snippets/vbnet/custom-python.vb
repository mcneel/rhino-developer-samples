Partial Friend Class Examples
  Public Shared Function CustomPython(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	If Nothing Is m_python Then
	  m_python = Rhino.Runtime.PythonScript.Create()
	  If Nothing Is m_python Then
		RhinoApp.WriteLine("Error: Unable to create an instance of the python engine")
		Return Rhino.Commands.Result.Failure
	  End If
	End If
	m_python.ScriptContextDoc = New CustomPythonDoc(doc)

	Const script As String = "
import rhinoscriptsyntax as rs
rs.AddLine((0,0,0), (10,10,10))
"
	m_python.ExecuteScript(script)
	Return Rhino.Commands.Result.Success
  End Function

  Private Shared m_python As Rhino.Runtime.PythonScript
End Class

' our fake RhinoDoc
Public Class CustomPythonDoc
  Private ReadOnly m_doc As RhinoDoc
  Public Sub New(ByVal doc As RhinoDoc)
	m_doc = doc
  End Sub

  Private ReadOnly m_table As New CustomObjectTable()
  Public ReadOnly Property Objects() As CustomObjectTable
	Get
		Return m_table
	End Get
  End Property

  Public ReadOnly Property Views() As Rhino.DocObjects.Tables.ViewTable
	Get
	  Return m_doc.Views
	End Get
  End Property

End Class

Public Class CustomObjectTable
  Public Function AddLine(ByVal p1 As Rhino.Geometry.Point3d, ByVal p2 As Rhino.Geometry.Point3d) As Guid
	Dim l As New Rhino.Geometry.Line(p1, p2)
	If l.IsValid Then
	  Dim id As Guid = Guid.NewGuid()
	  m_lines_dict.Add(id, l)
	  Return id
	End If
	Return Guid.Empty
  End Function

  Private ReadOnly m_lines_dict As New System.Collections.Generic.Dictionary(Of Guid, Rhino.Geometry.Line)()
End Class
