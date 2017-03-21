using System.Windows.Forms;
using Rhino;
using Rhino.UI;

namespace SampleCsWinForms.Forms
{
  internal class SampleCsDocPropertiesPage : OptionsDialogPage
  {
    private SampleCsDocPropertiesUserControl m_control;
    private readonly RhinoDoc m_doc;

    public SampleCsDocPropertiesPage(RhinoDoc doc)
      : base("Sample")
    {
      m_doc = doc;
    }

    public override Control PageControl
    {
      get { return m_control ?? (m_control = new SampleCsDocPropertiesUserControl(m_doc)); }
    }

    public override bool OnApply()
    {
      RhinoApp.WriteLine("SampleCsDocPropertiesPage.OnApply");
      return true;
    }

    public override void OnCancel()
    {
      RhinoApp.WriteLine("SampleCsDocPropertiesPage.OnCancel");
    }
  }
}
