using Rhino.PlugIns;
using Rhino.UI;

namespace SampleCsDragDrop
{
  public class SampleCsDragDropPlugIn : PlugIn
  {
    internal SampleCsDropTarget DropTarget { get; private set; }

    public SampleCsDragDropPlugIn()
    {
      Instance = this;
    }

    public static SampleCsDragDropPlugIn Instance { get; private set; }

    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      DropTarget = new SampleCsDropTarget();

      Panels.RegisterPanel(this, typeof(SampleCsUserControl), "SampleCsDragDrop", null);

      if (Settings.GetBool("DisplayPanelByDefault", true))
      {
        Settings.SetBool("DisplayPanelByDefault", false);
        Panels.OpenPanel(SampleCsUserControl.PanelId);
      }

      return base.OnLoad(ref errorMessage);
    }
  }
}