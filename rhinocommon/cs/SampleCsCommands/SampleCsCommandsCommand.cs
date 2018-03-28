using Rhino;
using Rhino.Commands;

namespace SampleCsCommands
{
  public class SampleCsCommandsCommand : Command
  {
    public SampleCsCommandsCommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a refence in a static property.
      Instance = this;
    }

    ///<summary>The only instance of this command.</summary>
    public static SampleCsCommandsCommand Instance
    {
      get;
      private set;
    }

    ///<returns>The command name as it appears on the Rhino command line.</returns>
    public override string EnglishName
    {
      get { return "SampleCsCommandsCommand"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      return Result.Success;
    }
  }
}
