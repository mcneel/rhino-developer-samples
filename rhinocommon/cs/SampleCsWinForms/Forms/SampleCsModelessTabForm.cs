using System.Windows.Forms;

namespace SampleCsWinForms.Forms
{
  public partial class SampleCsModelessTabForm : Form
  {
    public SampleCsModelessTabForm()
    {
      InitializeComponent();

      // Create an instance of the ModelessTabFix class passing this Form to the constructor
      m_tab_fix = new ModelessTabFix(this);
    }

    // Class to handle the message hooking and forwarding
    private ModelessTabFix m_tab_fix;
  }
}
