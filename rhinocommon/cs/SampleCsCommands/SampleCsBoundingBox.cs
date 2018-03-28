using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsBoundingBox : Command
  {
    private bool m_use_cplane = false;

    public SampleCsBoundingBox()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsBoundingBox"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var cs_option = new OptionToggle(m_use_cplane, "World", "CPlane");

      var go = new GetObject();
      go.SetCommandPrompt("Select objects for bounding box calculation");
      go.GroupSelect = true;
      go.SubObjectSelect = false;
      for (;;)
      {
        go.ClearCommandOptions();
        go.AddOptionToggle("CoordinateSystem", ref cs_option);

        var res = go.GetMultiple(1, 0);

        if (res == GetResult.Option)
          continue;
        else if (res != GetResult.Object)
          return Result.Cancel;

        break;
      }

      var plane = go.View().ActiveViewport.ConstructionPlane();
      m_use_cplane = cs_option.CurrentValue;

      var world_to_plane = new Transform(1.0);
      if (m_use_cplane)
        world_to_plane = Transform.ChangeBasis(Plane.WorldXY, plane);
      
      var bounding_box = new BoundingBox();
      for (var i = 0; i < go.ObjectCount; i++)
      {
        var rhino_obj = go.Object(i).Object();
        if (null != rhino_obj)
        {
          var box = rhino_obj.Geometry.GetBoundingBox(world_to_plane);
          if (box.IsValid)
          {
            if (i == 0)
              bounding_box = box;
            else
              bounding_box.Union(box);
          }
        }
      }

      if (!bounding_box.IsValid)
      {
        RhinoApp.WriteLine("BoundingBox failed. Unable to calculate bounding box.");
        return Result.Failure;
      }

      var box_corners = new Point3d[8];
      box_corners[0] = bounding_box.Corner(false, false, false);
      box_corners[1] = bounding_box.Corner(true, false, false);
      box_corners[2] = bounding_box.Corner(true, true, false);
      box_corners[3] = bounding_box.Corner(false, true, false);
      box_corners[4] = bounding_box.Corner(false, false, true);
      box_corners[5] = bounding_box.Corner(true, false, true);
      box_corners[6] = bounding_box.Corner(true, true, true);
      box_corners[7] = bounding_box.Corner(false, true, true);

      if (m_use_cplane)
      {
        // Transform corners points from cplane coordinates
        // to world coordinates if necessary.
        var plane_to_world = Transform.ChangeBasis(plane, Plane.WorldXY);
        for (var i = 0; i < 8; i++)
          box_corners[i].Transform(plane_to_world);
      }
      
      Point3d[] rect;
      var type = ClassifyBoundingBox(box_corners, out rect);

      if (type == BoundingBoxClassification.Point)
      {
        RhinoApp.WriteLine("BoundingBox failed. The bounding box is a point.");
      }
      else if (type == BoundingBoxClassification.Line)
      {
        RhinoApp.WriteLine("BoundingBox failed. The bounding box is a line.");
      }
      else if (type == BoundingBoxClassification.Rectangle)
      {
        doc.Objects.AddPolyline(rect);
      }
      else //if (type == BoundingBoxClassification.Box)
      {
        var brep = Brep.CreateFromBox(box_corners);
        doc.Objects.AddBrep(brep);
      }

      doc.Views.Redraw();

      return Result.Success;
    }

    private enum BoundingBoxClassification
    {
      Invalid = -1,
      Box = 0,
      Rectangle = 1,
      Line = 2,
      Point = 3
    }

    private BoundingBoxClassification GetBoundingBoxClassification(int i)
    {
      switch (i)
      {
        case 0:
          return BoundingBoxClassification.Box;
        case 1:
          return BoundingBoxClassification.Rectangle;
        case 2:
          return BoundingBoxClassification.Line;
        case 3:
          return BoundingBoxClassification.Point;
      }
      return BoundingBoxClassification.Invalid;
    }

    private BoundingBoxClassification ClassifyBoundingBox(Point3d[] box_corners, out Point3d[] rect)
    {
      rect = new Point3d[5];

      var num_flat = 0;
      var xflat = false;
      var yflat = false;
      //var zflat = false;

      const float flt_epsilon = 1.192092896e-07F;

      // flat in box x-direction?
      if (flt_epsilon > box_corners[0].DistanceTo(box_corners[1]))
      {
        num_flat++;
        xflat = true;
      }

      // flat in box y-direction?
      if (flt_epsilon > box_corners[0].DistanceTo(box_corners[3]))
      {
        num_flat++;
        yflat = true;
      }

      // flat in box z-direction?
      if (flt_epsilon > box_corners[0].DistanceTo(box_corners[4]))
      {
        num_flat++;
        //zflat = true;
      }

      if (num_flat == 1)
      {
        rect[0] = rect[4] = box_corners[0];
        if (xflat)
        {
          rect[1] = box_corners[4];
          rect[2] = box_corners[7];
          rect[3] = box_corners[3];
        }
        else if (yflat)
        {
          rect[1] = box_corners[1];
          rect[2] = box_corners[5];
          rect[3] = box_corners[4];
        }
        else // zflat
        {
          rect[1] = box_corners[1];
          rect[2] = box_corners[2];
          rect[3] = box_corners[3];
        }
      }

      return GetBoundingBoxClassification(num_flat);
    }
  }
}
