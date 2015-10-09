Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input
Imports Rhino.Input.Custom
Imports Rhino.Geometry

Namespace examples_vb
  Public Class ExtractIsocurveCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbExtractIsocurve"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select surface", False, ObjectType.Surface, obj_ref)
      If rc <> Result.Success OrElse obj_ref Is Nothing Then
        Return rc
      End If
      Dim surface = obj_ref.Surface()

      Dim gp = New GetPoint()
      gp.SetCommandPrompt("Point on surface")
      gp.Constrain(surface, False)
      'gp.GeometryFilter = ObjectType.Point;
      Dim option_toggle = New OptionToggle(False, "U", "V")
      gp.AddOptionToggle("Direction", option_toggle)
      Dim point As Point3d = Point3d.Unset
      While True
        Dim grc = gp.[Get]()
        If grc = GetResult.[Option] Then
          Continue While
        ElseIf grc = GetResult.Point Then
          point = gp.Point()
          Exit While
        Else
          Return Result.[Nothing]
        End If
      End While
      If point = Point3d.Unset Then
        Return Result.[Nothing]
      End If

      Dim direction As Integer = If(option_toggle.CurrentValue, 1, 0)
      ' V : U
      Dim u_parameter As Double, v_parameter As Double
      If Not surface.ClosestPoint(point, u_parameter, v_parameter) Then
        Return Result.Failure
      End If

      Dim iso_curve = surface.IsoCurve(direction, If(direction = 1, u_parameter, v_parameter))
      If iso_curve Is Nothing Then
        Return Result.Failure
      End If

      doc.Objects.AddCurve(iso_curve)
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace