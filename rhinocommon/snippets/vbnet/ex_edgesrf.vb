Imports System.Linq
Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects
Imports Rhino.Geometry
Imports Rhino.Input.Custom

Namespace examples_vb
  Public Class EdgeSrfCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbEdgeSrf"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim go = New GetObject()
      go.SetCommandPrompt("Select 2, 3, or 4 open curves")
      go.GeometryFilter = ObjectType.Curve
      go.GeometryAttributeFilter = GeometryAttributeFilter.OpenCurve
      go.GetMultiple(2, 4)
      If go.CommandResult() <> Result.Success Then
        Return go.CommandResult()
      End If

      Dim curves = go.Objects().[Select](Function(o) o.Curve())

      Dim brep__1 = Brep.CreateEdgeSurface(curves)

      If brep__1 IsNot Nothing Then
        doc.Objects.AddBrep(brep__1)
        doc.Views.Redraw()
      End If

      Return Result.Success
    End Function
  End Class
End Namespace