Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input
Imports Rhino.Input.Custom

Namespace examples_vb
  Public Class PickPointCommand
    Inherits Rhino.Commands.Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbPickPoint"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      ' this creates a point where the mouse is clicked.
      Dim gp = New GetPoint()
      gp.SetCommandPrompt("Click for new point")
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If
      Dim point3d = gp.Point()
      doc.Objects.AddPoint(point3d)
      doc.Views.Redraw()

      ' selects a point that already exists
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select point", False, ObjectType.Point, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim point = obj_ref.Point()
      RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}", point.Location.X, point.Location.Y, point.Location.Z)
      doc.Objects.UnselectAll()

      ' selects multiple points that already exist
      Dim obj_refs As ObjRef() = Nothing
      rc = RhinoGet.GetMultipleObjects("Select point", False, ObjectType.Point, obj_refs)
      If rc <> Result.Success Then
        Return rc
      End If
      For Each o_ref As ObjRef In obj_refs
        point = o_ref.Point()
        RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}", point.Location.X, point.Location.Y, point.Location.Z)
      Next
      doc.Objects.UnselectAll()

      ' also selects a point that already exists.
      ' Used when RhinoGet doesn't provide enough control
      Dim go = New GetObject()
      go.SetCommandPrompt("Select point")
      go.GeometryFilter = ObjectType.Point
      go.GetMultiple(1, 0)
      If go.CommandResult() <> Result.Success Then
        Return go.CommandResult()
      End If
      For Each o_ref As ObjRef In go.Objects()
        point = o_ref.Point()
        If point IsNot Nothing Then
          RhinoApp.WriteLine("Point: x:{0}, y:{1}, z:{2}", point.Location.X, point.Location.Y, point.Location.Z)
        End If
      Next

      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace