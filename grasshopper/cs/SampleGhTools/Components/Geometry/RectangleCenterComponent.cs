using System;
using Grasshopper.Kernel;
using Grasshopper.Kernel.Types;
using Rhino;
using Rhino.Geometry;

namespace SampleGhTools.Components.Geometry
{
  public class RectangleCenterComponent : GH_Component
  {
    public RectangleCenterComponent()
      : base("Rectangle Center", "CRect", "Create a center rectangle on a plane", "Sample", "Geometry")
    {
    }

    protected override void RegisterInputParams(GH_Component.GH_InputParamManager pManager)
    {
      pManager.AddPlaneParameter("Plane", "P", "Rectangle base plane", GH_ParamAccess.item, Plane.WorldXY);
      pManager.AddNumberParameter("X Size", "X", "Dimensions of rectangle in plane X direction.", GH_ParamAccess.item, 1.0);
      pManager.AddNumberParameter("Y Size", "Y", "Dimensions of rectangle in plane y direction.", GH_ParamAccess.item, 1.0);
      pManager.AddNumberParameter("Radius", "R", "Rectangle corner fillet radius", GH_ParamAccess.item, 0.0);
    }
    
    protected override void RegisterOutputParams(GH_Component.GH_OutputParamManager pManager)
    {
      pManager.AddGenericParameter("Rectangle", "R", "Rectangle", GH_ParamAccess.item);
      pManager.AddNumberParameter("Length", "L", "Length of rectangle curve", GH_ParamAccess.item);
    }

    protected override void SolveInstance(IGH_DataAccess DA)
    {
      var plane = Plane.Unset;
      var x = RhinoMath.UnsetValue;
      var y = RhinoMath.UnsetValue;
      var r = RhinoMath.UnsetValue;

      if (!DA.GetData(0, ref plane)) { return; }
      if (!DA.GetData(1, ref x)) { return; }
      if (!DA.GetData(2, ref y)) { return; }
      if (!DA.GetData(3, ref r)) { return; }

      if (!plane.IsValid) { return; }
      if (!RhinoMath.IsValidDouble(x)) { return; }
      if (!RhinoMath.IsValidDouble(y)) { return; }
      if (!RhinoMath.IsValidDouble(r)) { return; }

      if ((Math.Abs(x) < 1e-12) || (Math.Abs(y) < 1e-12))
      {
        AddRuntimeMessage(GH_RuntimeMessageLevel.Warning, "Rectangle diagonal cannot be zero length.");
        return;
      }

      var min = Math.Min(x, y);
      r = Math.Min(r, 0.5 * min);

      if (r <= 0.0)
      {
        var dx = x * 0.5;
        var dy = y * 0.5;
        var width = new Interval(-dx, dx);
        var height = new Interval(-dy, dy);
        var rect = new Rectangle3d(plane, width, height);
        DA.SetData(0, new GH_Rectangle(rect));
        DA.SetData(1, rect.Circumference);
        return;
      }

      if ((x == y) && (r == (0.5 * x)))
      {
        var circle = new Circle(plane, r);
        DA.SetData(0, new GH_Circle(circle));
        DA.SetData(1, circle.Circumference);
        return;
      }

      var curve = RoundedRectangle(plane, x, y, r);
      if (null == curve)
      {
        AddRuntimeMessage(GH_RuntimeMessageLevel.Error, "Rectangle construction failed.");
        return;
      }

      DA.SetData(0, curve);
      DA.SetData(1, curve.GetLength());
    }

    private Curve RoundedRectangle(Plane plane, double width, double height, double radius)
    {
      var dw = width * 0.5;
      var dh = height * 0.5;
      var x_interval = new Interval(-dw, dw);
      var y_interval = new Interval(-dh, dh);

      var rect = new Rectangle3d(plane, x_interval, y_interval);
      var a = rect.Corner(0);
      var b = rect.Corner(1);
      var c = rect.Corner(2);
      var d = rect.Corner(3);

      var x = plane.XAxis * radius;
      var y = plane.YAxis * radius;

      var crv = new PolyCurve();

      // bottom edge
      if (width > 2 * radius)
        crv.Append(new Line(a + x, b - x));

      // lower right fillet
      crv.Append(new Arc(b - x, plane.XAxis, b + y));

      // right edge
      if (height > 2 * radius)
        crv.Append(new Line(b + y, c - y));

      // upper right fillet
      crv.Append(new Arc(c - y, plane.YAxis, c - x));

      // top edge
      if (width > 2 * radius)
        crv.Append(new Line(c - x, d + x));

      // upper left fillet
      crv.Append(new Arc(d + x, -plane.XAxis, d - y));

      // left edge
      if (height > 2 * radius)
        crv.Append(new Line(d - y, a + y));

      // bottom left fillet
      crv.Append(new Arc(a + y, -plane.YAxis, a + x));

      return crv;
    }

    public override GH_Exposure Exposure => GH_Exposure.quarternary | GH_Exposure.obscure;

    protected override System.Drawing.Bitmap Icon => Properties.Resources.RectangleCenter_24x24;

    public override Guid ComponentGuid => new Guid("852b3e76-6725-4a07-a1de-20528f7f9e1b");
  }
}
