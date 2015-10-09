Imports Rhino
Imports Rhino.Commands
Imports System.Linq

Namespace examples_vb
  Public Class ex_printinstancedefinitions
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbInstanceDefinitionNames"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim instanceDefinitionNames = (From instanceDefinition In doc.InstanceDefinitions
                                     Where instanceDefinition IsNot Nothing AndAlso Not instanceDefinition.IsDeleted
                                     Select instanceDefinition.Name)

      For Each n As String In instanceDefinitionNames
        RhinoApp.WriteLine([String].Format("Instance definition = {0}", n))
      Next

      Return Result.Success
    End Function
  End Class
End Namespace