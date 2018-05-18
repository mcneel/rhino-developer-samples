using System.Windows.Forms;
using RhinoWindows.Forms;

namespace SampleCsRdk
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
      const int pad = 3;
      int runningYLoc = 3;
      this.SuspendLayout();
      this.components = new System.ComponentModel.Container();
      this.imageList = new ImageList(this.components);
      this.labelColor = new System.Windows.Forms.Label();
      this.buttonA = new System.Windows.Forms.Button();
      this.buttonB = new System.Windows.Forms.Button();
      this.buttonC = new System.Windows.Forms.Button();
      this.buttonColor = new System.Windows.Forms.Button();
      this.checkBox1 = new System.Windows.Forms.CheckBox();
      this.subNodeControl = new RhinoWindows.Forms.Controls.Render.SubNodeControl();
      this.bufferedTreeView = new BufferedTreeView();

      this.imageList.Images.Add(Properties.Resources.AtoZ);
      this.imageList.Images.SetKeyName(0, "ONE");

      this.buttonA.Location = new System.Drawing.Point(3, runningYLoc);
      this.buttonA.Name = "buttonA";
      this.buttonA.Text = "A";
      this.buttonA.Size = Dpi.ScaleSize(new System.Drawing.Size(20, 23));
      this.buttonA.TabIndex = 1;

      this.buttonB.Location = new System.Drawing.Point(26, runningYLoc);
      this.buttonB.Name = "buttonB";
      this.buttonB.Text = "B";
      this.buttonB.Size = Dpi.ScaleSize(new System.Drawing.Size(20, 23));
      this.buttonB.TabIndex = 1;

      this.buttonC.Location = new System.Drawing.Point(52, runningYLoc);
      this.buttonC.Name = "buttonC";
      this.buttonC.Text = "C";
      this.buttonC.Size = Dpi.ScaleSize(new System.Drawing.Size(20, 23));
      this.buttonC.TabIndex = 1;

      runningYLoc += this.buttonC.Height + pad;

      // special component
      this.bufferedTreeView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.bufferedTreeView.Location = new System.Drawing.Point(3, runningYLoc);
      this.bufferedTreeView.Size = Dpi.ScaleSize(new System.Drawing.Size(149, 200));
      this.bufferedTreeView.ImageList = this.imageList;
			this.bufferedTreeView.ItemHeight = Dpi.ScaleInt(30);
      this.bufferedTreeView.ImageIndex = 0;
      this.bufferedTreeView.SelectedImageIndex = 0;
      this.bufferedTreeView.BackColor = System.Drawing.Color.BlueViolet;

      var rootNode = new TreeNode("root");
      rootNode.ImageIndex = 0;
      rootNode.SelectedImageIndex = 1;

      for(int i = 0; i < 5; i++)
      {
        var chNode = new TreeNode("child");
        chNode.ImageIndex = 0;
        chNode.SelectedImageIndex = 0;
        rootNode.Nodes.Add(chNode);
      }
      this.bufferedTreeView.Nodes.Add(rootNode);

      runningYLoc += this.bufferedTreeView.Height + pad;
      // 
      // labelColor
      // 
      this.labelColor.AutoSize = true;
      this.labelColor.Location = new System.Drawing.Point(3, runningYLoc);
      this.labelColor.Name = "labelColor";
      this.labelColor.Size = Dpi.ScaleSize(new System.Drawing.Size(34, 13));
      this.labelColor.TabIndex = 0;
      this.labelColor.Text = "Color:";

      runningYLoc += this.labelColor.Height + pad;
      // 
      // buttonColor
      // 
      this.buttonColor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.buttonColor.BackColor = System.Drawing.Color.White;
      this.buttonColor.Location = new System.Drawing.Point(47, runningYLoc);
      this.buttonColor.Name = "buttonColor";
      this.buttonColor.Size = Dpi.ScaleSize(new System.Drawing.Size(105, 23));
      this.buttonColor.TabIndex = 1;
      this.buttonColor.UseVisualStyleBackColor = false;
      this.buttonColor.Click += new System.EventHandler(this.ButtonColorClick);

      runningYLoc += this.buttonColor.Height + pad;
      // checkBox1
      // 
      this.checkBox1.AutoSize = true;
      this.checkBox1.Location = new System.Drawing.Point(3, runningYLoc);
      this.checkBox1.Name = "checkBox1";
      this.checkBox1.Size = Dpi.ScaleSize(new System.Drawing.Size(99, 17));
      this.checkBox1.TabIndex = 2;
      this.checkBox1.Text = "Show section 2";
      this.checkBox1.UseVisualStyleBackColor = true;
      this.checkBox1.CheckedChanged += new System.EventHandler(this.CheckBox1CheckedChanged);

      runningYLoc += this.checkBox1.Height + pad;
      // 
      // subNodeControl
      // 
      this.subNodeControl.AmountValue = 1D;
      this.subNodeControl.AmountValueEnabled = false;
      this.subNodeControl.AmountValueVaries = false;
      this.subNodeControl.BackColor = System.Drawing.SystemColors.Window;
      this.subNodeControl.CheckBoxEnabled = false;
      this.subNodeControl.ChildSlotName = "diffuse-child-slot";
      this.subNodeControl.IsChecked = false;
      this.subNodeControl.Location = new System.Drawing.Point(3, runningYLoc);
      this.subNodeControl.Name = "subNodeControl";
      this.subNodeControl.ShowAmountEditControl = true;
      this.subNodeControl.ShowCheckBox = true;
      this.subNodeControl.Size = Dpi.ScaleSize(new System.Drawing.Size(149, 20));
      this.subNodeControl.TabIndex = 3;

      runningYLoc += this.subNodeControl.Height + pad;
      // slider
      this.trackBar = new TrackBar();
      this.trackBar.Location = new System.Drawing.Point(3, runningYLoc);
			this.trackBar.Size = Dpi.ScaleSize(new System.Drawing.Size(300, 40));
      this.trackBar.Name = "trackBarControl";
      this.trackBar.Orientation = Orientation.Horizontal;
      this.trackBar.TickStyle = TickStyle.Both;
      this.trackBar.Maximum = 100;
      this.trackBar.Minimum = 0;
      this.trackBar.TickFrequency = 1;
      this.trackBar.ValueChanged += TrackBar_ValueChanged;

      runningYLoc += this.trackBar.Height + pad;

      // slider position label
      this.trackPositionLabel = new Label();
      this.trackPositionLabel.Location = new System.Drawing.Point(3, runningYLoc);
      this.trackPositionLabel.Name = "trackPositionLabel";
			this.trackPositionLabel.Size = Dpi.ScaleSize(new System.Drawing.Size(100, 30));
      this.trackPositionLabel.Text = $"{this.trackBar.Value}";

      runningYLoc += trackPositionLabel.Height + pad;

      // 
      // CustomMaterialUserInterfaceSection, put it all together
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.Controls.Add(this.buttonA);
      this.Controls.Add(this.buttonB);
      this.Controls.Add(this.buttonC);
      this.Controls.Add(this.bufferedTreeView);
      this.Controls.Add(this.subNodeControl);
      this.Controls.Add(this.checkBox1);
      this.Controls.Add(this.buttonColor);
      this.Controls.Add(this.labelColor);
      this.Controls.Add(this.trackBar);
      this.Controls.Add(this.trackPositionLabel);
      this.Name = "CustomMaterialUserInterfaceSection";
      this.Size = Dpi.ScaleSize(new System.Drawing.Size(155, runningYLoc));
      this.ResumeLayout(false);
      this.PerformLayout();
      this.ResumeLayout(true);

    }

    #endregion

    private System.Windows.Forms.Button buttonA;
    private System.Windows.Forms.Button buttonB;
    private System.Windows.Forms.Button buttonC;
    private System.Windows.Forms.Label labelColor;
    private System.Windows.Forms.Label trackPositionLabel;
    private System.Windows.Forms.Button buttonColor;
    private System.Windows.Forms.CheckBox checkBox1;
    private System.Windows.Forms.ImageList imageList;
    private System.Windows.Forms.TrackBar trackBar;
    private RhinoWindows.Forms.Controls.Render.SubNodeControl subNodeControl;
    private BufferedTreeView bufferedTreeView;
  }
}
