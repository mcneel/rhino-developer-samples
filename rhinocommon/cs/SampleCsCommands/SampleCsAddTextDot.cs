using System.Globalization;
using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsAddTextDot : Command
  {
    public override string EnglishName => "SampleCsAddTextDot";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var count = 0;

      var gp = new GetPoint();
      gp.AcceptNothing(true);
      for (; ; )
      {
        gp.SetCommandPrompt(0 == count ? "Location of text dot" : "Location of text dot. Press Enter when done");

        var res = gp.Get();

        if (res == GetResult.Point)
        {
          var point = gp.Point();
          var str = count.ToString(CultureInfo.InvariantCulture);
          var dot = new TextDot(str, point);
          doc.Objects.Add(dot);
          doc.Views.Redraw();
          count++;
          continue;
        }

        break;
      }

      return Result.Success;
    }
  }
}
