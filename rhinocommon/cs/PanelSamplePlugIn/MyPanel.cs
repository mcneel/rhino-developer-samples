using System;
using System.Windows.Forms;

namespace PanelSamplePlugIn
{
  //TODO: Create your own Guid for this class
  [System.Runtime.InteropServices.Guid("320EDE95-B56E-427A-8C9E-670A3EF874AD")]
  public partial class MyPanel : UserControl
  {
    public MyPanel()
    {
      InitializeComponent();
    }

    private void button1_Click(object sender, EventArgs e)
    {
      MessageBox.Show(this, "Hello World!");
    }
  }
}
