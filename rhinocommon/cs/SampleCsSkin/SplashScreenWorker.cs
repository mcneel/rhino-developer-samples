using System;
using System.Threading;

namespace SampleCsSkin
{
  public class SplashScreenWorker
  {
    public event EventHandler<SplashScreenWorkerArgs> ProgressChanged;
    public event EventHandler HardWorkDone;

    public void DoHardWork()
    {
      // Do something time consuming...
      for (var i = 1; i <= 100; i++)
      {
        for (var j = 1; j <= 100000; j++)
        {
          Thread.Sleep(0);
        }
        OnProgressChanged(i);
      }

      OnWorkerDone();
    }

    private void OnProgressChanged(int progress)
    {
      var handler = ProgressChanged;
      if (handler != null)
      {
        handler(this, new SplashScreenWorkerArgs(progress));
      }
    }

    private void OnWorkerDone()
    {
      var handler = HardWorkDone;
      if (handler != null)
      {
        handler(this, EventArgs.Empty);
      }
    }
  }

  public class SplashScreenWorkerArgs : EventArgs
  {
    public SplashScreenWorkerArgs(int progress)
    {
      Progress = progress;
    }

    public int Progress
    {
      get;
      private set;
    }
  }
}