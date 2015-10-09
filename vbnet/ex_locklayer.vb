Imports Rhino
Imports Rhino.Commands
Imports System.Linq

Namespace examples_vb
  Public Class LockLayerCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbLockLayer"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim layerName As String = ""
      Dim rc = Rhino.Input.RhinoGet.GetString("Name of layer to lock", True, layerName)
      If rc <> Result.Success Then
        Return rc
      End If
      If [String].IsNullOrWhiteSpace(layerName) Then
        Return Result.[Nothing]
      End If

      ' because of sublayers it's possible that mone than one layer has the same name
      ' so simply calling doc.Layers.Find(layerName) isn't good enough.  If "layerName" returns
      ' more than one layer then present them to the user and let him decide.
      Dim matchingLayers = (From layer In doc.Layers Where layer.Name = layerName Select layer).ToList()

      Dim layerToLock As Rhino.DocObjects.Layer = Nothing
      If matchingLayers.Count = 0 Then
        RhinoApp.WriteLine([String].Format("Layer ""{0}"" does not exist.", layerName))
        Return Result.[Nothing]
      ElseIf matchingLayers.Count = 1 Then
        layerToLock = matchingLayers(0)
      ElseIf matchingLayers.Count > 1 Then
        For i As Integer = 0 To matchingLayers.Count - 1
          RhinoApp.WriteLine([String].Format("({0}) {1}", i + 1, matchingLayers(i).FullPath.Replace("::", "->")))
        Next
        Dim selectedLayer As Integer = -1
        rc = Rhino.Input.RhinoGet.GetInteger("which layer?", True, selectedLayer)
        If rc <> Result.Success Then
          Return rc
        End If
        If selectedLayer > 0 AndAlso selectedLayer <= matchingLayers.Count Then
          layerToLock = matchingLayers(selectedLayer - 1)
        Else
          Return Result.[Nothing]
        End If
      End If

      If layerToLock Is Nothing Then
        Return Result.Nothing
      End If

      If Not layerToLock.IsLocked Then
        layerToLock.IsLocked = True
        layerToLock.CommitChanges()
        Return Result.Success
      Else
        RhinoApp.WriteLine([String].Format("layer {0} is already locked.", layerToLock.FullPath))
        Return Result.[Nothing]
      End If
    End Function
  End Class
End Namespace