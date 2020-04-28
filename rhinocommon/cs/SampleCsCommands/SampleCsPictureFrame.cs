using System;
using System.Windows.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;

namespace SampleCsCommands
{
  public class SampleCsPictureFrame : Command
  {
    public override string EnglishName => "SampleCsPictureFrame";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      OpenFileDialog dialog = new OpenFileDialog();
      dialog.Filter = @"Image Files|*.bmp;*.gif;*.jpg;*.jpeg;*.pcx;*.png;*.tif;*.tiff";
      DialogResult rc = dialog.ShowDialog();
      if (rc != DialogResult.OK)
        return Result.Cancel;

      string filename = dialog.FileName;

      Point3d[] corners;
      Result res = Rhino.Input.RhinoGet.GetRectangle(out corners);
      if (res != Result.Success)
        return res;
 
      Point3d p0 = corners[0];
      Point3d p1 = corners[1];
      Point3d p3 = corners[3];
      Plane plane = new Rhino.Geometry.Plane(p0, p1, p3);

      double width = p0.DistanceTo(p1);
      double height = p0.DistanceTo(p3);

      doc.Objects.AddPictureFrame(plane, filename, false, width, height, true, false);
      doc.Views.Redraw();
      
      return Result.Success;
    }
  }
}
