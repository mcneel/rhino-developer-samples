using System;
using System.Drawing;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Render;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsZAnalysisOn command
  /// </summary>
  public class SampleCsZAnalysisOn : Command
  {
    public override string EnglishName => "SampleCsZAnalysisOn";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var analysis_mode = VisualAnalysisMode.Find(typeof(SampleCsZAnalysisMode));
      if (null == analysis_mode)
        analysis_mode = VisualAnalysisMode.Register(typeof(SampleCsZAnalysisMode));
      if (null == analysis_mode)
      {
        RhinoApp.WriteLine("Unable to register Z-Analysis mode.");
        return Result.Failure;
      }

      const ObjectType filter = ObjectType.Surface | ObjectType.PolysrfFilter | ObjectType.Mesh;
      var rc = RhinoGet.GetMultipleObjects("Select objects for Z analysis", false, filter, out var obj_refs);
      if (rc != Result.Success)
        return rc;

      var count = 0;
      foreach (var obj_ref in obj_refs)
      {
        var obj = obj_ref.Object();

        // see if this object is already in Z-Analysis mode
        if (obj.InVisualAnalysisMode(analysis_mode))
          continue;

        if (obj.EnableVisualAnalysisMode(analysis_mode, true))
          count++;
      }

      doc.Views.Redraw();
      RhinoApp.WriteLine("{0} object(s) set into Z-Analysis mode.", count);

      return Result.Success;
    }
  }

  /// <summary>
  /// SampleCsZAnalysisOff command
  /// </summary>
  public class SampleCsZAnalysisOff : Command
  {
    public override string EnglishName => "SampleCsZAnalysisOff";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var analysis_mode = VisualAnalysisMode.Find(typeof(SampleCsZAnalysisMode));
      if (analysis_mode != null)
      {
        foreach (var obj in doc.Objects)
          obj.EnableVisualAnalysisMode(analysis_mode, false);
        doc.Views.Redraw();
      }
      RhinoApp.WriteLine("Z-Analysis is off.");
      return Result.Success;
    }
  }

  /// <summary>
  /// This simple example provides a false color based on the world z-coordinate.
  /// For details, see the implementation of the FalseColor() function.
  /// </summary>
  public class SampleCsZAnalysisMode : VisualAnalysisMode
  {
    private Interval m_z_range = new Interval(-10.0, 10.0);
    private Interval m_hue_range = new Interval(0.0, 4.0 * Math.PI / 3.0); // red to green to blue

    /// <inheritdoc />
    public override string Name => "Z-Analysis";

    /// <inheritdoc />
    public override AnalysisStyle Style => AnalysisStyle.FalseColor;

    /// <inheritdoc />
    public override bool ObjectSupportsAnalysisMode(RhinoObject obj)
    {
      // This function should return true if the analysis mode works
      // on the object.  This example works on meshes and breps, so
      // its version of ObjectSupportsAnalysisMode looks like this.
      switch (obj.ObjectType)
      {
        case ObjectType.Brep:
        case ObjectType.Extrusion:
        case ObjectType.Mesh:
        case ObjectType.Surface:
          return true;
      }
      return false;
    }

    /// <inheritdoc />
    protected override void UpdateVertexColors(RhinoObject obj, Mesh[] meshes)
    {
      // A "mapping tag" is used to determine if the colors need to be set
      var mt = GetMappingTag();

      foreach (var mesh in meshes)
      {
        if (mesh.VertexColors.Tag.Id != Id)
        {
          // The mesh's mapping tag is different from ours. Either the mesh has
          // no false colors, has false colors set by another analysis mode, has
          // false colors set using different m_z_range[]/m_hue_range[] values, or
          // the mesh has been moved.  In any case, we need to set the false
          // colors to the ones we want.
          var colors = new Color[mesh.Vertices.Count];
          for (var i = 0; i < mesh.Vertices.Count; i++)
          {
            double z = mesh.Vertices[i].Z;
            colors[i] = FalseColor(z);
          }
          mesh.VertexColors.SetColors(colors);
          // set the mesh's color tag
          mesh.VertexColors.Tag = mt;
        }
      }
    }

    /// <inheritdoc />
    public override bool ShowIsoCurves => true;

    /// <summary>
    /// Returns a mapping tag that is used to detect when a mesh's colors need to be set.
    /// </summary>
    private MappingTag GetMappingTag()
    {
      var mt = new MappingTag();

      // Since the false colors that are shown will change if
      // the mesh is transformed, we have to initialize the
      // transformation.
      mt.MeshTransform = Transform.Identity;

      // This is the analysis mode id passed to the 
      // CRhinoVisualAnalysisMode constructor. Use the
      // m_am_id member and it this code will alwasy 
      // work correctly.
      mt.Id = Id;

      // This is a 32 bit CRC or the information used to
      // set the false colors.

      // For this example, the m_z_range and m_hue_range 
      // intervals control the colors, so we calculate 
      // their crc.
      uint crc = 0;
      crc = RhinoMath.CRC32(crc, m_z_range.T0);
      crc = RhinoMath.CRC32(crc, m_z_range.T1);
      crc = RhinoMath.CRC32(crc, m_hue_range.T0);
      crc = RhinoMath.CRC32(crc, m_hue_range.T1);
      mt.MappingCRC = crc;

      return mt;
    }

    /// <summary>
    /// Calculate false color.
    /// </summary>
    private Color FalseColor(double z)
    {
      // Simple example of one way to change a number
      // into a color.
      var s = m_z_range.NormalizedParameterAt(z);
      s = RhinoMath.Clamp(s, 0.0, 1.0);
      var hue = m_hue_range.ParameterAt(s);
      return ColorFromHsv(hue, 1.0, 1.0);
    }

    /// <summary>
    /// Returns a color from an hue, saturation, value.
    /// </summary>
    /// <param name="hue">Hue in radians.</param>
    /// <param name="saturation">Satuation, hwere 0.0 = gray, 1.0 = saturated.</param>
    /// <param name="value">The value.</param>
    /// <returns>The color.</returns>
    private static Color ColorFromHsv(double hue, double saturation, double value)
    {
      double r, g, b;
      if (saturation <= 1.0 / 256.0)
      {
        r = value;
        g = value;
        b = value;
      }
      else
      {
        hue *= 3.0 / Math.PI;  // (6.0 / 2.0 * ON_PI);
        var i = (int)Math.Floor(hue);
        if (i < 0 || i > 5)
        {
          hue = hue % 6.0;
          if (hue < 0.0)
            hue += 6.0;
          i = (int)Math.Floor(hue);
        }
        var f = hue - i;
        var p = value * (1.0 - saturation);
        var q = value * (1.0 - (saturation * f));
        var t = value * (1.0 - (saturation * (1.0 - f)));
        switch (i)
        {
          case 0:
            r = value; g = t; b = p; break;
          case 1:
            r = q; g = value; b = p; break;
          case 2:
            r = p; g = value; b = t; break;
          case 3:
            r = p; g = q; b = value; break;
          case 4:
            r = t; g = p; b = value; break;
          case 5:
            r = value; g = p; b = q; break;
          default:
            r = 0; g = 0; b = 0; break;
        }
      }
      return ColorFromFractionalArgb(0.0, r, g, b);
    }

    /// <summary>
    /// Returns a color from fractional argb values.
    /// </summary>
    private static Color ColorFromFractionalArgb(double alpha, double red, double green, double blue)
    {
      int r, g, b, a;
      if (red < 0.0) red = 0.0; else if (red > 1.0) red = 1.0;
      if (green < 0.0) green = 0.0; else if (green > 1.0) green = 1.0;
      if (blue < 0.0) blue = 0.0; else if (blue > 1.0) blue = 1.0;
      if (alpha < 0.0) alpha = 0.0; else if (alpha > 1.0) alpha = 1.0;

      red *= 255.0;
      green *= 255.0;
      blue *= 255.0;
      alpha *= 255.0;

      r = (int)red;
      g = (int)green;
      b = (int)blue;
      a = (int)alpha;

      if (red - r >= 0.5) r++;
      if (green - g >= 0.5) g++;
      if (blue - b >= 0.5) b++;
      if (alpha - a >= 0.5) a++;

      return Color.FromArgb(a, r, g, b);
    }

  }
}