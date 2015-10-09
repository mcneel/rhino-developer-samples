using Rhino;
using Rhino.Commands;
using Rhino.Display;
using System.Linq;
using Rhino.Input.Custom;
using Rhino.DocObjects;

namespace examples_cs
{
  public class SingleColorBackfacesCommand : Command
  {
    public override string EnglishName { get { return "csSingleColorBackfaces"; } }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var display_mode_descs = //DisplayModeDescription.GetDisplayModes();
        from dm in DisplayModeDescription.GetDisplayModes()
        where dm.EnglishName == "Shaded"
        select dm;

      foreach (var dmd in display_mode_descs)
      {
        RhinoApp.WriteLine("CurveColor {0}", dmd.DisplayAttributes.CurveColor.ToKnownColor());
        RhinoApp.WriteLine("ObjectColor {0}", dmd.DisplayAttributes.ObjectColor.ToKnownColor());
      }
      return Result.Success;
    }
  }
}