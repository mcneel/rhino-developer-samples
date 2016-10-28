Partial Friend Class Examples
  Public Shared Function SetRhinoPageViewWidthAndHeight(ByVal doc As RhinoDoc) As Result
	Dim width = 1189
	Dim height = 841
	Dim page_views = doc.Views.GetPageViews()
	Dim page_number As Integer = If(page_views Is Nothing, 1, page_views.Length + 1)
	Dim pageview = doc.Views.AddPageView(String.Format("A0_{0}",page_number), width, height)

	Dim new_width As Integer = width
	Dim rc = RhinoGet.GetInteger("new width", False, new_width)
	If rc IsNot Result.Success OrElse new_width <= 0 Then
		Return rc
	End If

	Dim new_height As Integer = height
	rc = RhinoGet.GetInteger("new height", False, new_height)
	If rc IsNot Result.Success OrElse new_height <= 0 Then
		Return rc
	End If

	pageview.PageWidth = new_width
	pageview.PageHeight = new_height
	doc.Views.Redraw()
	Return Result.Success
  End Function
End Class
