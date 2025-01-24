using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using System.Collections.Generic;
using System.Drawing;

namespace SampleCsCommands
{
  public class SampleCsDrawPin : Command
  {
    private readonly SampleCsPinConduit m_conduit = new SampleCsPinConduit();

    public override string EnglishName => "SampleCsDrawPin";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      if (m_conduit.Enabled)
      {
        m_conduit.Enabled = false;
      }
      else
      {
        if (RhinoGet.GetOneObject("Select surface", false, ObjectType.Surface, out ObjRef obj) == Result.Success)
        {
          m_conduit.Points.Clear();
          Surface surface = obj.Surface();
          Interval u_domain = surface.Domain(0);
          u_domain.MakeIncreasing();
          Interval v_domain = surface.Domain(1);
          v_domain.MakeIncreasing();
          const int count = 10;
          for (int i = 0; i < count + 1; i++)
          {
            for (int j = 0; j < count + 1; j++)
            {
              double u = u_domain.Min + u_domain.Length * i / count;
              double v = v_domain.Min + v_domain.Length * j / count;
              Point3d point = surface.PointAt(u, v);
              m_conduit.Points.Add(point);
            }
          }
          m_conduit.Enabled = true;
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }

  internal class SampleCsPinConduit : DisplayConduit
  {
    public List<Point3d> Points { get; set; } = new List<Point3d>();

    protected override void PostDrawObjects(DrawEventArgs e)
    {
      foreach (Point3d point in Points)
        e.Display.DrawPoint(point, PointStyle.Pin, Color.Lavender, Color.Blue, 1, .1f, .4f, 0, false, false);
    }
  }
}