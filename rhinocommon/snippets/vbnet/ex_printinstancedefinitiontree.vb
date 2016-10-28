Imports Rhino
Imports Rhino.DocObjects
Imports Rhino.FileIO
Imports Rhino.Commands

Namespace examples_vb
  Public Class InstanceDefinitionTreeCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbInstanceDefinitionTree"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim instanceDefinitions = doc.InstanceDefinitions
      Dim instanceDefinitionCount = instanceDefinitions.Count

      If instanceDefinitionCount = 0 Then
        RhinoApp.WriteLine("Document contains no instance definitions.")
        Return Result.[Nothing]
      End If

      Dim dump = New TextLog()
      dump.IndentSize = 4

      For i As Integer = 0 To instanceDefinitionCount - 1
        DumpInstanceDefinition(instanceDefinitions(i), dump, True)
      Next

      RhinoApp.WriteLine(dump.ToString())

      Return Result.Success
    End Function

    Private Sub DumpInstanceDefinition(instanceDefinition As InstanceDefinition, ByRef dump As TextLog, isRoot As Boolean)
      If instanceDefinition IsNot Nothing AndAlso Not instanceDefinition.IsDeleted Then
        Dim node As String
        If isRoot Then
          node = "─"
        Else
          '"\u2500"; 
          node = "└"
        End If
        '"\u2514"; 
        dump.Print(String.Format("{0} Instance definition {1} = {2}" & vbLf, node, instanceDefinition.Index, instanceDefinition.Name))

        If instanceDefinition.ObjectCount > 0 Then
          dump.PushIndent()
          For i As Integer = 0 To instanceDefinition.ObjectCount - 1
            Dim obj = instanceDefinition.[Object](i)

            If obj Is Nothing Then Continue For

            If TypeOf obj Is InstanceObject Then
              DumpInstanceDefinition(TryCast(obj, InstanceObject).InstanceDefinition, dump, False)
            Else
              ' Recursive...
              dump.Print(String.Format("└ Object {0} = {1}" & vbLf, i, obj.ShortDescription(False)))
            End If
          Next
          dump.PopIndent()
        End If
      End If
    End Sub
  End Class
End Namespace