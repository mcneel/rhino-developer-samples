using Eto.Forms;

namespace SampleCsWizardPanel
{
  internal class EtoPanel3 : SampleCsWizardEtoPanel
  {
    public EtoPanel3(SampleCsWizardPanelViewModel dataContext) : base(dataContext)
    {
      DataContext = dataContext;
      InitializeComponent();
    }

    private void InitializeComponent()
    {
      Content = new TableLayout
      {
        Padding = 0,
        Spacing = SpacingSize,
        Rows =
        {
          null,
          new Label { Text="Last Panel" },
          null,
          new TableRow(null, new NextBackButtons(ViewModel, true))
        }
      };
    }
  }
}
