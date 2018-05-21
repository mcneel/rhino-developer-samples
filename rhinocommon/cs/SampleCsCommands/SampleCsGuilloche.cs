using System;
using Rhino;
using Rhino.Commands;
using Rhino.Display;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  /// <summary>
  /// Guilloche pattern generator
  /// </summary>
  public class Guilloche
  {
    public static int MinSteps = 10;
    public static int MaxSteps = 3000;

    public static double MinMajorRipple = 0.0;
    public static double MaxMajorRipple = 100.0;

    public static double MinMinorRipple = 0.0;
    public static double MaxMinorRipple = 0.5;

    public static double MinRadius = -100.0;
    public static double MaxRadius = 200.0;

    public static double MinMultiplier = -0.1;
    public static double MaxMultiplier = 100.0;

    public static double MinAmplitude = 1.0;
    public static double MaxAmplitude = 20.0;

    public enum OutputType
    {
      Nurbs,
      Polyline
    }

    private int m_steps;            // Divides a circle this many times
    private double m_major_ripple;  // The major ripple
    private double m_minor_ripple;  // The minor ripple
    private double m_radius;        // Radius type effect
    private double m_multiplier;    // Angle multiplier
    private double m_amplitude;     // Scale of output

    /// <summary>
    /// Public constructor
    /// </summary>
    public Guilloche()
    {
      Defaults();
    }

    /// <summary>
    /// Sets factory defaults
    /// </summary>
    public void Defaults()
    {
      m_steps = 391;
      m_major_ripple = 21;
      m_minor_ripple = 0.28;
      m_radius = 47;
      m_multiplier = 1.7;
      m_amplitude = 3.5;
      Output = OutputType.Nurbs;
    }

    /// <summary>
    /// Divides a circle this many times
    /// </summary>
    public int Steps
    {
      get => m_steps;
      set => m_steps = RhinoMath.Clamp(value, MinSteps, MaxSteps);
    }

    /// <summary>
    /// The major ripple
    /// </summary>
    public double MajorRipple
    {
      get => m_major_ripple;
      set => m_major_ripple = RhinoMath.Clamp(value, MinMajorRipple, MaxMajorRipple);
    }

    /// <summary>
    /// The minor ripple
    /// </summary>
    public double MinorRipple
    {
      get => m_minor_ripple;
      set => m_minor_ripple = RhinoMath.Clamp(value, MinMinorRipple, MaxMinorRipple);
    }

    /// <summary>
    /// Radius type effect
    /// </summary>
    public double Radius
    {
      get => m_radius;
      set => m_radius = RhinoMath.Clamp(value, MinRadius, MaxRadius);
    }

    /// <summary>
    /// Angle multiplier
    /// </summary>
    public double Multiplier
    {
      get => m_multiplier;
      set => m_multiplier = RhinoMath.Clamp(value, MinMultiplier, MaxMultiplier);
    }

    /// <summary>
    /// Scale of output
    /// </summary>
    public double Amplitude
    {
      get => m_amplitude;
      set => m_amplitude = RhinoMath.Clamp(value, MinAmplitude, MaxAmplitude);
    }

    /// <summary>
    /// Output curve type
    /// </summary>
    public OutputType Output { get; set; }

    public Curve Calculate()
    {
      var points = new Point3d[m_steps + 1];
      var theta = 0.0;
      var theta_step = 2.0 * Math.PI / m_steps;

      var s = (m_major_ripple + m_minor_ripple) / m_minor_ripple;
      var rr = m_minor_ripple + m_major_ripple;
      var rp = m_minor_ripple + m_radius;

      for (var t = 0; t < m_steps + 1; t++)
      {
        var x = rr * Math.Cos(m_multiplier * theta) + rp * Math.Cos(s * m_multiplier * theta);
        var y = rr * Math.Sin(m_multiplier * theta) + rp * Math.Sin(s * m_multiplier * theta);
        x *= m_amplitude;
        y *= m_amplitude;

        points[t] = new Point3d(x, y, 0.0);

        theta += theta_step;
      }

      return Output == OutputType.Nurbs
        ? Curve.CreateInterpolatedCurve(points, 3)
        : new PolylineCurve(points);
    }
  }


  /// <summary>
  /// DisplayConduit class
  /// </summary>
  public class GuillocheConduit : DisplayConduit
  {
    private Curve m_curve = null;

    public GuillocheConduit()
    {
      Args = new Guilloche();
    }

    public Guilloche Args { get; private set; }

    public void UpdateCurve()
    {
      m_curve = null;
      var curve = Args.Calculate();
      if (null != curve && curve.IsValid)
        m_curve = curve;
    }

    protected override void CalculateBoundingBox(CalculateBoundingBoxEventArgs e)
    {
      if (null != m_curve && m_curve.IsValid)
      {
        e.IncludeBoundingBox(m_curve.GetBoundingBox(false));
      }
    }

    protected override void DrawOverlay(DrawEventArgs e)
    {
      if (null != m_curve && m_curve.IsValid)
      {
        var color = Rhino.ApplicationSettings.AppearanceSettings.DefaultLayerColor;
        e.Display.DrawCurve(m_curve, color);
      }
    }
  }


  /// <summary>
  /// Command
  /// </summary>
  public class SampleCsGuilloche : Command
  {
    public override string EnglishName => "SampleCsGuilloche";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var conduit = new GuillocheConduit { Enabled = true };
      doc.Views.Redraw();

      var opt_amplitude = new OptionDouble(conduit.Args.Amplitude, Guilloche.MinAmplitude, Guilloche.MaxAmplitude);
      var opt_major_ripple = new OptionDouble(conduit.Args.MajorRipple, Guilloche.MinMajorRipple, Guilloche.MinMinorRipple);
      var opt_minor_ripple = new OptionDouble(conduit.Args.MinorRipple, Guilloche.MinMinorRipple, Guilloche.MaxMinorRipple);
      var opt_multiplier = new OptionDouble(conduit.Args.Multiplier, Guilloche.MinMultiplier, Guilloche.MaxMultiplier);
      var opt_radius = new OptionDouble(conduit.Args.Radius, Guilloche.MinRadius, Guilloche.MaxRadius);
      var opt_steps = new OptionInteger(conduit.Args.Steps, Guilloche.MinSteps, Guilloche.MaxSteps);

      var go = new GetOption();
      go.SetCommandPrompt("Guilloché pattern options");
      go.AcceptNothing(true);
      for (; ; )
      {
        go.ClearCommandOptions();

        var idx_amplitude = go.AddOptionDouble("Amplitude", ref opt_amplitude);
        var idx_major_ripple = go.AddOptionDouble("MajorRipple", ref opt_major_ripple);
        var idx_minor_ripple = go.AddOptionDouble("MinorRipple", ref opt_minor_ripple);
        var idx_multiplier = go.AddOptionDouble("Multiplier", ref opt_multiplier);
        var idx_radius = go.AddOptionDouble("Radius", ref opt_radius);
        var idx_steps = go.AddOptionInteger("Steps", ref opt_steps);

        var res = go.Get();

        if (res == GetResult.Option)
        {
          var option = go.Option();
          if (null != option)
          {
            if (idx_amplitude == option.Index)
              conduit.Args.Amplitude = opt_amplitude.CurrentValue;
            else if (idx_major_ripple == option.Index)
              conduit.Args.MajorRipple = opt_major_ripple.CurrentValue;
            else if (idx_minor_ripple == option.Index)
              conduit.Args.MinorRipple = opt_minor_ripple.CurrentValue;
            else if (idx_multiplier == option.Index)
              conduit.Args.Multiplier = opt_multiplier.CurrentValue;
            else if (idx_radius == option.Index)
              conduit.Args.Radius = opt_radius.CurrentValue;
            else if (idx_steps == option.Index)
              conduit.Args.Steps = opt_steps.CurrentValue;

            conduit.UpdateCurve();
            doc.Views.Redraw();
          }

          continue;
        }

        if (res != GetResult.Nothing)
          return Result.Cancel;

        break;
      }

      var curve = conduit.Args.Calculate();
      if (null != curve && curve.IsValid)
      {
        doc.Objects.AddCurve(curve);
        doc.Views.Redraw();
      }


      return Result.Success;
    }
  }
}