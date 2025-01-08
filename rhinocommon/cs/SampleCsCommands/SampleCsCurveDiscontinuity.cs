using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using System;
using System.Collections.Generic;
using System.Linq;

namespace SampleCsCommands
{
  public class SampleCsCurveDiscontinuity : Command
  {
    public override string EnglishName => "SampleCsCurveDiscontinuity";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select curves");
      go.GeometryFilter = ObjectType.Curve;
      go.Get();
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      Curve curve = go.Object(0).Curve();
      if (null == curve)
        return Result.Failure;

      Continuity continuity = Continuity.G1_continuous;

      GetOption gd = new GetOption();
      gd.SetCommandPrompt("Discontinuity to search");
      gd.AddOptionEnumList("Discontinuity", continuity);
      gd.AcceptNothing(true);
      GetResult res = gd.Get();
      if (res == GetResult.Option)
      {
        CommandLineOption option = gd.Option();
        if (null == option)
          return Result.Failure;

        List<Continuity> list = Enum.GetValues(typeof(Continuity)).Cast<Continuity>().ToList();
        continuity = list[option.CurrentListOptionIndex];
      }
      else if (res != GetResult.Nothing)
        return Result.Cancel;

      double t0 = curve.Domain.Min;
      double t1 = curve.Domain.Max;

      List<double> parameters = new List<double>();
      parameters.Add(t0);
      for (; ; )
      {
        double t;
        bool rc = curve.GetNextDiscontinuity(continuity, t0, t1, out t);
        if (rc)
        {
          parameters.Add(t);
          t0 = t;
        }
        else
          break;
      }
      parameters.Add(t1);

      if (parameters.Count > 2)
      {
        for (int i = 0; i < parameters.Count - 1; i++)
        {
          t0 = parameters[i];
          t1 = parameters[i + 1];
          Interval dom = new Interval(t0, t1);
          Curve new_curve = curve.Trim(dom);
          if (null != new_curve)
            doc.Objects.AddCurve(new_curve);
        }

        doc.Objects.Delete(go.Object(0), false);
      }

      return Result.Success;
    }
  }
}
