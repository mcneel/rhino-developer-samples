Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.Input
Imports Rhino.Commands

Namespace examples_vb
  Public Class PrincipalCurvatureCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbPrincipalCurvature"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim obj_ref As ObjRef = Nothing
      Dim rc = RhinoGet.GetOneObject("Select surface for curvature measurement", True,
                                     ObjectType.Surface, obj_ref)
      If rc <> Result.Success Then
        Return rc
      End If
      Dim surface = obj_ref.Surface()

      Dim gp = New Rhino.Input.Custom.GetPoint()
      gp.SetCommandPrompt("Select point on surface for curvature measurement")
      gp.Constrain(surface, False)
      gp.[Get]()
      If gp.CommandResult() <> Result.Success Then
        Return gp.CommandResult()
      End If
      Dim point_on_surface = gp.Point()

      Dim u As Double, v As Double
      If Not surface.ClosestPoint(point_on_surface, u, v) Then
        Return Result.Failure
      End If

      Dim surface_curvature = surface.CurvatureAt(u, v)
      If surface_curvature Is Nothing Then
        Return Result.Failure
      End If

      RhinoApp.WriteLine("Surface curvature evaluation at parameter: ({0}, {1})", u, v)

      RhinoApp.WriteLine("  3-D Point: {0}", surface_curvature.Point)
      RhinoApp.WriteLine("  3-D Normal: {0}", surface_curvature.Normal)
      RhinoApp.WriteLine("  Maximum principal curvature: {0} ({1})", surface_curvature.Kappa(0), surface_curvature.Direction(0))
      RhinoApp.WriteLine("  Minimum principal curvature: {0} ({1})", surface_curvature.Kappa(1), surface_curvature.Direction(1))
      RhinoApp.WriteLine("  Gaussian curvature: {0}", surface_curvature.Gaussian)
      RhinoApp.WriteLine("  Mean curvature: {0}", surface_curvature.Mean)
      Return Result.Success
    End Function
  End Class
End Namespace