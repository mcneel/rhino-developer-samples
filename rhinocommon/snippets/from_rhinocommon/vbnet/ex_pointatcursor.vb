Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class PointAtCursorCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbPointAtCursor"
      End Get
    End Property

    <System.Runtime.InteropServices.DllImport("user32.dll")> _
    Public Shared Function GetCursorPos(ByRef point As System.Drawing.Point) As Boolean
    End Function

    <System.Runtime.InteropServices.DllImport("user32.dll")> _
    Public Shared Function ScreenToClient(hWnd As IntPtr, ByRef point As System.Drawing.Point) As Boolean
    End Function

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim result__1 = Result.Failure
      Dim view = doc.Views.ActiveView
      If view Is Nothing Then
        Return result__1
      End If

      Dim windowsDrawingPoint As System.Drawing.Point
      If Not GetCursorPos(windowsDrawingPoint) OrElse Not ScreenToClient(view.Handle, windowsDrawingPoint) Then
        Return result__1
      End If

      Dim xform = view.ActiveViewport.GetTransform(CoordinateSystem.Screen, CoordinateSystem.World)
      Dim point = New Rhino.Geometry.Point3d(windowsDrawingPoint.X, windowsDrawingPoint.Y, 0.0)
      RhinoApp.WriteLine([String].Format("screen point: ({0}, {1}, {2})", point.X, point.Y, point.Z))
      point.Transform(xform)
      RhinoApp.WriteLine([String].Format("world point: ({0}, {1}, {2})", point.X, point.Y, point.Z))
      result__1 = Result.Success
      Return result__1
    End Function
  End Class
End Namespace