using System.Windows.Forms;
using Rhino;
using Rhino.UI;

namespace SampleCsWinForms.Forms
{
  internal class SampleCsOptionsPage : OptionsDialogPage
  {
    private SampleCsOptionsUserControl m_control;

    public SampleCsOptionsPage()
      : base("Sample")
    {
    }

    public override Control PageControl
    {
      get { return m_control ?? (m_control = new SampleCsOptionsUserControl()); }
    }

    public override bool OnApply()
    {
      RhinoApp.WriteLine("SampleCsOptionsPage.OnApply");
      return true;
    }

    public override void OnCancel()
    {
      RhinoApp.WriteLine("SampleCsOptionsPage.OnCancel");
    }
  }
}
