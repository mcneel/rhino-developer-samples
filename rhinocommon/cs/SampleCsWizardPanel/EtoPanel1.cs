using Eto.Forms;

namespace SampleCsWizardPanel
{
  internal class EtoPanel1 : SampleCsWizardEtoPanel
  {
    public EtoPanel1(SampleCsWizardPanelViewModel dataContext) : base(dataContext)
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
          new Label { Text="Panel One" },
          null,
          new TableRow(null, new NextBackButtons(ViewModel, false))
        }
      };
    }
  }
}
