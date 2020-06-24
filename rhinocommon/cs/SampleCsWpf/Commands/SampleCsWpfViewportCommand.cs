using Rhino;
using Rhino.Commands;
using RhinoWindows;

namespace SampleCsWpf.Commands
{
  /// <summary>
  /// Demonstrates how to host RhinoWindows.Forms.Controls.ViewportControl in a WPF window.
  /// https://docs.microsoft.com/en-us/dotnet/api/system.windows.forms.integration.windowsformshost?view=netcore-3.1
  /// https://docs.microsoft.com/en-us/dotnet/framework/wpf/advanced/walkthrough-hosting-a-windows-forms-control-in-wpf-by-using-xaml
  /// https://github.com/microsoft/WPF-Samples/blob/master/Migration%20and%20Interoperability/HostingWfInWpfWithXaml/MainWindow.xaml
  /// </summary>
  public class SampleCsWpfViewportCommand : Command
  {
    /// <summary>
    /// Gets the command name as it appears on the Rhino command line.
    /// </summary>
    public override string EnglishName => "SampleCsWpfViewport";

    /// <summary>
    /// Called by Rhino when the user wants to run this command.
    /// </summary>
    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var dialog = new Views.SampleCsWpfViewportDialog();
      dialog.ShowSemiModal(RhinoApp.MainWindowHandle());
      return Result.Success;
    }
  }
}
