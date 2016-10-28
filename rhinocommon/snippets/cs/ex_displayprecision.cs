using Rhino;
using Rhino.Input.Custom;
using Rhino.Commands;

namespace examples_cs
{
  public class DisplayPrecisionCommand : Command
  {
    public override string EnglishName { get { return "csDisplayPrecision"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var gi = new GetInteger();
      gi.SetCommandPrompt("New display precision");
      gi.SetDefaultInteger(doc.ModelDistanceDisplayPrecision);
      gi.SetLowerLimit(0, false);
      gi.SetUpperLimit(7, false);
      gi.Get();
      if (gi.CommandResult() != Result.Success)
        return gi.CommandResult();
      var distance_display_precision = gi.Number();

      if (distance_display_precision != doc.ModelDistanceDisplayPrecision)
        doc.ModelDistanceDisplayPrecision = distance_display_precision;

      return Result.Success;
    }
  }
}