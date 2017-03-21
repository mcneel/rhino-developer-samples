using System.Runtime.InteropServices;
using Rhino;
using Rhino.Commands;

namespace SampleCsRhino
{
  /// <summary>
  /// SampleCsRhinoCommand is a hidden command  that basically does nothing.
  /// This command is called by SampleCsCommand just so we can make sure this
  /// plug-ins is loaded before trying to get it's scripting object.
  /// </summary>
  [Guid("3f8c8ccb-13ab-4050-a259-a53e2fff4700"), CommandStyle(Style.Hidden)]
  public class SampleCsRhinoCommand : Command
  {
    /// <returns>
    /// The command name as it appears on the Rhino command line.
    /// </returns>
    public override string EnglishName
    {
      get { return "SampleCsRhinoCommand"; }
    }

    /// <summary>
    /// Called by Rhino to run the command.
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      // Do nothing...
      return Result.Success;
    }
  }
}
