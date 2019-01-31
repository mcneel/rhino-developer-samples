namespace SampleCsDragDrop
{
  partial class SampleCsUserControl
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
      this.m_listbox = new System.Windows.Forms.ListBox();
      this.SuspendLayout();
      // 
      // m_listbox
      // 
      this.m_listbox.AllowDrop = true;
      this.m_listbox.Dock = System.Windows.Forms.DockStyle.Fill;
      this.m_listbox.FormattingEnabled = true;
      this.m_listbox.Location = new System.Drawing.Point(0, 0);
      this.m_listbox.Name = "m_listbox";
      this.m_listbox.Size = new System.Drawing.Size(296, 326);
      this.m_listbox.TabIndex = 0;
      this.m_listbox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnListBoxMouseDown);
      this.m_listbox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnListBoxMouseMove);
      // 
      // SampleCsUserControl
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.m_listbox);
      this.Name = "SampleCsUserControl";
      this.Size = new System.Drawing.Size(296, 326);
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.ListBox m_listbox;
  }
}
