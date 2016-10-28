Imports System.Drawing
Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Geometry
Imports Rhino.Input
Imports Rhino.Commands

Namespace examples_vb
  Public Class ModifyObjectColorCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbModifyObjectColor"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select object", False, ObjectType.AnyObject, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim rhino_object = obj_ref.[Object]()
      Dim color__1 = rhino_object.Attributes.ObjectColor
      Dim b As Boolean = Rhino.UI.Dialogs.ShowColorDialog(color__1)
      If Not b Then
        Return Result.Cancel
      End If

      rhino_object.Attributes.ObjectColor = color__1
      rhino_object.Attributes.ColorSource = ObjectColorSource.ColorFromObject
      rhino_object.CommitChanges()

      ' an object's color attributes can also be specified
      ' when the object is added to Rhino
      Dim sphere = New Sphere(Point3d.Origin, 5.0)
      Dim attributes = New ObjectAttributes()
      attributes.ObjectColor = Color.CadetBlue
      attributes.ColorSource = ObjectColorSource.ColorFromObject
      doc.Objects.AddSphere(sphere, attributes)

      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace