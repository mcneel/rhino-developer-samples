using Eto.Forms;

namespace SampleCsWizardPanel
{
  /// <summary>
  /// Back, Next and Finish horizontal button row, right justified, which
  /// should be placed at the bottom of each wizard page.
  /// </summary>
  class NextBackButtons : Panel
  {
    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="dataContext">
    /// Binds the back button to dataContext.BackCommand, the next button to
    /// dataContext.NextCommand and the finish button to
    /// dataContext.FinishCommand.
    /// </param>
    /// <param name="isLastPage">
    /// If true then the "Next" button will be disabled.
    /// </param>
    public NextBackButtons(SampleCsWizardPanelViewModel dataContext, bool isLastPage)
    {
      Content = new StackLayout
      {
        Padding = 0,
        Spacing = SampleCsWizardEtoPanel.Spacing,
        Orientation = Orientation.Horizontal,
        HorizontalContentAlignment = HorizontalAlignment.Right,
        Items =
        {
          new Button
          {
            Text = Rhino.UI.LOC.STR("< Back"),
            Command = dataContext.BackCommand
          },
          new Button
          {
            Text = Rhino.UI.LOC.STR("Next >"),
            Command = dataContext.NextCommand,
            Enabled = !isLastPage
          },
          new Button
          {
            Text = Rhino.UI.LOC.STR("Finish"),
            Command = dataContext.FinishCommand
          }
        }
      };
    }
  }
}
