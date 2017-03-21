namespace SampleCsWinForms.Forms
{
  partial class SampleCsFibonacciForm
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.m_progress_bar = new System.Windows.Forms.ProgressBar();
      this.resultLabel = new System.Windows.Forms.Label();
      this.m_cancel_button = new System.Windows.Forms.Button();
      this.m_start_button = new System.Windows.Forms.Button();
      this.m_background_worker = new System.ComponentModel.BackgroundWorker();
      this.m_spinner = new System.Windows.Forms.NumericUpDown();
      ((System.ComponentModel.ISupportInitialize)(this.m_spinner)).BeginInit();
      this.SuspendLayout();
      // 
      // progressBar1
      // 
      this.m_progress_bar.Location = new System.Drawing.Point(12, 47);
      this.m_progress_bar.Name = "m_progress_bar";
      this.m_progress_bar.Size = new System.Drawing.Size(268, 8);
      this.m_progress_bar.Step = 2;
      this.m_progress_bar.TabIndex = 9;
      // 
      // resultLabel
      // 
      this.resultLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
      this.resultLabel.Location = new System.Drawing.Point(98, 12);
      this.resultLabel.Name = "resultLabel";
      this.resultLabel.Size = new System.Drawing.Size(182, 23);
      this.resultLabel.TabIndex = 8;
      this.resultLabel.Text = "(no result)";
      this.resultLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      // 
      // cancelAsyncButton
      // 
      this.m_cancel_button.Enabled = false;
      this.m_cancel_button.Location = new System.Drawing.Point(161, 61);
      this.m_cancel_button.Name = "m_cancel_button";
      this.m_cancel_button.Size = new System.Drawing.Size(119, 23);
      this.m_cancel_button.TabIndex = 7;
      this.m_cancel_button.Text = "Cancel";
      this.m_cancel_button.Click += new System.EventHandler(this.OnCancelButtonClick);
      // 
      // startAsyncButton
      // 
      this.m_start_button.Location = new System.Drawing.Point(12, 61);
      this.m_start_button.Name = "m_start_button";
      this.m_start_button.Size = new System.Drawing.Size(120, 23);
      this.m_start_button.TabIndex = 6;
      this.m_start_button.Text = "Start";
      this.m_start_button.Click += new System.EventHandler(this.OnStartButtonClick);
      // 
      // backgroundWorker1
      // 
      this.m_background_worker.WorkerReportsProgress = true;
      this.m_background_worker.WorkerSupportsCancellation = true;
      // 
      // numericUpDown1
      // 
      this.m_spinner.Location = new System.Drawing.Point(12, 12);
      this.m_spinner.Maximum = new decimal(new int[] {
            91,
            0,
            0,
            0});
      this.m_spinner.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
      this.m_spinner.Name = "m_spinner";
      this.m_spinner.Size = new System.Drawing.Size(80, 20);
      this.m_spinner.TabIndex = 5;
      this.m_spinner.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
      // 
      // SampleCsFibonacciForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(292, 118);
      this.Controls.Add(this.m_progress_bar);
      this.Controls.Add(this.resultLabel);
      this.Controls.Add(this.m_cancel_button);
      this.Controls.Add(this.m_start_button);
      this.Controls.Add(this.m_spinner);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "SampleCsFibonacciForm";
      this.ShowIcon = false;
      this.ShowInTaskbar = false;
      this.Text = "Fibonacci Calculator";
      ((System.ComponentModel.ISupportInitialize)(this.m_spinner)).EndInit();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.ProgressBar m_progress_bar;
    private System.Windows.Forms.Label resultLabel;
    private System.Windows.Forms.Button m_cancel_button;
    private System.Windows.Forms.Button m_start_button;
    private System.ComponentModel.BackgroundWorker m_background_worker;
    private System.Windows.Forms.NumericUpDown m_spinner;
  }
}