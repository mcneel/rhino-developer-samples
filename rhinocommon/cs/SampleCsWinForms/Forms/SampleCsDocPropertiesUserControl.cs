using System.Windows.Forms;
using Rhino;

namespace SampleCsWinForms.Forms
{
  public partial class SampleCsDocPropertiesUserControl : UserControl
  {
    private readonly RhinoDoc m_doc;

    public SampleCsDocPropertiesUserControl(RhinoDoc doc)
    {
      m_doc = doc;
      InitializeComponent();
    }
  }
}
