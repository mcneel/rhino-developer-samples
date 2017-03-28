using System;
using System.Windows.Forms;

namespace SampleCsSkin
{
  public partial class SampleCsSplashScreen : Form
  {
    private delegate void ProgressDelegate(int progress);
    private readonly ProgressDelegate m_delegate;

    public SampleCsSplashScreen()
    {
      InitializeComponent();
      progressBar.Maximum = 100;
      m_delegate = UpdateProgressInternal;
    }

    private void SplashScreen_Load(object sender, EventArgs e)
    {
    }

    private void UpdateProgressInternal(int progress)
    {
      progressBar.Value = progress;
    }

    public void UpdateProgress(int progress)
    {
      Invoke(m_delegate, progress);
    }
  }
}
