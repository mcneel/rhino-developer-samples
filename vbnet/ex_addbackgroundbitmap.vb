Partial Class Examples
  Public Shared Function AddBackgroundBitmap(ByVal doc As Rhino.RhinoDoc) As Rhino.Commands.Result
    ' Allow the user to select a bitmap file
    Dim fd As New Rhino.UI.OpenFileDialog()
    fd.Filter = "Image Files (*.bmp;*.png;*.jpg)|*.bmp;*.png;*.jpg"
    If fd.ShowDialog() <> System.Windows.Forms.DialogResult.OK Then
      Return Rhino.Commands.Result.Cancel
    End If

    ' Verify the file that was selected
    Dim image As System.Drawing.Image
    Try
      image = System.Drawing.Image.FromFile(fd.FileName)
    Catch generatedExceptionName As Exception
      Return Rhino.Commands.Result.Failure
    End Try

    ' Allow the user to pick the bitmap origin
    Dim gp As New Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Bitmap Origin")
    gp.ConstrainToConstructionPlane(True)
    gp.Get()
    If gp.CommandResult() <> Rhino.Commands.Result.Success Then
      Return gp.CommandResult()
    End If

    ' Get the view that the point was picked in.
    ' This will be the view that the bitmap appears in.
    Dim view As Rhino.Display.RhinoView = gp.View()
    If view Is Nothing Then
      view = doc.Views.ActiveView
      If view Is Nothing Then
        Return Rhino.Commands.Result.Failure
      End If
    End If

    ' Allow the user to specify the bitmap with in model units
    Dim gn As New Rhino.Input.Custom.GetNumber()
    gn.SetCommandPrompt("Bitmap width")
    gn.SetLowerLimit(1.0, False)
    gn.Get()
    If gn.CommandResult() <> Rhino.Commands.Result.Success Then
      Return gn.CommandResult()
    End If

    ' Cook up some scale factors
    Dim w As Double = gn.Number()
    Dim image_width As Double = CDbl(image.Width)
    Dim image_height As Double = CDbl(image.Height)
    Dim h As Double = w * (image_height / image_width)

    Dim plane As Rhino.Geometry.Plane = view.ActiveViewport.ConstructionPlane()
    plane.Origin = gp.Point()
    view.ActiveViewport.SetTraceImage(fd.FileName, plane, w, h, False, False)
    view.Redraw()
    Return Rhino.Commands.Result.Success
  End Function
End Class
