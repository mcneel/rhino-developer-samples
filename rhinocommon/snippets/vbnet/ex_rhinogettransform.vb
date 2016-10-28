Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Geometry
Imports Rhino.Commands
Imports Rhino.Input.Custom
Imports Rhino.Display

Namespace examples_vb
  Public Class GetTranslation
    Inherits GetTransform
    Public Overrides Function CalculateTransform(viewport As RhinoViewport, point As Point3d) As Transform
      Dim xform = Transform.Identity
      Dim base_point As Point3d
      If TryGetBasePoint(base_point) Then
        Dim v = point - base_point
        If Not v.IsZero Then
          xform = Transform.Translation(v)
          If Not xform.IsValid Then
            xform = Transform.Identity
          End If
        End If
      End If
      Return xform
    End Function
  End Class

  Public Class RhinoGetTransformCommand
    Inherits TransformCommand
    Public Sub New()
      ' simple example of handling the BeforeTransformObjects event
      AddHandler RhinoDoc.BeforeTransformObjects, AddressOf RhinoDocOnBeforeTransformObjects
    End Sub

    Private Sub RhinoDocOnBeforeTransformObjects(sender As Object, ea As RhinoTransformObjectsEventArgs)
      RhinoApp.WriteLine("Transform Objects Count: {0}", ea.ObjectCount)
    End Sub

    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbGetTranslation"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim list = New Rhino.Collections.TransformObjectList()
      Dim rc = SelectObjects("Select objects to move", list)
      If rc <> Rhino.Commands.Result.Success Then
        Return rc
      End If

      Dim gp = New GetPoint()
      gp.SetCommandPrompt("Point to move from")
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If


      Dim gt = New GetTranslation()
      gt.SetCommandPrompt("Point to move to")
      gt.SetBasePoint(gp.Point(), True)
      gt.DrawLineFromPoint(gp.Point(), True)
      gt.AddTransformObjects(list)
      gt.GetXform()
      If gt.CommandResult() <> Result.Success Then
        Return gt.CommandResult()
      End If

      Dim xform = gt.CalculateTransform(gt.View().ActiveViewport, gt.Point())
      TransformObjects(list, xform, False, False)
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace
