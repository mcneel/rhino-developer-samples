using System.Drawing;
using System.Threading;
using System.Windows.Forms;

namespace SampleCsSkin
{
  /// <summary>
  /// The Rhino Skin object
  /// http://developer.rhino3d.com/guides/rhinocommon/creating-a-skin/
  /// </summary>
  public class SampleCsSkinObject : Rhino.Runtime.Skin
  {
    private SampleCsSplashScreen m_splash;
    private bool m_done;

    protected override string ApplicationName
    {
      get
      {
        return "SampleCsSkin";
      }
    }

    protected override Bitmap MainRhinoIcon
    {
      get
      {
        var icon = new Icon(Properties.Resources.SampleCsSkin, new Size(32, 32));
        return icon.ToBitmap();
      }
    }

    protected override void ShowSplash()
    {
      m_splash = new SampleCsSplashScreen();

      var thread = new Thread(ShowSplashScreen);
      thread.Start();

      var worker = new SampleCsSplashScreenWorker();
      worker.ProgressChanged += (o, ex) =>
      {
        if (null != m_splash) 
          m_splash.UpdateProgress(ex.Progress);
      };

      worker.HardWorkDone += (o, ex) =>
      {
        m_done = true;
      };

      worker.DoHardWork();
    }

    private void ShowSplashScreen()
    {
      m_splash.Show();
      
      while (!m_done)
      {
        Application.DoEvents();
      }

      m_splash.Close();
      m_splash.Dispose();
      m_splash = null;
      m_done = false;
    }
  }
}
