using Eto.Forms;

namespace SampleCsWizardPanel
{
  internal class EtoPanel2 : SampleCsWizardEtoPanel
  {
    public EtoPanel2(SampleCsWizardPanelViewModel dataContext) : base(dataContext)
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
          new Label { Text="Panel Two" },
          null,
          new TableRow(null, new NextBackButtons(ViewModel, false))
        }
      };
    }
  }
}
