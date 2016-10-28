Imports System.Collections.Generic

Partial Class Examples
  Public Shared Function BooleanDifference(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim rc As Rhino.Commands.Result
    Dim objrefs As Rhino.DocObjects.ObjRef() = Nothing
    rc = Rhino.Input.RhinoGet.GetMultipleObjects("Select first set of polysurfaces", False, Rhino.DocObjects.ObjectType.PolysrfFilter, objrefs)
    If rc <> Rhino.Commands.Result.Success Then
      Return rc
    End If
    If objrefs Is Nothing OrElse objrefs.Length < 1 Then
      Return Rhino.Commands.Result.Failure
    End If

    Dim in_breps0 As New List(Of Rhino.Geometry.Brep)()
    For i As Integer = 0 To objrefs.Length - 1
      Dim brep As Rhino.Geometry.Brep = objrefs(i).Brep()
      If brep IsNot Nothing Then
        in_breps0.Add(brep)
      End If
    Next

    doc.Objects.UnselectAll()
    rc = Rhino.Input.RhinoGet.GetMultipleObjects("Select second set of polysurfaces", False, Rhino.DocObjects.ObjectType.PolysrfFilter, objrefs)
    If rc <> Rhino.Commands.Result.Success Then
      Return rc
    End If
    If objrefs Is Nothing OrElse objrefs.Length < 1 Then
      Return Rhino.Commands.Result.Failure
    End If

    Dim in_breps1 As New List(Of Rhino.Geometry.Brep)()
    For i As Integer = 0 To objrefs.Length - 1
      Dim brep As Rhino.Geometry.Brep = objrefs(i).Brep()
      If brep IsNot Nothing Then
        in_breps1.Add(brep)
      End If
    Next

    Dim tolerance As Double = doc.ModelAbsoluteTolerance
    Dim breps As Rhino.Geometry.Brep() = Rhino.Geometry.Brep.CreateBooleanDifference(in_breps0, in_breps1, tolerance)
    If breps.Length < 1 Then
      Return Rhino.Commands.Result.[Nothing]
    End If
    For i As Integer = 0 To breps.Length - 1
      doc.Objects.AddBrep(breps(i))
    Next
    doc.Views.Redraw()
    Return Rhino.Commands.Result.Success
  End Function
End Class
