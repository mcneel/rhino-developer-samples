using Eto.Drawing;
using Eto.Forms;

namespace SampleCsWizardPanel
{
  /// <summary>
  /// Abstract class used by the SampleCsWizardPanelViewModel as MainPanel.Content
  /// </summary>
  internal abstract class SampleCsWizardEtoPanel : Panel
  {
    protected SampleCsWizardEtoPanel(SampleCsWizardPanelViewModel dataContext)
    {
      ViewModel = dataContext;
    }

    /// <summary>
    /// The view model associated with a specific Rhino document used by
    /// a instance of a MainPanel for the life of the document.  The MainPanel
    /// and view model will get disposed when the document closes.
    /// </summary>
    protected SampleCsWizardPanelViewModel ViewModel { get; }
    /// <summary>
    /// Standard spacing used by all wizard pages
    /// </summary>
    public static int Spacing => 4;
    /// <summary>
    /// Standard spacing used by all wizard pages
    /// </summary>
    public static Size SpacingSize = new Size(Spacing, Spacing);
  }
}
