using System;

partial class Examples
{
  public static Rhino.Commands.Result AddBackgroundBitmap(Rhino.RhinoDoc doc)
  {
    // Allow the user to select a bitmap file
    var fd = new Rhino.UI.OpenFileDialog { Filter = "Image Files (*.bmp;*.png;*.jpg)|*.bmp;*.png;*.jpg" };
	  if (!fd.ShowOpenDialog())
      return Rhino.Commands.Result.Cancel;

    // Verify the file that was selected
    System.Drawing.Image image;
    try
    {
      image = System.Drawing.Image.FromFile(fd.FileName);
    }
    catch (Exception)
    {
      return Rhino.Commands.Result.Failure;
    }

    // Allow the user to pick the bitmap origin
    var gp = new Rhino.Input.Custom.GetPoint();
    gp.SetCommandPrompt("Bitmap Origin");
    gp.ConstrainToConstructionPlane(true);
    gp.Get();
    if (gp.CommandResult() != Rhino.Commands.Result.Success)
      return gp.CommandResult();

    // Get the view that the point was picked in.
    // This will be the view that the bitmap appears in.
    var view = gp.View();
    if (view == null)
    {
      view = doc.Views.ActiveView;
      if (view == null)
        return Rhino.Commands.Result.Failure;
    }

    // Allow the user to specify the bitmap width in model units
    var gn = new Rhino.Input.Custom.GetNumber();
    gn.SetCommandPrompt("Bitmap width");
    gn.SetLowerLimit(1.0, false);
    gn.Get();
    if (gn.CommandResult() != Rhino.Commands.Result.Success)
      return gn.CommandResult();

    // Cook up some scale factors
    var w = gn.Number();
    var image_width = image.Width;
    var image_height = image.Height;
    var h = w * (image_height / image_width);

    var plane = view.ActiveViewport.ConstructionPlane();
    plane.Origin = gp.Point();
    view.ActiveViewport.SetTraceImage(fd.FileName, plane, w, h, false, false);
    view.Redraw();
    return Rhino.Commands.Result.Success;
  }
}
