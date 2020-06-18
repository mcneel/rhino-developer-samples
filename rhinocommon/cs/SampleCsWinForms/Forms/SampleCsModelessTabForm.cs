using System.Windows.Forms;

namespace SampleCsWinForms.Forms
{
  public partial class SampleCsModelessTabForm : Form
  {
    private SampleModelessTabFix m_tab_fix;

    public SampleCsModelessTabForm()
    {
      InitializeComponent();
      m_tab_fix = new SampleModelessTabFix(this);
    }
  }
}
