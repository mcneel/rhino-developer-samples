using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.DocObjects.Tables;
using Rhino.Geometry;

namespace SampleCsCommands
{
  /// <summary>
  /// HatchPatternTable extension methods
  /// </summary>
  public static class HatchPatternTableExtensions
  {
    /// <summary>
    /// Returns the Solid hatch pattern
    /// </summary>
    public static HatchPattern SolidPattern(this HatchPatternTable table)
    {
      return FindOrAddHatchPattern(table, HatchPattern.Defaults.Solid);
    }

    /// <summary>
    /// Returns the Hatch1 hatch pattern
    /// </summary>
    public static HatchPattern Hatch1Pattern(this HatchPatternTable table)
    {
      return FindOrAddHatchPattern(table, HatchPattern.Defaults.Hatch1);
    }

    /// <summary>
    /// Returns the Hatch2 hatch pattern
    /// </summary>
    public static HatchPattern Hatch2Pattern(this HatchPatternTable table)
    {
      return FindOrAddHatchPattern(table, HatchPattern.Defaults.Hatch2);
    }

    /// <summary>
    /// Returns the Hatch3 hatch pattern
    /// </summary>
    public static HatchPattern Hatch3Pattern(this HatchPatternTable table)
    {
      return FindOrAddHatchPattern(table, HatchPattern.Defaults.Hatch3);
    }

    /// <summary>
    /// Returns the Dash hatch pattern
    /// </summary>
    public static HatchPattern DashPattern(this HatchPatternTable table)
    {
      return FindOrAddHatchPattern(table, HatchPattern.Defaults.Dash);
    }

    /// <summary>
    /// Returns the Grid hatch pattern
    /// </summary>
    public static HatchPattern GridPattern(this HatchPatternTable table)
    {
      return FindOrAddHatchPattern(table, HatchPattern.Defaults.Grid);
    }

    /// <summary>
    /// Returns the Grid60 hatch pattern
    /// </summary>
    public static HatchPattern Grid60Pattern(this HatchPatternTable table)
    {
      return FindOrAddHatchPattern(table, HatchPattern.Defaults.Grid60);
    }

    private static HatchPattern FindOrAddHatchPattern(HatchPatternTable table, HatchPattern pattern)
    {
      HatchPattern rc = null;
      if (null != pattern)
      {
        rc = table.FindName(pattern.Name);
        var index = rc?.Index ?? table.Add(pattern);
        rc = table[index];
      }
      return rc;
    }
  }

  /// <summary>
  /// SampleCsHatch command
  /// </summary>
  public class SampleCsHatch : Command
  {
    public override string EnglishName => "SampleCsHatch";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var circle = new Circle(Plane.WorldXY, 5.0);
      var curve = circle.ToNurbsCurve();

      var pattern = doc.HatchPatterns.DashPattern();

      var hatch = Hatch.Create(curve, pattern.Index, 0.0, 1.0, doc.ModelAbsoluteTolerance);
      doc.Objects.AddHatch(hatch[0]);
      doc.Views.Redraw();

      return Result.Success;
    }
  }
}