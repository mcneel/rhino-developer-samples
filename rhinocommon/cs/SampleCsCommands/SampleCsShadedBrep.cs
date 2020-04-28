using System;
using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsShadedBrepConduit display conduit
  /// </summary>
  class SampleCsShadedBrepConduit : DisplayConduit
  {
    private Brep m_brep;

    public SampleCsShadedBrepConduit()
    {
      m_brep = null;
    }

    public Brep BrepGeometry
    {
      get => m_brep;
      set 
      { 
        m_brep = value;
        if (null != m_brep && m_brep.IsValid)
        {
          if (m_brep.SolidOrientation == BrepSolidOrientation.Inward)
            m_brep.Flip();
        }
      }
    }

    protected override void CalculateBoundingBox(CalculateBoundingBoxEventArgs e)
    {
      if (null != m_brep && m_brep.IsValid)
      {
        e.IncludeBoundingBox(BrepGeometry.GetBoundingBox(false));
      }
    }

    /// <summary>
    /// If you’re drawing shaded objects, then you’ll probably want to draw them in either
    /// the PreDrawObjects or PostDrawObjects channel. However, if transparency is involved,
    /// then you’ll have to draw them in PostDrawObjects in order for all other objects to
    /// “show through” your transparent ones.
    /// </summary>
    protected override void PostDrawObjects(DrawEventArgs e)
    {
      if (null != m_brep && m_brep.IsValid)
      {
        DisplayMaterial material = new DisplayMaterial();
        material.Diffuse = Color.RoyalBlue;
        e.Display.DrawBrepShaded(m_brep, material);
      }    
    }
  }

  /// <summary>
  /// SampleCsShadedBrep command
  /// </summary>
  public class SampleCsShadedBrep : Command
  {
    public override string EnglishName => "SampleCsShadedBrep";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Plane plane = Plane.WorldXY;
      double radius = 10.0;
      double height = 2.0;

      Circle circle = new Circle(plane, radius);
      Cylinder cylinder = new Cylinder(circle, height);

      Brep brep = Brep.CreateFromCylinder(cylinder, true, true);

      SampleCsShadedBrepConduit conduit = new SampleCsShadedBrepConduit();
      conduit.BrepGeometry = brep;
      conduit.Enabled = true;
      doc.Views.Redraw();

      string out_str = null;
      Result rc = RhinoGet.GetString("Press <Enter> to continue", true, ref out_str);

      conduit.Enabled = false;

      doc.Objects.AddBrep(brep);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}
