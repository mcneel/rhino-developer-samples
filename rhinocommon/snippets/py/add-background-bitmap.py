import Rhino
import scriptcontext
import System.Windows.Forms.DialogResult
import System.Drawing.Image

def AddBackgroundBitmap():
    # Allow the user to select a bitmap file
    fd = Rhino.UI.OpenFileDialog()
    fd.Filter = "Image Files (*.bmp;*.png;*.jpg)|*.bmp;*.png;*.jpg"
    if not fd.ShowDialog():
        return Rhino.Commands.Result.Cancel

    # Verify the file that was selected
    image = None
    try:
        image = System.Drawing.Image.FromFile(fd.FileName)
    except:
        return Rhino.Commands.Result.Failure

    # Allow the user to pick the bitmap origin
    gp = Rhino.Input.Custom.GetPoint()
    gp.SetCommandPrompt("Bitmap Origin")
    gp.ConstrainToConstructionPlane(True)
    gp.Get()
    if gp.CommandResult()!=Rhino.Commands.Result.Success:
        return gp.CommandResult()

    # Get the view that the point was picked in.
    # This will be the view that the bitmap appears in.
    view = gp.View()
    if view is None:
        view = scriptcontext.doc.Views.ActiveView
        if view is None: return Rhino.Commands.Result.Failure

    # Allow the user to specify the bitmap with in model units
    gn = Rhino.Input.Custom.GetNumber()
    gn.SetCommandPrompt("Bitmap width")
    gn.SetLowerLimit(1.0, False)
    gn.Get()
    if gn.CommandResult()!=Rhino.Commands.Result.Success:
        return gn.CommandResult()

    # Cook up some scale factors
    w = gn.Number()
    h = w * (image.Width / image.Height)

    plane = view.ActiveViewport.ConstructionPlane()
    plane.Origin = gp.Point()
    view.ActiveViewport.SetTraceImage(fd.FileName, plane, w, h, False, False)
    view.Redraw()
    return Rhino.Commands.Result.Success

if __name__=="__main__":
    AddBackgroundBitmap()
