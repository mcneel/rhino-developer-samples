using System;
using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  /// <summary>
  /// SampleCsCustomLineObject
  /// </summary>
  public class SampleCsCustomLineObject : Rhino.DocObjects.Custom.CustomCurveObject
  {
    public SampleCsCustomLineObject()
    {
    }

    public SampleCsCustomLineObject(Rhino.Geometry.LineCurve crv)
      : base(crv)
    {
    }

    protected override void OnDraw(Rhino.Display.DrawEventArgs e)
    {
      System.Drawing.Color color = this.Attributes.DrawColor(e.RhinoDoc);
      e.Display.DrawPoint(this.CurveGeometry.PointAtStart, color);
      if (!this.CurveGeometry.IsClosed)
        e.Display.DrawPoint(this.CurveGeometry.PointAtEnd, color);
      base.OnDraw(e);
    }
  }

  /// <summary>
  /// SampleCsCustomLine
  /// </summary>
  public class SampleCsCustomLine : Command
  {
    public override string EnglishName => "SampleCsCustomLine";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Rhino.Geometry.Line line;
      Rhino.Commands.Result rc = Rhino.Input.RhinoGet.GetLine(out line);
      if (rc == Rhino.Commands.Result.Success)
      {
        Rhino.Geometry.LineCurve crv = new Rhino.Geometry.LineCurve(line);
        SampleCsCustomLineObject obj = new SampleCsCustomLineObject();
        doc.Objects.AddRhinoObject(obj, crv);
        doc.Views.Redraw();
      }

      return Rhino.Commands.Result.Success;
    }
  }
}
