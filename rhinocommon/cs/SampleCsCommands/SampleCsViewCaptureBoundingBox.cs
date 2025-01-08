using Eto.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.UI;
using Rhino.UI.Forms;
using System.Drawing;
using System.Linq;
using Command = Rhino.Commands.Command;

namespace SampleCsCommands
{
  public class SampleCsViewCaptureBoundingBox : Command
  {
    static SampleCsViewCaptureBoundingBox _instance;
    public SampleCsViewCaptureBoundingBox()
    {
      _instance = this;
    }

    ///<summary>The only instance of the SampleCsViewCaptureBoundingBox command.</summary>
    public static SampleCsViewCaptureBoundingBox Instance
    {
      get { return _instance; }
    }

    public override string EnglishName
    {
      get { return "SampleCsViewCaptureBoundingBox"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {

      //Grab a bounding box off of the scenes normal objects.. 
      System.Collections.Generic.List<Rhino.DocObjects.RhinoObject> normal_objects = doc.Objects.Where(c => c.IsNormal).ToList();
      if (!normal_objects.Any()) return Result.Cancel;

      //Figure out the bounding box of the normal objects in the scene.
      BoundingBox scene_bounding_box = BoundingBox.Unset;
      foreach (Rhino.DocObjects.RhinoObject ro in normal_objects)
        scene_bounding_box.Union(ro.Geometry.GetBoundingBox(true));

      if (!scene_bounding_box.IsValid) { return Result.Cancel; }

      //By default Zoom commands have a 1.1 padding around them.
      //We can temporarily override this padding by setting it to 0
      //Make sure we set it back when finished. 
      Rhino.ApplicationSettings.ViewSettings.ZoomExtentsParallelViewBorder = 0;

      //Disable view drawing while we work
      doc.Views.RedrawEnabled = false;


      //Find a view to base our grab off of and establish a scale. 
      RhinoView top_view = doc.Views.Find("top", false);
      top_view.ActiveViewport.GetWorldToScreenScale(scene_bounding_box.Center, out double screen_scale);

      //figure out the width and height
      double width_x = scene_bounding_box.Min.DistanceTo(scene_bounding_box.Corner(false, true, true)) * screen_scale;
      double height_y = scene_bounding_box.Min.DistanceTo(scene_bounding_box.Corner(true, false, true)) * screen_scale;

      //create the size of our view
      Size bb_size = new Size((int)width_x, (int)height_y);

      //Create a temporary view for the capture
      RhinoView bb_view = doc.Views.Add("bbview", DefinedViewportProjection.Top, new Rectangle(new System.Drawing.Point(0, 0), bb_size), true);

      //Setting the size again will snug up the camera.
      bb_view.Size = bb_size;

      //pull the camera as tight as possible
      bb_view.ActiveViewport.ZoomBoundingBox(scene_bounding_box);

      //create a bitmap view capture
      Bitmap bmp = bb_view.CaptureToBitmap(DisplayModeDescription.FindByName("Shaded"));

      //close the temp view
      bb_view.Close();

      //Reset the zoom padding
      Rhino.ApplicationSettings.ViewSettings.ZoomExtentsParallelViewBorder = 1.1;

      //Re-enable view drawing
      doc.Views.RedrawEnabled = true;

      //Make a simple eto dialog to show the bitmap
      CommandDialog dlg = new CommandDialog
      {
        Size = new Eto.Drawing.Size(800, 600),
        Content = new ImageView() { Image = bmp.ToEto() },
        ShowHelpButton = false,
        Resizable = true,
      };
      dlg.ShowModal(RhinoEtoApp.MainWindow);


      //fin
      return Result.Success;
    }
  }
}