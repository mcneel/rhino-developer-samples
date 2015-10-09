Imports Rhino
Imports Rhino.Commands
Imports Rhino.DocObjects
Imports Rhino.Geometry
Imports System.Drawing
Imports Rhino.Input

Namespace examples_vb
  Class DeviationConduit
    Inherits Rhino.Display.DisplayConduit
    Private ReadOnly _curveA As Curve
    Private ReadOnly _curveB As Curve
    Private ReadOnly _minDistPointA As Point3d
    Private ReadOnly _minDistPointB As Point3d
    Private ReadOnly _maxDistPointA As Point3d
    Private ReadOnly _maxDistPointB As Point3d

    Public Sub New(curveA As Curve, curveB As Curve, minDistPointA As Point3d, minDistPointB As Point3d, maxDistPointA As Point3d, maxDistPointB As Point3d)
      _curveA = curveA
      _curveB = curveB
      _minDistPointA = minDistPointA
      _minDistPointB = minDistPointB
      _maxDistPointA = maxDistPointA
      _maxDistPointB = maxDistPointB
    End Sub

    Protected Overrides Sub DrawForeground(e As Rhino.Display.DrawEventArgs)
      e.Display.DrawCurve(_curveA, Color.Red)
      e.Display.DrawCurve(_curveB, Color.Red)

      e.Display.DrawPoint(_minDistPointA, Color.LawnGreen)
      e.Display.DrawPoint(_minDistPointB, Color.LawnGreen)
      e.Display.DrawLine(New Line(_minDistPointA, _minDistPointB), Color.LawnGreen)
      e.Display.DrawPoint(_maxDistPointA, Color.Red)
      e.Display.DrawPoint(_maxDistPointB, Color.Red)
      e.Display.DrawLine(New Line(_maxDistPointA, _maxDistPointB), Color.Red)
    End Sub
  End Class


  Public Class CurveDeviationCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbCurveDeviation"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      doc.Objects.UnselectAll()

      Dim objRef1 As ObjRef = Nothing
      Dim rc1 = RhinoGet.GetOneObject("first curve", True, ObjectType.Curve, objRef1)
      If rc1 <> Result.Success Then
        Return rc1
      End If
      Dim curveA As Curve = Nothing
      If objRef1 IsNot Nothing Then
        curveA = objRef1.Curve()
      End If
      If curveA Is Nothing Then
        Return Result.Failure
      End If

      ' Since you already selected a curve if you don't unselect it
      ' the next GetOneObject won't stop as it considers that curve 
      ' input, i.e., curveA and curveB will point to the same curve.
      ' Another option would be to use an instance of Rhino.Input.Custom.GetObject
      ' instead of Rhino.Input.RhinoGet as GetObject has a DisablePreSelect() method.
      doc.Objects.UnselectAll()

      Dim objRef2 As ObjRef = Nothing
      Dim rc2 = RhinoGet.GetOneObject("second curve", True, ObjectType.Curve, objRef2)
      If rc2 <> Result.Success Then
        Return rc2
      End If
      Dim curveB As Curve = Nothing
      If objRef2 IsNot Nothing Then
        curveB = objRef2.Curve()
      End If
      If curveB Is Nothing Then
        Return Result.Failure
      End If

      Dim tolerance = doc.ModelAbsoluteTolerance

      Dim maxDistance As Double
      Dim maxDistanceParameterA As Double
      Dim maxDistanceParameterB As Double
      Dim minDistance As Double
      Dim minDistanceParameterA As Double
      Dim minDistanceParameterB As Double

      Dim conduit As DeviationConduit
      If Not Curve.GetDistancesBetweenCurves(curveA, curveB, tolerance, maxDistance, maxDistanceParameterA, maxDistanceParameterB, _
        minDistance, minDistanceParameterA, minDistanceParameterB) Then
        RhinoApp.WriteLine("Unable to find overlap intervals.")
        Return Result.Success
      Else
        If minDistance <= RhinoMath.ZeroTolerance Then
          minDistance = 0.0
        End If
        Dim maxDistPtA = curveA.PointAt(maxDistanceParameterA)
        Dim maxDistPtB = curveB.PointAt(maxDistanceParameterB)
        Dim minDistPtA = curveA.PointAt(minDistanceParameterA)
        Dim minDistPtB = curveB.PointAt(minDistanceParameterB)

        conduit = New DeviationConduit(curveA, curveB, minDistPtA, minDistPtB, maxDistPtA, maxDistPtB)
        conduit.Enabled = True

        doc.Views.Redraw()
        RhinoApp.WriteLine("Minimum deviation= {0}   pointA= {1}, pointB= {2}", minDistance, minDistPtA, minDistPtB)
        RhinoApp.WriteLine("Maximum deviation= {0}   pointA= {1}, pointB= {2}", maxDistance, maxDistPtA, maxDistPtB)
      End If

      Dim str As String = ""
      RhinoGet.GetString("Press Enter when done", True, str)
      conduit.Enabled = False

      Return Result.Success
    End Function
  End Class
End Namespace