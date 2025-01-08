using Rhino;
using Rhino.Commands;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;
using System.Globalization;

namespace SampleCsCommands
{
  public class SampleCsAddTextDot : Command
  {
    public override string EnglishName => "SampleCsAddTextDot";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      int count = 0;

      GetPoint gp = new GetPoint();
      gp.AcceptNothing(true);
      for (; ; )
      {
        gp.SetCommandPrompt(0 == count ? "Location of text dot" : "Location of text dot. Press Enter when done");

        GetResult res = gp.Get();

        if (res == GetResult.Point)
        {
          Point3d point = gp.Point();
          string str = count.ToString(CultureInfo.InvariantCulture);
          TextDot dot = new TextDot(str, point);
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
