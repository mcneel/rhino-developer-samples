Imports Rhino
Imports Rhino.Commands
Imports Rhino.Input

Namespace examples_vb
  Public Class RhinoPageViewWidthHeightCommand
    Inherits Command
    Public Overrides ReadOnly Property EnglishName() As String
      Get
        Return "vbSetRhinoPageViewWidthAndHeight"
      End Get
    End Property

    Protected Overrides Function RunCommand(doc As RhinoDoc, mode As RunMode) As Result
      Dim width = 1189
      Dim height = 841
      Dim page_views = doc.Views.GetPageViews()
      Dim page_number As Integer = If((page_views Is Nothing), 1, page_views.Length + 1)
      Dim pageview = doc.Views.AddPageView(String.Format("A0_{0}", page_number), width, height)

      Dim new_width As Integer = width
      Dim rc = RhinoGet.GetInteger("new width", False, new_width)
      If rc <> Result.Success OrElse new_width <= 0 Then
        Return rc
      End If

      Dim new_height As Integer = height
      rc = RhinoGet.GetInteger("new height", False, new_height)
      If rc <> Result.Success OrElse new_height <= 0 Then
        Return rc
      End If

      pageview.PageWidth = new_width
      pageview.PageHeight = new_height
      doc.Views.Redraw()
      Return Result.Success
    End Function
  End Class
End Namespace