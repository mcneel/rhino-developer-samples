Imports Rhino
Imports Rhino.Geometry
Imports Rhino.DocObjects
Imports Rhino.Commands
Imports Rhino.Input.Custom

Namespace examples_vb
  Public Class ExtendSurfaceCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbExtendSurface"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim go = New Rhino.Input.Custom.GetObject()
      go.SetCommandPrompt("Select edge of surface to extend")
      go.GeometryFilter = ObjectType.EdgeFilter
      go.GeometryAttributeFilter = GeometryAttributeFilter.EdgeCurve
      go.[Get]()
      If go.CommandResult() <> Result.Success Then
        Return go.CommandResult()
      End If
      Dim obj_ref = go.[Object](0)

      Dim surface = obj_ref.Surface()
      If surface Is Nothing Then
        RhinoApp.WriteLine("Unable to extend polysurfaces.")
        Return Result.Failure
      End If

      Dim brep__1 = obj_ref.Brep()
      Dim face = obj_ref.Face()
      If brep__1 Is Nothing OrElse face Is Nothing Then
        Return Result.Failure
      End If
      If face.FaceIndex < 0 Then
        Return Result.Failure
      End If

      If Not brep__1.IsSurface Then
        RhinoApp.WriteLine("Unable to extend trimmed surfaces.")
        Return Result.[Nothing]
      End If

      Dim curve = obj_ref.Curve()

      Dim trim = obj_ref.Trim()
      If trim Is Nothing Then
        Return Result.Failure
      End If

      If trim.TrimType = BrepTrimType.Seam Then
        RhinoApp.WriteLine("Unable to extend surface at seam.")
        Return Result.[Nothing]
      End If

      Dim extended_surface = surface.Extend(trim.IsoStatus, 5.0, True)
      If extended_surface IsNot Nothing Then
        Dim mybrep = Brep.CreateFromSurface(extended_surface)
        doc.Objects.Replace(obj_ref.ObjectId, mybrep)
        doc.Views.Redraw()
      End If
      Return Result.Success
    End Function
  End Class
End Namespace