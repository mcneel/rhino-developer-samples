using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace SampleCsWinForms.Forms
{
  public partial class SampleCsFibonacciForm : Form
  {
    private int m_number_to_compute;
    private int m_highest_percentage_reached;

    public SampleCsFibonacciForm()
    {
      InitializeComponent();
      InitializeBackgroundWorker();
    }

    private void InitializeBackgroundWorker()
    {
      m_background_worker.DoWork += DoWork;
      m_background_worker.RunWorkerCompleted += RunWorkerCompleted;
      m_background_worker.ProgressChanged += ProgressChanged;
    }

    /// <summary>
    /// OnStartButtonClick
    /// </summary>
    private void OnStartButtonClick(System.Object sender, System.EventArgs e)
    {
      // Reset the text in the result label.
      resultLabel.Text = String.Empty;

      // Disable the UpDown control until the asynchronous operation is done.
      m_spinner.Enabled = false;

      // Disable the Start button until the asynchronous operation is done.
      m_start_button.Enabled = false;

      // Enable the Cancel button while the asynchronous operation runs.
      m_cancel_button.Enabled = true;

      // Get the value from the UpDown control.
      m_number_to_compute = (int)m_spinner.Value;

      // Reset the variable for percentage tracking.
      m_highest_percentage_reached = 0;

      // Start the asynchronous operation.
      m_background_worker.RunWorkerAsync(m_number_to_compute);
    }

    /// <summary>
    /// OnCancelButtonClick
    /// </summary>
    private void OnCancelButtonClick(System.Object sender, System.EventArgs e)
    {
      // Cancel the asynchronous operation.
      this.m_background_worker.CancelAsync();

      // Disable the Cancel button.
      m_cancel_button.Enabled = false;
    }

    /// <summary>
    /// This event handler is where the actual, potentially time-consuming work is done.
    /// </summary>
    private void DoWork(object sender, DoWorkEventArgs e)
    {
      // Get the BackgroundWorker that raised this event.
      var worker = sender as BackgroundWorker;

      // Assign the result of the computation to the Result property of the DoWorkEventArgs
      // object. This is will be available to the RunWorkerCompleted eventhandler.
      e.Result = ComputeFibonacci((int)e.Argument, worker, e);
    }

    /// <summary>
    /// This event handler deals with the results of the background operation.
    /// </summary>
    private void RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
    {
      // First, handle the case where an exception was thrown.
      if (e.Error != null)
      {
        MessageBox.Show(e.Error.Message);
      }
      else if (e.Cancelled)
      {
        // Next, handle the case where the user canceled the operation.
        // Note that due to a race condition in the DoWork event handler, the Cancelled
        // flag may not have been set, even though CancelAsync was called.
        resultLabel.Text = "Canceled";
      }
      else
      {
        // Finally, handle the case where the operation succeeded.
        resultLabel.Text = e.Result.ToString();
      }

      // Enable the UpDown control.
      m_spinner.Enabled = true;

      // Enable the Start button.
      m_start_button.Enabled = true;

      // Disable the Cancel button.
      m_cancel_button.Enabled = false;
    }

    /// <summary>
    /// This event handler updates the progress bar.
    /// </summary>
    private void ProgressChanged(object sender, ProgressChangedEventArgs e)
    {
      m_progress_bar.Value = e.ProgressPercentage;
    }

    /// <summary>
    /// This is the method that does the actual work. For this example, 
    /// it computes a Fibonacci number and reports progress as it does its work.
    /// </summary>
    private long ComputeFibonacci(int n, BackgroundWorker worker, DoWorkEventArgs e)
    {
      // The parameter n must be >= 0 and <= 91.
      // Fib(n), with n > 91, overflows a long.
      if (n < 0 || n > 91)
      {
        throw new ArgumentException("Value must be >= 0 and <= 91", "n");
      }

      long result = 0;

      // Abort the operation if the user has canceled. Note that a call to CancelAsync may have set 
      // CancellationPending to true just after the last invocation of this method exits, so this 
      // code will not have the opportunity to set the DoWorkEventArgs.Cancel flag to true. This means
      // that RunWorkerCompletedEventArgs.Cancelled will not be set to true in your RunWorkerCompleted
      // event handler. This is a race condition.

      if (worker.CancellationPending)
      {
        e.Cancel = true;
      }
      else
      {
        if (n < 2)
          result = 1;
        else
          result = ComputeFibonacci(n - 1, worker, e) + ComputeFibonacci(n - 2, worker, e);

        // Report progress as a percentage of the total task.
        var percent_complete = (int)(n / (float)m_number_to_compute * 100);
        if (percent_complete > m_highest_percentage_reached)
        {
          m_highest_percentage_reached = percent_complete;
          worker.ReportProgress(percent_complete);
        }
      }

      return result;
    }
  }
}
