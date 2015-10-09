Partial Class Examples
  Public Shared Function IsBrepBox(brep As Rhino.Geometry.Brep) As Boolean
    Const zero_tolerance As Double = 0.000001 ' or whatever
    Dim rc As Boolean = brep.IsSolid
    If rc Then
      rc = brep.Faces.Count = 6
    End If

    Dim N = New Rhino.Geometry.Vector3d(5) {}
    Dim i As Integer = 0
    While rc AndAlso i < 6
      Dim plane As Rhino.Geometry.Plane
      rc = brep.Faces(i).TryGetPlane(plane, zero_tolerance)
      If rc Then
        N(i) = plane.ZAxis
        N(i).Unitize()
      End If
      i += 1
    End While

    i = 0
    While rc AndAlso i < 6
      Dim count As Integer = 0
      Dim j As Integer = 0
      While rc AndAlso j < 6
        Dim dot As Double = Math.Abs(N(i) * N(j))
        If dot <= zero_tolerance Then
          Continue While
        End If
        If Math.Abs(dot - 1.0) <= zero_tolerance Then
          count += 1
        Else
          rc = False
        End If
        j += 1
      End While

      If rc Then
        If 2 <> count Then
          rc = False
        End If
      End If
      i += 1
    End While
    Return rc
  End Function

  Public Shared Function TestBrepBox(doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    Dim obj_ref As Rhino.DocObjects.ObjRef = Nothing
    Dim rc = Rhino.Input.RhinoGet.GetOneObject("Select Brep", True, Rhino.DocObjects.ObjectType.Brep, obj_ref)
    If rc = Rhino.Commands.Result.Success Then
      Dim brep = obj_ref.Brep()
      If brep IsNot Nothing Then
        If IsBrepBox(brep) Then
          Rhino.RhinoApp.WriteLine("Yes it is a box")
        Else
          Rhino.RhinoApp.WriteLine("No it is not a box")
        End If
      End If
    End If
    Return rc
  End Function
End Class
