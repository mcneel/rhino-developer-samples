namespace SampleCsWinForms.Forms
{
  partial class SampleCsModelessTabForm
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
      this.textBox1 = new System.Windows.Forms.TextBox();
      this.textBox2 = new System.Windows.Forms.TextBox();
      this.textBox3 = new System.Windows.Forms.TextBox();
      this.textBox4 = new System.Windows.Forms.TextBox();
      this.textBox5 = new System.Windows.Forms.TextBox();
      this.textBox6 = new System.Windows.Forms.TextBox();
      this.SuspendLayout();
      // 
      // textBox1
      // 
      this.textBox1.Location = new System.Drawing.Point(12, 12);
      this.textBox1.Name = "textBox1";
      this.textBox1.Size = new System.Drawing.Size(260, 20);
      this.textBox1.TabIndex = 0;
      // 
      // textBox2
      // 
      this.textBox2.Location = new System.Drawing.Point(12, 38);
      this.textBox2.Name = "textBox2";
      this.textBox2.Size = new System.Drawing.Size(260, 20);
      this.textBox2.TabIndex = 1;
      // 
      // textBox3
      // 
      this.textBox3.Location = new System.Drawing.Point(12, 64);
      this.textBox3.Name = "textBox3";
      this.textBox3.Size = new System.Drawing.Size(260, 20);
      this.textBox3.TabIndex = 2;
      // 
      // textBox4
      // 
      this.textBox4.Location = new System.Drawing.Point(12, 90);
      this.textBox4.Name = "textBox4";
      this.textBox4.Size = new System.Drawing.Size(260, 20);
      this.textBox4.TabIndex = 3;
      // 
      // textBox5
      // 
      this.textBox5.Location = new System.Drawing.Point(12, 116);
      this.textBox5.Name = "textBox5";
      this.textBox5.Size = new System.Drawing.Size(260, 20);
      this.textBox5.TabIndex = 4;
      // 
      // textBox6
      // 
      this.textBox6.Location = new System.Drawing.Point(12, 142);
      this.textBox6.Name = "textBox6";
      this.textBox6.Size = new System.Drawing.Size(260, 20);
      this.textBox6.TabIndex = 5;
      // 
      // ModelessForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(284, 262);
      this.Controls.Add(this.textBox6);
      this.Controls.Add(this.textBox5);
      this.Controls.Add(this.textBox4);
      this.Controls.Add(this.textBox3);
      this.Controls.Add(this.textBox2);
      this.Controls.Add(this.textBox1);
      this.Name = "ModelessForm";
      this.Text = "ModelessForm";
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TextBox textBox1;
    private System.Windows.Forms.TextBox textBox2;
    private System.Windows.Forms.TextBox textBox3;
    private System.Windows.Forms.TextBox textBox4;
    private System.Windows.Forms.TextBox textBox5;
    private System.Windows.Forms.TextBox textBox6;
  }
}