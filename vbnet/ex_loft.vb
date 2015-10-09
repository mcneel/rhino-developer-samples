Imports Rhino
Imports Rhino.Input.Custom
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports System.Collections.Generic
Imports Rhino.Geometry

Namespace examples_vb
  Public Class LoftCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbLoft"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As Rhino.Commands.RunMode) As Result
      ' select curves to loft
      Dim gs = New GetObject()
      gs.SetCommandPrompt("select curves to loft")
      gs.GeometryFilter = ObjectType.Curve
      gs.DisablePreSelect()
      gs.SubObjectSelect = False
      gs.GetMultiple(2, 0)
      If gs.CommandResult() <> Result.Success Then
        Return gs.CommandResult()
      End If

      Dim curves = New List(Of Curve)()
      For Each obj As ObjRef In gs.Objects()
        curves.Add(obj.Curve())
      Next

      Dim breps = Rhino.Geometry.Brep.CreateFromLoft(curves, Point3d.Unset, Point3d.Unset, LoftType.Tight, False)
      For Each brep As Brep In breps
        doc.Objects.AddBrep(brep)
      Next

      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace