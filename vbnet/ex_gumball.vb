Imports Rhino
Imports Rhino.Geometry


Public Class GumballExampleCommand
  Inherits Rhino.Commands.TransformCommand
  Public Overrides ReadOnly Property EnglishName() As String
    Get
      Return "vbExampleGumball"
    End Get
  End Property

  Protected Overrides Function RunCommand(doc As RhinoDoc, mode As Rhino.Commands.RunMode) As Rhino.Commands.Result
    ' Select objects to scale
    Dim list = New Rhino.Collections.TransformObjectList()
    Dim rc = SelectObjects("Select objects to gumball", list)
    If rc <> Rhino.Commands.Result.Success Then Return rc

    Dim bbox = list.GetBoundingBox(True, True)
    If Not bbox.IsValid Then Return Rhino.Commands.Result.Failure

    Dim cmdrc As Rhino.Commands.Result

    Dim base_gumball = New Rhino.UI.Gumball.GumballObject()
    base_gumball.SetFromBoundingBox(bbox)
    Dim dc = New Rhino.UI.Gumball.GumballDisplayConduit()
    Dim appearance = New Rhino.UI.Gumball.GumballAppearanceSettings()

    ' turn off some of the scale appearance settings to have a slightly different gumball
    appearance.ScaleXEnabled = False
    appearance.ScaleYEnabled = False
    appearance.ScaleZEnabled = False

    Dim bCopy As Boolean = False
    While True
      dc.SetBaseGumball(base_gumball, appearance)
      dc.Enabled = True
      doc.Views.Redraw()

      Dim gp As New GetGumballXform(dc)
      Dim copy_optindx As Integer = gp.AddOption("Copy")
      If dc.PreTransform = Transform.Identity Then
        gp.SetCommandPrompt("Drag gumball")
      Else
        gp.AcceptNothing(True)
        gp.SetCommandPrompt("Drag gumball. Press Enter when done")
      End If
      gp.AddTransformObjects(list)
      gp.MoveGumball()
      dc.Enabled = False
      cmdrc = gp.CommandResult()
      If cmdrc <> Rhino.Commands.Result.Success Then Exit While

      Dim getpoint_result = gp.Result()
      If getpoint_result = Rhino.Input.GetResult.Point Then
        If Not dc.InRelocate Then
          Dim xform As Transform = dc.TotalTransform
          dc.PreTransform = xform
        End If
        ' update location of base gumball
        Dim gbframe = dc.Gumball.Frame
        Dim baseFrame = base_gumball.Frame
        baseFrame.Plane = gbframe.Plane
        baseFrame.ScaleGripDistance = gbframe.ScaleGripDistance
        base_gumball.Frame = baseFrame
        Continue While
      End If
      If getpoint_result = Rhino.Input.GetResult.Option Then
        If gp.OptionIndex() = copy_optindx Then
          bCopy = True
        End If
        Continue While
      End If

      Exit While
    End While

    dc.Enabled = False
    If dc.PreTransform <> Transform.Identity Then
      Dim xform As Transform = dc.PreTransform
      TransformObjects(list, xform, bCopy, bCopy)
    End If
    doc.Views.Redraw()
    Return cmdrc
  End Function
End Class


Class GetGumballXform
  Inherits Rhino.Input.Custom.GetTransform
  Private ReadOnly m_dc As Rhino.UI.Gumball.GumballDisplayConduit
  Public Sub New(dc As Rhino.UI.Gumball.GumballDisplayConduit)
    m_dc = dc
  End Sub

  Public Overrides Function CalculateTransform(viewport As Rhino.Display.RhinoViewport, point As Point3d) As Transform
    If m_dc.InRelocate Then
      ' don't move objects while relocating gumball
      Return m_dc.PreTransform
    End If

    Return m_dc.TotalTransform
  End Function

  Protected Overrides Sub OnMouseDown(e As Rhino.Input.Custom.GetPointMouseEventArgs)
    If m_dc.PickResult.Mode <> Rhino.UI.Gumball.GumballMode.None Then
      Return
    End If
    m_dc.PickResult.SetToDefault()

    Dim pick_context As New Rhino.Input.Custom.PickContext()
    pick_context.View = e.Viewport.ParentView
    pick_context.PickStyle = Rhino.Input.Custom.PickStyle.PointPick
    Dim xform = e.Viewport.GetPickTransform(e.WindowPoint)
    pick_context.SetPickTransform(xform)
    Dim pick_line As Rhino.Geometry.Line
    e.Viewport.GetFrustumLine(e.WindowPoint.X, e.WindowPoint.Y, pick_line)
    pick_context.PickLine = pick_line
    pick_context.UpdateClippingPlanes()
    ' pick gumball and, if hit, set getpoint dragging constraints.
    m_dc.PickGumball(pick_context, Me)
  End Sub

  Protected Overrides Sub OnMouseMove(e As Rhino.Input.Custom.GetPointMouseEventArgs)
    If m_dc.PickResult.Mode = Rhino.UI.Gumball.GumballMode.None Then Return

    m_dc.CheckShiftAndControlKeys()
    Dim world_line As Rhino.Geometry.Line
    If Not e.Viewport.GetFrustumLine(e.WindowPoint.X, e.WindowPoint.Y, world_line) Then
      world_line = Rhino.Geometry.Line.Unset
    End If

    Dim rc As Boolean = m_dc.UpdateGumball(e.Point, world_line)
    If rc Then MyBase.OnMouseMove(e)
  End Sub

  Protected Overrides Sub OnDynamicDraw(e As Rhino.Input.Custom.GetPointDrawEventArgs)
    ' Disable default GetTransform drawing by not calling the base class
    ' implementation. All aspects of gumball display are handled by 
    ' GumballDisplayConduit
    'base.OnDynamicDraw(e);
  End Sub

  ' lets user drag m_gumball around.
  Public Function MoveGumball() As Rhino.Input.GetResult
    ' Get point on a MouseUp event
    If m_dc.PreTransform <> Transform.Identity Then
      HaveTransform = True
      Transform = m_dc.PreTransform
    End If
    SetBasePoint(m_dc.BaseGumball.Frame.Plane.Origin, False)

    ' V5 uses a display conduit to provide display feedback
    ' so shaded objects move
    ObjectList.DisplayFeedbackEnabled = True
    If Transform <> Transform.Identity Then
      ObjectList.UpdateDisplayFeedbackTransform(Transform)
    End If

    ' Call Get with mouseUp set to true
    Dim rc = Me.Get(True)

    ' V5 uses a display conduit to provide display feedback
    ' so shaded objects move
    ObjectList.DisplayFeedbackEnabled = False
    Return rc
  End Function
End Class
