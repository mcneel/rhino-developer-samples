Imports Rhino.DocObjects

Partial Class Examples
  Public Shared Function CreateBlock(doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' Select objects to define block
    Dim go = New Rhino.Input.Custom.GetObject()
    go.SetCommandPrompt("Select objects to define block")
    go.ReferenceObjectSelect = False
    go.SubObjectSelect = False
    go.GroupSelect = True

    ' Phantoms, grips, lights, etc., cannot be in blocks.
    Const forbidden_geometry_filter As ObjectType = Rhino.DocObjects.ObjectType.Light Or Rhino.DocObjects.ObjectType.Grip Or Rhino.DocObjects.ObjectType.Phantom
    Const geometry_filter As ObjectType = forbidden_geometry_filter Xor Rhino.DocObjects.ObjectType.AnyObject
    go.GeometryFilter = geometry_filter
    go.GetMultiple(1, 0)
    If go.CommandResult() <> Rhino.Commands.Result.Success Then
      Return go.CommandResult()
    End If

    ' Block base point
    Dim base_point As Rhino.Geometry.Point3d
    Dim rc = Rhino.Input.RhinoGet.GetPoint("Block base point", False, base_point)
    If rc <> Rhino.Commands.Result.Success Then
      Return rc
    End If

    ' Block definition name
    Dim idef_name As String = ""
    rc = Rhino.Input.RhinoGet.GetString("Block definition name", False, idef_name)
    If rc <> Rhino.Commands.Result.Success Then
      Return rc
    End If
    ' Validate block name
    idef_name = idef_name.Trim()
    If String.IsNullOrEmpty(idef_name) Then
      Return Rhino.Commands.Result.[Nothing]
    End If

    ' See if block name already exists
    Dim existing_idef As Rhino.DocObjects.InstanceDefinition = doc.InstanceDefinitions.Find(idef_name, True)
    If existing_idef IsNot Nothing Then
      Rhino.RhinoApp.WriteLine("Block definition {0} already exists", idef_name)
      Return Rhino.Commands.Result.[Nothing]
    End If

    ' Gather all of the selected objects
    Dim geometry = New System.Collections.Generic.List(Of Rhino.Geometry.GeometryBase)()
    Dim attributes = New System.Collections.Generic.List(Of Rhino.DocObjects.ObjectAttributes)()
    For i As Integer = 0 To go.ObjectCount - 1
      Dim rhinoObject = go.Object(i).[Object]()
      If rhinoObject IsNot Nothing Then
        geometry.Add(rhinoObject.Geometry)
        attributes.Add(rhinoObject.Attributes)
      End If
    Next

    ' Gather all of the selected objects
    Dim idef_index As Integer = doc.InstanceDefinitions.Add(idef_name, String.Empty, base_point, geometry, attributes)

    If idef_index < 0 Then
      Rhino.RhinoApp.WriteLine("Unable to create block definition", idef_name)
      Return Rhino.Commands.Result.Failure
    End If
    Return Rhino.Commands.Result.Failure
  End Function
End Class
