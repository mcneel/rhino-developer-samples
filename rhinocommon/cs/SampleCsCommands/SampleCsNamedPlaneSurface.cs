using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;

namespace SampleCsCommands
{
  public class SampleCsNamedPlaneSurface : Command
  {
    public override string EnglishName => "SampleCsNamedPlaneSurface";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Point3d[] corners;
      var rc = RhinoGet.GetRectangle(out corners);
      if (rc != Result.Success)
        return rc;

      var plane = new Plane(corners[0], corners[1], corners[2]);

      var plane_surface = new PlaneSurface(
        plane,
        new Interval(0, corners[0].DistanceTo(corners[1])),
        new Interval(0, corners[1].DistanceTo(corners[2]))
        );

      rc = Result.Cancel;
      if (plane_surface.IsValid)
      {
        var layer_name = doc.Layers.GetUnusedLayerName();
        rc = RhinoGet.GetString("Name of layer to create", true, ref layer_name);
        if (rc == Result.Success && !string.IsNullOrEmpty(layer_name))
        {
          var layer_index = doc.Layers.FindByFullPath(layer_name, -1);
          if (-1 == layer_index)
            layer_index = doc.Layers.Add(layer_name, System.Drawing.Color.Black);

          if (layer_index >= 0)
          {
            var attribs = doc.CreateDefaultAttributes();
            attribs.LayerIndex = layer_index;
            attribs.Name = layer_name;
            doc.Objects.AddSurface(plane_surface, attribs);
            doc.Views.Redraw();
            rc = Result.Success;
          }
        }
      }

      return rc;
    }
  }
}
