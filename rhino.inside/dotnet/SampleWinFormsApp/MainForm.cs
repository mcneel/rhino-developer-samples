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
        // temporary hack. The viewport control needs to be adjusted to
        // handle cases when the active doc changes
        this.Controls.Remove(viewportControl1);
        viewportControl1.MouseDown -= OnViewportMouseDown;
        var ctrl = new RhinoWindows.Forms.Controls.ViewportControl();
        ctrl.Location = viewportControl1.Location;
        ctrl.Size = viewportControl1.Size;
        ctrl.Dock = ctrl.Dock = DockStyle.Fill;
        ctrl.MouseDown += OnViewportMouseDown;
        viewportControl1.Dispose();
        this.Controls.Add(ctrl);
        viewportControl1 = ctrl;
        viewportControl1.Viewport.ZoomExtents();
        viewportControl1.Refresh();
        UseWaitCursor = false;
      }
    }

    private void OnViewportMouseDown(object sender, MouseEventArgs e)
    {
      var doc = Rhino.RhinoDoc.ActiveDoc;
      using (var pick = new Rhino.Input.Custom.PickContext())
      {
        var pickTransform = viewportControl1.Viewport.GetPickTransform(e.Location);
        pick.SetPickTransform(pickTransform);
        pick.PickStyle = Rhino.Input.Custom.PickStyle.PointPick;
        var objects = doc.Objects.PickObjects(pick);
        doc.Objects.UnselectAll();
        doc.Objects.Select(objects);
        viewportControl1.Refresh();
      }
    }
  }
}
