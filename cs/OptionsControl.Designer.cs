namespace examples_cs
{
  partial class OptionsControl
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
      this.checkBox1 = new System.Windows.Forms.CheckBox();
      this.label1 = new System.Windows.Forms.Label();
      this.SuspendLayout();
      // 
      // checkBox1
      // 
      this.checkBox1.AutoSize = true;
      this.checkBox1.Location = new System.Drawing.Point(28, 44);
      this.checkBox1.Name = "checkBox1";
      this.checkBox1.Size = new System.Drawing.Size(80, 17);
      this.checkBox1.TabIndex = 1;
      this.checkBox1.Text = "checkBox1";
      this.checkBox1.UseVisualStyleBackColor = true;
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(4, 4);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(158, 13);
      this.label1.TabIndex = 2;
      this.label1.Text = "RhinoCommon Example Options";
      // 
      // OptionsControl
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.label1);
      this.Controls.Add(this.checkBox1);
      this.Name = "OptionsControl";
      this.Size = new System.Drawing.Size(344, 385);
      this.Load += new System.EventHandler(this.OptionsControl_Load);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.CheckBox checkBox1;
    private System.Windows.Forms.Label label1;
  }
}
