using Rhino;
using Rhino.Commands;
using Rhino.UI;

namespace SampleCsWizardPanel
{
  public class SampleCsWizardPanelCommand : Rhino.Commands.Command
  {
    public SampleCsWizardPanelCommand()
    {
      // Rhino only creates one instance of each command class defined in a
      // plug-in, so it is safe to store a reference in a static property.
      Instance = this;

      Panels.RegisterPanel(PlugIn, typeof(MainPanel), LOC.STR("SampleCsWizardPanel"), null);
    }

    /// <summary>
    /// The only instance of this command.
    /// </summary>
    public static SampleCsWizardPanelCommand Instance
    {
      get; private set;
    }

    /// <returns>
    /// The command name as it appears on the Rhino command line.
    /// </returns>
    public override string EnglishName => "SampleCsWizardPanel";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      Panels.OpenPanel(typeof(MainPanel).GUID);
      return Rhino.Commands.Result.Success;
    }
  }
}
