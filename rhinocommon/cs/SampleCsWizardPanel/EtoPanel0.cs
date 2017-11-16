using Eto.Forms;

namespace SampleCsWizardPanel
{
  internal class EtoPanel0 : SampleCsWizardEtoPanel
  {
    public EtoPanel0(SampleCsWizardPanelViewModel dataContext) : base(dataContext)
    {
      InitializeComponent();
    }

    /// <summary>
    /// Create Panel.Content
    /// </summary>
    private void InitializeComponent()
    {
      var textbox = new TextBox();
      Content = new TableLayout
      {
        Spacing = SpacingSize,
        Rows =
        {
          new TableRow(textbox) { ScaleHeight = true },
          new StackLayout
          {
            Padding = 0,
            HorizontalContentAlignment = HorizontalAlignment.Stretch,
            Spacing = Spacing,
            Items =
            {
              new Button {Text = Rhino.UI.LOC.STR("Next >"), Command = ViewModel.NextCommand },
            }
          }
        }
      };
    }
  }
}
