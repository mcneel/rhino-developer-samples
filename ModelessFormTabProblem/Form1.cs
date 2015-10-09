using System.Windows.Forms;
using Rhino.UI;

namespace ModelessFormTabProblem
{
  public partial class Form1 : Form
  {
    public Form1()
    {
      InitializeComponent();
      // Create an instance of the ModelessTabFix class passing this Form to the constructor
      m_TabFix = new ModelessTabFix(this);
    }

    // Class to handle the message hooking and forwarding
    private ModelessTabFix m_TabFix;
  }
}
