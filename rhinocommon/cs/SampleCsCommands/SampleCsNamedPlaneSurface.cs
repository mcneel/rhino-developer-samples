using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;

namespace SampleCsCommands
{
  [System.Runtime.InteropServices.Guid("9d8378ef-9087-4ebd-b17e-b53b82ed3483")]
  public class SampleCsNamedPlaneSurface : Command
  {
    public SampleCsNamedPlaneSurface()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsNamedPlaneSurface"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Geometry.Point3d[] corners;
      Result rc = Rhino.Input.RhinoGet.GetRectangle(out corners);
      if (rc != Result.Success)
        return rc;

      Rhino.Geometry.Plane plane = new Rhino.Geometry.Plane(corners[0], corners[1], corners[2]);

      Rhino.Geometry.PlaneSurface plane_surface = new Rhino.Geometry.PlaneSurface(
        plane,
        new Rhino.Geometry.Interval(0, corners[0].DistanceTo(corners[1])),
        new Rhino.Geometry.Interval(0, corners[1].DistanceTo(corners[2]))
        );

      rc = Result.Cancel;
      if (plane_surface.IsValid)
      {
        string layer_name = doc.Layers.GetUnusedLayerName(true);
        rc = Rhino.Input.RhinoGet.GetString("Name of layer to create", true, ref layer_name);
        if (rc == Result.Success && !string.IsNullOrEmpty(layer_name))
        {
          int layer_index = doc.Layers.FindByFullPath(layer_name, true);
          if (-1 == layer_index)
            layer_index = doc.Layers.Add(layer_name, System.Drawing.Color.Black);

          if (layer_index >= 0)
          {
            Rhino.DocObjects.ObjectAttributes attribs = doc.CreateDefaultAttributes();
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
