using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsBoxMorph command class
  /// </summary>
  public class SampleCsBoxMorph : Command
  {
    public SampleCsBoxMorph()
    {
    }

    public override string EnglishName
    {
      get { return "SampleCsBoxMorph"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Select objects to define block
      Rhino.Input.Custom.GetObject go = new Rhino.Input.Custom.GetObject();
      go.SetCommandPrompt("Select surface or polysurface to box morph");
      go.GeometryFilter = Rhino.DocObjects.ObjectType.Surface | Rhino.DocObjects.ObjectType.Brep;
      go.SubObjectSelect = false;
      go.Get();
      if (go.CommandResult() != Rhino.Commands.Result.Success)
        return go.CommandResult();

      Rhino.Geometry.Brep brep = go.Object(0).Brep();
      if (null == brep)
        return Result.Failure;

      Rhino.Geometry.BoundingBox bbox = brep.GetBoundingBox(true);

      // The original box points
      Rhino.Geometry.Point3d[] box = new Rhino.Geometry.Point3d[8];
      box[0] = bbox.Corner(true, true, true);
      box[1] = bbox.Corner(false, true, true);
      box[2] = bbox.Corner(false, false, true);
      box[3] = bbox.Corner(true, false, true);
      box[4] = bbox.Corner(true, true, false);
      box[5] = bbox.Corner(false, true, false);
      box[6] = bbox.Corner(false, false, false);
      box[7] = bbox.Corner(true, false, false);

      // The modified box points
      Rhino.Geometry.Point3d[] corners = new Rhino.Geometry.Point3d[8];
      corners[0] = box[0];
      corners[1] = box[1];
      corners[2] = box[2];
      corners[3] = box[3];
      corners[4] = box[4];
      corners[5] = box[5];
      corners[6] = box[6] * 2; // some goofy point
      corners[7] = box[7];

      BoxMorph box_morph = new BoxMorph(box, corners);
      if (box_morph.IsValid() && !box_morph.IsIdentity())
      {
        if (box_morph.Morph(brep))
        {
          doc.Objects.Add(brep);
          doc.Views.Redraw();
        }
      }

      return Result.Success;
    }
  }

  /// <summary>
  /// BoxMorph space morph class
  /// </summary>
  class BoxMorph : Rhino.Geometry.SpaceMorph
  {
    private Rhino.Geometry.Point3d[] m_box;
    private Rhino.Geometry.Point3d[] m_corners;
    private Rhino.Geometry.Line m_ref_x;
    private Rhino.Geometry.Line m_ref_y;
    private Rhino.Geometry.Line m_ref_z;

    public BoxMorph(Rhino.Geometry.Point3d[] box, Rhino.Geometry.Point3d[] corners)
    {
      if (8 == box.Length)
      {
        m_box = new Rhino.Geometry.Point3d[8];
        for (int i = 0; i < 8; i++)
          m_box[i] = box[i];
      }

      if (8 == corners.Length)
      {
        m_corners = new Rhino.Geometry.Point3d[8];
        for (int i = 0; i < 8; i++)
          m_corners[i] = corners[i];
      }

      if (null != m_box)
      {
        m_ref_x = new Rhino.Geometry.Line(m_box[0], m_box[1]);
        m_ref_y = new Rhino.Geometry.Line(m_box[0], m_box[3]);
        m_ref_z = new Rhino.Geometry.Line(m_box[0], m_box[4]);
      }
    }

    public bool IsValid()
    {
      if (null != m_box &&  null != m_corners)
        return true;
      return false;
    }

    public override Rhino.Geometry.Point3d MorphPoint(Rhino.Geometry.Point3d point)
    {
      if (!IsValid())
        return Rhino.Geometry.Point3d.Unset;

      double u = m_ref_x.ClosestParameter(point);
      double v = m_ref_y.ClosestParameter(point);
      double w = m_ref_z.ClosestParameter(point);

      Rhino.Geometry.Point3d ab = m_corners[0] + (m_corners[1] - m_corners[0]) * u;
      Rhino.Geometry.Point3d dc = m_corners[3] + (m_corners[2] - m_corners[3]) * u;
      Rhino.Geometry.Point3d ef = m_corners[4] + (m_corners[5] - m_corners[4]) * u;
      Rhino.Geometry.Point3d hg = m_corners[7] + (m_corners[6] - m_corners[7]) * u;
      Rhino.Geometry.Point3d abdc = ab + (dc - ab) * v;
      Rhino.Geometry.Point3d efhg = ef + (hg - ef) * v;

      return abdc + (efhg - abdc) * w;
    }

    public bool IsIdentity()
    {
      if (!IsValid())
        return false;

      for (int i = 0; i < 8; i++ )
      {
        if( m_corners[i] != m_box[i] )
          return false;
      }

      return true;
    }
  }
}
