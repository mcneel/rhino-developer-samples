using System.Windows.Forms;

namespace rdktest_cs
{
  partial class CustomMaterialUserInterfaceSection
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
      this.labelColor = new System.Windows.Forms.Label();
      this.buttonColor = new System.Windows.Forms.Button();
      this.checkBox1 = new System.Windows.Forms.CheckBox();
      this.subNodeControl = new RhinoWindows.Forms.Controls.Render.SubNodeControl();
      this.SuspendLayout();
      // 
      // labelColor
      // 
      this.labelColor.AutoSize = true;
      this.labelColor.Location = new System.Drawing.Point(3, 8);
      this.labelColor.Name = "labelColor";
      this.labelColor.Size = new System.Drawing.Size(34, 13);
      this.labelColor.TabIndex = 0;
      this.labelColor.Text = "Color:";
      // 
      // buttonColor
      // 
      this.buttonColor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.buttonColor.BackColor = System.Drawing.Color.White;
      this.buttonColor.Location = new System.Drawing.Point(47, 3);
      this.buttonColor.Name = "buttonColor";
      this.buttonColor.Size = new System.Drawing.Size(105, 23);
      this.buttonColor.TabIndex = 1;
      this.buttonColor.UseVisualStyleBackColor = false;
      this.buttonColor.Click += new System.EventHandler(this.ButtonColorClick);
      // 
      // checkBox1
      // 
      this.checkBox1.AutoSize = true;
      this.checkBox1.Location = new System.Drawing.Point(3, 32);
      this.checkBox1.Name = "checkBox1";
      this.checkBox1.Size = new System.Drawing.Size(99, 17);
      this.checkBox1.TabIndex = 2;
      this.checkBox1.Text = "Show section 2";
      this.checkBox1.UseVisualStyleBackColor = true;
      this.checkBox1.CheckedChanged += new System.EventHandler(this.CheckBox1CheckedChanged);
      // 
      // subNodeControl
      // 
      this.subNodeControl.AmountValue = 1D;
      this.subNodeControl.AmountValueEnabled = false;
      this.subNodeControl.AmountValueVaries = false;
      this.subNodeControl.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
      this.subNodeControl.BackColor = System.Drawing.SystemColors.Window;
      this.subNodeControl.CheckBoxEnabled = false;
      this.subNodeControl.ChildSlotName = "diffuse-child-slot";
      this.subNodeControl.IsChecked = false;
      this.subNodeControl.Location = new System.Drawing.Point(3, 53);
      this.subNodeControl.Name = "subNodeControl";
      this.subNodeControl.ShowAmountEditControl = true;
      this.subNodeControl.ShowCheckBox = true;
      this.subNodeControl.Size = new System.Drawing.Size(149, 20);
      this.subNodeControl.TabIndex = 3;
      // 
      // CustomMaterialUserInterfaceSection
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.subNodeControl);
      this.Controls.Add(this.checkBox1);
      this.Controls.Add(this.buttonColor);
      this.Controls.Add(this.labelColor);
      this.Name = "CustomMaterialUserInterfaceSection";
      this.Size = new System.Drawing.Size(155, 75);
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Label labelColor;
    private System.Windows.Forms.Button buttonColor;
    private System.Windows.Forms.CheckBox checkBox1;
    private RhinoWindows.Forms.Controls.Render.SubNodeControl subNodeControl;
  }
}
