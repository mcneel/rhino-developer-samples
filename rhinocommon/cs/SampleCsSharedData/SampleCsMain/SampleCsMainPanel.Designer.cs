namespace SampleCsMain
{
  partial class SampleCsMainPanel
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

    #region Component Designer generated code

    /// <summary> 
    /// Required method for Designer support - do not modify 
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.listBox = new System.Windows.Forms.ListBox();
      this.SuspendLayout();
      // 
      // listBox
      // 
      this.listBox.Dock = System.Windows.Forms.DockStyle.Fill;
      this.listBox.FormattingEnabled = true;
      this.listBox.Location = new System.Drawing.Point(0, 0);
      this.listBox.Name = "listBox";
      this.listBox.Size = new System.Drawing.Size(252, 246);
      this.listBox.TabIndex = 0;
      // 
      // SampleCsMainPanel
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.listBox);
      this.Name = "SampleCsMainPanel";
      this.Size = new System.Drawing.Size(252, 246);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.ListBox listBox;
  }
}
