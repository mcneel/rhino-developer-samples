namespace SampleCsWinForms.Forms
{
  partial class SampleCsModalForm
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
      this.HelloBtn = new System.Windows.Forms.Button();
      this.OkBtn = new System.Windows.Forms.Button();
      this.CancelBtn = new System.Windows.Forms.Button();
      this.PickBtn = new System.Windows.Forms.Button();
      this.SuspendLayout();
      // 
      // HelloBtn
      // 
      this.HelloBtn.Location = new System.Drawing.Point(42, 30);
      this.HelloBtn.Name = "HelloBtn";
      this.HelloBtn.Size = new System.Drawing.Size(75, 23);
      this.HelloBtn.TabIndex = 0;
      this.HelloBtn.Text = "Hello!";
      this.HelloBtn.UseVisualStyleBackColor = true;
      this.HelloBtn.Click += new System.EventHandler(this.HelloButton_Click);
      // 
      // OkBtn
      // 
      this.OkBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.OkBtn.Location = new System.Drawing.Point(42, 79);
      this.OkBtn.Name = "OkBtn";
      this.OkBtn.Size = new System.Drawing.Size(75, 23);
      this.OkBtn.TabIndex = 1;
      this.OkBtn.Text = "OK";
      this.OkBtn.UseVisualStyleBackColor = true;
      // 
      // CancelBtn
      // 
      this.CancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.CancelBtn.Location = new System.Drawing.Point(159, 79);
      this.CancelBtn.Name = "CancelBtn";
      this.CancelBtn.Size = new System.Drawing.Size(75, 23);
      this.CancelBtn.TabIndex = 2;
      this.CancelBtn.Text = "Cancel";
      this.CancelBtn.UseVisualStyleBackColor = true;
      // 
      // PickBtn
      // 
      this.PickBtn.Location = new System.Drawing.Point(159, 30);
      this.PickBtn.Name = "PickBtn";
      this.PickBtn.Size = new System.Drawing.Size(75, 23);
      this.PickBtn.TabIndex = 3;
      this.PickBtn.Text = "Pick";
      this.PickBtn.UseVisualStyleBackColor = true;
      this.PickBtn.Click += new System.EventHandler(this.PickButton_Click);
      // 
      // SampleCsModalWinForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.CancelButton = this.CancelBtn;
      this.ClientSize = new System.Drawing.Size(284, 130);
      this.Controls.Add(this.PickBtn);
      this.Controls.Add(this.CancelBtn);
      this.Controls.Add(this.OkBtn);
      this.Controls.Add(this.HelloBtn);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "SampleCsModalWinForm";
      this.ShowIcon = false;
      this.ShowInTaskbar = false;
      this.Text = "SampleCsModalWinForm";
      this.Load += new System.EventHandler(this.SampleCsForm_Load);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.Button HelloBtn;
    private System.Windows.Forms.Button OkBtn;
    private System.Windows.Forms.Button CancelBtn;
    private System.Windows.Forms.Button PickBtn;
  }
}