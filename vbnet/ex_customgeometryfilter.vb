Imports Rhino
Imports Rhino.Geometry
Imports Rhino.Commands
Imports Rhino.Input.Custom
Imports Rhino.DocObjects

Namespace examples_vb
  Public Class CustomGeometryFilterCommand
    Inherits Command
    Private _tolerance As Double
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbCustomGeometryFilter"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      _tolerance = doc.ModelAbsoluteTolerance

      ' only use a custom geometry filter if no simpler filter does the job

      ' only curves
      Dim gc = New GetObject()
      gc.SetCommandPrompt("select curve")
      gc.GeometryFilter = ObjectType.Curve
      gc.DisablePreSelect()
      gc.SubObjectSelect = False
      gc.[Get]()
      If gc.CommandResult() <> Result.Success Then
        Return gc.CommandResult()
      End If
      If gc.[Object](0).Curve() Is Nothing Then
        Return Result.Failure
      End If
      Rhino.RhinoApp.WriteLine("curve was selected")

      ' only closed curves
      Dim gcc = New GetObject()
      gcc.SetCommandPrompt("select closed curve")
      gcc.GeometryFilter = ObjectType.Curve
      gcc.GeometryAttributeFilter = GeometryAttributeFilter.ClosedCurve
      gcc.DisablePreSelect()
      gcc.SubObjectSelect = False
      gcc.[Get]()
      If gcc.CommandResult() <> Result.Success Then
        Return gcc.CommandResult()
      End If
      If gcc.[Object](0).Curve() Is Nothing Then
        Return Result.Failure
      End If
      Rhino.RhinoApp.WriteLine("closed curve was selected")

      ' only circles with a radius of 10
      Dim gcc10 = New GetObject()
      gcc10.SetCommandPrompt("select circle with radius of 10")
      gc.GeometryFilter = ObjectType.Curve
      gcc10.SetCustomGeometryFilter(AddressOf CircleWithRadiusOf10GeometryFilter)
      ' custom geometry filter
      gcc10.DisablePreSelect()
      gcc10.SubObjectSelect = False
      gcc10.[Get]()
      If gcc10.CommandResult() <> Result.Success Then
        Return gcc10.CommandResult()
      End If
      If gcc10.[Object](0).Curve() Is Nothing Then
        Return Result.Failure
      End If
      Rhino.RhinoApp.WriteLine("circle with radius of 10 was selected")

      Return Result.Success
    End Function

    Private Function CircleWithRadiusOf10GeometryFilter(rhObject As Rhino.DocObjects.RhinoObject, geometry As GeometryBase, componentIndex As ComponentIndex) As Boolean
      Dim isCircleWithRadiusOf10 As Boolean = False
      Dim circle As Circle
      If TypeOf geometry Is Curve AndAlso TryCast(geometry, Curve).TryGetCircle(circle) Then
        isCircleWithRadiusOf10 = circle.Radius <= 10.0 + _tolerance AndAlso circle.Radius >= 10.0 - _tolerance
      End If
      Return isCircleWithRadiusOf10
    End Function
  End Class
End Namespace