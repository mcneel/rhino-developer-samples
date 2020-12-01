using System;
using System.Windows.Forms;

namespace WinFormsApp
{
  public partial class MainForm : Form
  {
    Rhino.Runtime.InProcess.RhinoCore _rhinoCore;
    public MainForm()
    {
      InitializeComponent();
    }

    protected override void OnHandleCreated(EventArgs e)
    {
      // This is a good spot to start Rhino.Inside as we now have a
      // handle to the top level parent window for Rhino
      _rhinoCore = new Rhino.Runtime.InProcess.RhinoCore(new string[] { "-appmode" }, Rhino.Runtime.InProcess.WindowStyle.Hidden, Handle);
      base.OnHandleCreated(e);
    }

    protected override void OnHandleDestroyed(EventArgs e)
    {
      _rhinoCore.Dispose();
      _rhinoCore = null;
      base.OnHandleDestroyed(e);
    }

    protected override void OnLoad(EventArgs e)
    {
      base.OnLoad(e);
      var displayModes = Rhino.Display.DisplayModeDescription.GetDisplayModes();
      foreach(var mode in displayModes)
      {
        var item = viewToolStripMenuItem.DropDownItems.Add(mode.EnglishName);
        item.Click += (s, evt) =>
        {
          viewportControl1.Viewport.DisplayMode = mode;
          viewportControl1.Invalidate();
        };
      }
    }

    private void openToolStripMenuItem_Click(object sender, EventArgs e)
    {
      var ofd = new OpenFileDialog();
      ofd.Filter = "Rhino file (*.3dm) | *.3dm";
      if (ofd.ShowDialog(this) == DialogResult.OK)
      {
        UseWaitCursor = true;
        Rhino.RhinoDoc.Open(ofd.FileName, out bool alreadyOpen);
        Text = $"Rhino.Inside ({ofd.FileName})";
        viewportControl1.Viewport.ZoomExtents();
        viewportControl1.Refresh();
        UseWaitCursor = false;
      }
    }
  }
}
