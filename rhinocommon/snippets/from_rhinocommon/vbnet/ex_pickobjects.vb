
Imports System.Runtime.InteropServices
Imports Rhino
Imports Rhino.Collections
Imports Rhino.Commands
Imports System.Collections.Generic
Imports Rhino.Display
Imports Rhino.Geometry
Imports Rhino.Input.Custom

Namespace examples_vb
  Public Class PickPointsOnConduitCommand
    Inherits Rhino.Commands.Command
    Private conduitPoints As New List(Of ConduitPoint)()

    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbPickPoints"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As Rhino.Commands.RunMode) As Rhino.Commands.Result
      Dim conduit = New PointsConduit(conduitPoints)
      conduit.Enabled = True

      Dim gp = New Rhino.Input.Custom.GetPoint()
      While True
        gp.SetCommandPrompt("click location to create point. (<ESC> exit)")
        gp.AcceptNothing(True)
        gp.[Get]()
        If gp.CommandResult() <> Rhino.Commands.Result.Success Then
          Exit While
        End If
        conduitPoints.Add(New ConduitPoint(gp.Point()))
        doc.Views.Redraw()
      End While

      Dim gcp = New GetConduitPoint(conduitPoints)
      While True
        gcp.SetCommandPrompt("select conduit point. (<ESC> to exit)")
        gcp.AcceptNothing(True)
        gcp.[Get](True)
        doc.Views.Redraw()
        If gcp.CommandResult() <> Rhino.Commands.Result.Success Then
          Exit While
        End If
      End While

      Return Rhino.Commands.Result.Success
    End Function
  End Class

  Public Class ConduitPoint
    Public Sub New(point__1 As Point3d)
      Color = System.Drawing.Color.White
      Point = point__1
    End Sub
    Public Property Color() As System.Drawing.Color
      Get
        Return m_Color
      End Get
      Set(value As System.Drawing.Color)
        m_Color = value
      End Set
    End Property
    Private m_Color As System.Drawing.Color
    Public Property Point() As Point3d
      Get
        Return m_Point
      End Get
      Set(value As Point3d)
        m_Point = value
      End Set
    End Property
    Private m_Point As Point3d
  End Class

  Public Class GetConduitPoint
    Inherits GetPoint
    Private _conduitPoints As List(Of ConduitPoint)

    Public Sub New(conduitPoints As List(Of ConduitPoint))
      _conduitPoints = conduitPoints
    End Sub

    Protected Overrides Sub OnMouseDown(e As GetPointMouseEventArgs)
      MyBase.OnMouseDown(e)
      Dim picker = New PickContext()
      picker.View = e.Viewport.ParentView

      picker.PickStyle = PickStyle.PointPick

      Dim xform = e.Viewport.GetPickTransform(e.WindowPoint)
      picker.SetPickTransform(xform)

      For Each cp As ConduitPoint In _conduitPoints
        Dim depth As Double
        Dim distance As Double
        If picker.PickFrustumTest(cp.Point, depth, distance) Then
          cp.Color = System.Drawing.Color.Red
        Else
          cp.Color = System.Drawing.Color.White
        End If
      Next
    End Sub
  End Class

  Class PointsConduit
    Inherits Rhino.Display.DisplayConduit
    Private _conduitPoints As List(Of ConduitPoint)

    Public Sub New(conduitPoints As List(Of ConduitPoint))
      _conduitPoints = conduitPoints
    End Sub

    Protected Overrides Sub DrawForeground(e As Rhino.Display.DrawEventArgs)
      If _conduitPoints IsNot Nothing Then
        For Each cp As ConduitPoint In _conduitPoints
          e.Display.DrawPoint(cp.Point, PointStyle.Simple, 3, cp.Color)
        Next
      End If
    End Sub
  End Class
End Namespace