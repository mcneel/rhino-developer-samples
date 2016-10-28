Imports Rhino

Public Class ex_selbyusertext
  Inherits Rhino.Commands.SelCommand

  Public Overrides ReadOnly Property EnglishName() As String
    Get
      Return "vbSelByUserText"
    End Get
  End Property

  Dim m_key As String = String.Empty
  Protected Overrides Function RunCommand(ByVal doc As RhinoDoc, ByVal mode As Rhino.Commands.RunMode) As Rhino.Commands.Result
    ' You don't have to override RunCommand if you don't need any user input. In
    ' this case we want to get a key from the user. If you return something other
    ' than Success, the selection is canceled
    Return Rhino.Input.RhinoGet.GetString("key", True, m_key)
  End Function

  Protected Overrides Function SelFilter(rhObj As Rhino.DocObjects.RhinoObject) As Boolean
    Dim s As String = rhObj.Attributes.GetUserString(m_key)
    Return Not String.IsNullOrEmpty(s)
  End Function
End Class
